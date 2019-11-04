#include <stdlib.h>
#include "bit_array.h"

static const size_t LOOKUP_TABLE_SIZE = 256;
static const size_t LOOKUP_TABLE_BSIZE = 256 * sizeof(bit_array*);

static void
fill_table(bit_array **table, size_t dim, uint8_t shift)
{
	uint8_t one = 1;
	const size_t BIT_COUNT = 8;
	for(size_t i = 0; i < LOOKUP_TABLE_SIZE; i++) {
		uint8_t num = i;
		for (size_t j = 0; j < BIT_COUNT; j++) {
			if (num & one) {
				bit_array_set(table[i], j * dim + shift);
			}
			num >>= 1ULL;
		}
	}
}

static int
bit_array_interleave_new_lookup_table(size_t dim, bit_array** table,
		size_t shift)
{
	for (size_t i = 0; i < LOOKUP_TABLE_SIZE; i++) {
		table[i] = bit_array_create(dim);
		if (table[i] == NULL) {
			free(table);
			return -1;
		}
	}

	fill_table(table, dim, shift);
	return 0;
}

static void
bit_array_interleave_free_lookup_table(bit_array **table)
{
	for (size_t i = 0; i < LOOKUP_TABLE_SIZE; i++) {
		bit_array_free(table[i]);
	}
	free(table);
}

bit_array***
bit_array_interleave_new_lookup_tables(size_t dim)
{
	bit_array ***tables = malloc(dim * sizeof(bit_array**));
	if (tables == NULL) {
		return NULL;
	}

	for(size_t i = 0; i < dim; i++) {
		tables[i] = malloc(LOOKUP_TABLE_BSIZE);
		if (tables[i] == 0) {
			for(size_t j = 0; j < i; j++) {
				free(tables[j]);
			}
			free(tables);
			return NULL;
		}
	}

	for(size_t i = 0; i < dim; i++) {
		int res = bit_array_interleave_new_lookup_table(dim, tables[i], i);
		if (res < 0) {
			for(size_t j = 0; j < i; j++) {
				bit_array_interleave_free_lookup_table(tables[j]);
			}
			free(tables);
			return NULL;
		}
	}

	return tables;
}

void
bit_array_interleave_free_lookup_tables(bit_array ***tables, size_t dim)
{
	for (size_t i = 0; i < dim; i++) {
		bit_array_interleave_free_lookup_table(tables[i]);
	}
	free(tables);
}

int
bit_array_interleave(bit_array ***tables, size_t dim,
					 const uint64_t *in, bit_array *out)
{
	const size_t octets_count = 8;
	const size_t octet_size = 8;

	bit_array *tmp = bit_array_create(dim);
	if (tmp == NULL)
		return -1;

	for (size_t i = 0; i < octets_count; i++) {
		size_t shift = octet_size * i;
		for (size_t j = 0; j < dim; j++) {
			uint8_t octet = (in[j] >> shift);
			const bit_array *value = tables[j][octet];
			bit_array_or(tmp, tmp, value);
		}
		bit_array_shift_left(tmp, dim * shift);
		bit_array_or(out, out, tmp);
		bit_array_clear_all(tmp);
	}
	bit_array_free(tmp);
	return 0;
}
