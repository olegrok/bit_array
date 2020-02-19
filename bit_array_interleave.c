#include <stdlib.h>
#include <assert.h>

#include "bit_array.h"
#include "bit_array_interleave.h"

#define LOOKUP_TABLE_SIZE 256
#define BIT_COUNT 8

static void
fill_table(bit_array **table, uint8_t dim, uint8_t shift)
{
	for (uint16_t i = 0; i < LOOKUP_TABLE_SIZE; i++) {
		uint8_t num = i;
		for (uint8_t j = 0; j < BIT_COUNT; j++) {
			if (num & 1U) {
				bit_array_set(table[i], j * dim + shift);
			}
			num >>= 1ULL;
		}
	}
}

static void
bit_array_interleave_free_dim_lookup_table(struct mempool *pool,
		bit_array **table)
{
	if (table == NULL) {
		return;
	}

	for (uint16_t i = 0; i < LOOKUP_TABLE_SIZE; i++) {
		if (table[i] != NULL) {
			bit_array_free(pool, table[i]);
			table[i] = NULL;
		}
	}
	free(table);
}

struct bit_array_interleave_lookup_table *
bit_array_interleave_new_lookup_tables(struct mempool *pool, uint8_t dim)
{
	assert(dim > 0);
	/* Allocate memory for structure */
	struct bit_array_interleave_lookup_table *table = calloc(1,
			sizeof(struct bit_array_interleave_lookup_table));

	if (table == NULL) {
		return NULL;
	}
	table->dim = dim;
	table->pool = pool;

	/* Allocate memory for buffer */
	table->buffer = bit_array_create(pool, dim);
	if (table->buffer == NULL) {
		free(table);
		return NULL;
	}

	/* Allocate memory for lookup tables for each dimension */
	table->tables = calloc(dim, sizeof(bit_array***));
	if (table->tables == NULL) {
		free(table->buffer);
		free(table);
		return NULL;
	}

	for(uint8_t i = 0; i < dim; i++) {
		table->tables[i] = calloc(LOOKUP_TABLE_SIZE, sizeof(bit_array**));
		if (table->tables[i] == NULL) {
			bit_array_interleave_free_lookup_tables(table);
			return NULL;
		}

		for (uint16_t j = 0; j < LOOKUP_TABLE_SIZE; j++) {
			table->tables[i][j] = bit_array_create(pool, dim);
			if (table->tables[i] == NULL) {
				bit_array_interleave_free_lookup_tables(table);
				return NULL;
			}
		}

		fill_table(table->tables[i], dim, i);
	}

	return table;
}

void
bit_array_interleave_free_lookup_tables(
		struct bit_array_interleave_lookup_table *table)
{

	if (table->tables != NULL) {
		for (uint8_t i = 0; i < table->dim; i++) {
			bit_array_interleave_free_dim_lookup_table(table->pool,
					table->tables[i]);
			table->tables[i] = NULL;
		}

		free(table->tables);
		table->tables = NULL;
	}

	if (table->buffer != NULL) {
		mempool_free(table->pool, table->buffer);
		table->buffer = NULL;
	}

	table->dim = 0;
	free(table);
}

int
bit_array_interleave(struct bit_array_interleave_lookup_table *table,
					 const uint64_t *in, bit_array *out)
{
	const uint8_t octets_count_in_word = 8;
	const uint8_t octet_size_in_bits = 8;
	const uint8_t dim = table->dim;

	bit_array_clear_all(table->buffer, dim);
	for (uint8_t i = 0; i < octets_count_in_word; i++) {
		uint16_t shift = octet_size_in_bits * i;
		for (uint8_t j = 0; j < dim; j++) {
			uint8_t octet = (in[j] >> shift);
			const bit_array *value = table->tables[j][octet];
			bit_array_or(table->buffer, value, dim);
		}
		bit_array_shift_left(table->buffer, dim * shift, dim);
		bit_array_or(out, table->buffer, dim);
		bit_array_clear_all(table->buffer, dim);
	}

	return 0;
}
