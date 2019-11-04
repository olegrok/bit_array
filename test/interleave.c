#include <assert.h>
#include "bit_array.h"
#include "bit_array_interleave.h"

static void
test_one_dim()
{
	const size_t dim = 1;
	bit_array *arr = bit_array_create(dim);
	uint64_t keys[dim];

	uint64_t test_cases[] = {0, 1, -1ULL / 2, -1ULL};
	size_t size = sizeof(test_cases) / sizeof(uint64_t);

	bit_array ***tables = bit_array_interleave_new_lookup_tables(dim);

	for (size_t i = 0; i < size; i++) {
		bit_array_clear_all(arr);
		keys[0] = test_cases[i];
		bit_array_interleave(tables, dim, keys, arr);
		assert(bit_array_get_word(arr, 0) == keys[0]);
	}

	bit_array_interleave_free_lookup_tables(tables, dim);
	bit_array_free(arr);
}

static void
test_two_dims()
{
	const size_t dim = 2;
	bit_array *arr = bit_array_create(dim);
	uint64_t keys[dim];

	bit_array ***tables = bit_array_interleave_new_lookup_tables(dim);

	keys[0] = keys[1] = 1;
	bit_array_interleave(tables, dim, keys, arr);
	/* 0..01 and 0..01 -> 0..011 */
	assert(bit_array_get_word(arr, 0) == 3);
	assert(bit_array_get_word(arr, 1) == 0);
	bit_array_clear_all(arr);

	keys[0] = keys[1] = -1ULL;
	bit_array_interleave(tables, dim, keys, arr);
	/* 1..11 and 1..11 -> 1..11 */
	assert(bit_array_get_word(arr, 0) == -1ULL);
	assert(bit_array_get_word(arr, 1) == -1ULL);
	bit_array_clear_all(arr);

	/* 1..11 */
	keys[0] = -1ULL;
	/* 0..00 */
	keys[1] = 0;
	bit_array_interleave(tables, dim, keys, arr);
	/* 010..101 */
	assert(bit_array_get_word(arr, 0) == 6148914691236517205ULL);
	assert(bit_array_get_word(arr, 1) == 6148914691236517205ULL);
	bit_array_clear_all(arr);

	/* 111 */
	keys[0] = 7;
	/* 100 */
	keys[1] = 4;
	bit_array_interleave(tables, dim, keys, arr);
	/* 110101 */
	assert(bit_array_get_word(arr, 0) == 53);
	assert(bit_array_get_word(arr, 1) == 0);
	bit_array_clear_all(arr);

	bit_array_interleave_free_lookup_tables(tables, dim);
	bit_array_free(arr);
}

static void
test_20_dims()
{
	const size_t dim = 20;
	bit_array *arr = bit_array_create(dim);
	uint64_t keys[dim];

	for (size_t i = 0; i < dim; i++) {
		keys[i] = i;
	}

	bit_array ***tables = bit_array_interleave_new_lookup_tables(dim);
	bit_array_interleave(tables, dim, keys, arr);
	bit_array_interleave_free_lookup_tables(tables, dim);
	bit_array_free(arr);
}

int
main()
{
	test_one_dim();
	test_two_dims();
	test_20_dims();
	return 0;
}
