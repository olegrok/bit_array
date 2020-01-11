#ifndef BIT_ARRAY_INTERLEAVE_H
#define BIT_ARRAY_INTERLEAVE_H

#include <small/mempool.h>
#include "bit_array.h"

struct bit_array_interleave_lookup_table {
	/**
	 * Contains octet lookup table with
	 * shifts for each dimension
	 */
	bit_array ***tables;
	/**
	 * Preallocated buffer using in process of interleaving
	 */
	bit_array *buffer;
	/**
	 * Amount of dimensions
	 */
	size_t dim;
	/**
	 * Mempool ptr
	 */
	 struct mempool *pool;
};

struct bit_array_interleave_lookup_table *
bit_array_interleave_new_lookup_tables(struct mempool *pool, size_t dim);

void
bit_array_interleave_free_lookup_tables(
		struct bit_array_interleave_lookup_table *table);

int
bit_array_interleave(struct bit_array_interleave_lookup_table *table,
					 const uint64_t *in, bit_array *out);

#endif //BIT_ARRAY_INTERLEAVE_H
