#ifndef BIT_ARRAY_INTERLEAVE_H
#define BIT_ARRAY_INTERLEAVE_H

#include "bit_array.h"

bit_array***
bit_array_interleave_new_lookup_tables(size_t dim);

void
bit_array_interleave_free_lookup_tables(bit_array ***table, size_t dim);

int
bit_array_interleave(bit_array ***tables, size_t dim,
					 const uint64_t *in, bit_array *out);

#endif //BIT_ARRAY_INTERLEAVE_H
