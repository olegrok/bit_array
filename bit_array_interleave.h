#ifndef BIT_ARRAY_INTERLEAVE_H
#define BIT_ARRAY_INTERLEAVE_H

#include "bit_array.h"

bit_array**
bit_array_interleave_new_lookup_table(size_t dim);

void
bit_array_interleave_free_lookup_table(bit_array **table);

int
bit_array_interleave(size_t dim, bit_array **table,
					 const uint64_t *in, bit_array *out);

#endif //BIT_ARRAY_INTERLEAVE_H
