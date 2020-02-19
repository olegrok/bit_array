#include <assert.h>
#include <small/mempool.h>
#include <small/slab_cache.h>
#include <small/slab_arena.h>
#include <small/quota.h>
#include "bit_array.h"
#include "bit_array_interleave.h"

struct quota quota;
struct slab_arena arena;
struct slab_cache cache;

static void
test_one_dim()
{
	const size_t dim = 1;
	struct mempool pool;
	mempool_create(&pool, &cache, bit_array_bsize(dim));

	bit_array *arr = bit_array_create(&pool, dim);
	uint64_t keys[dim];

	uint64_t test_cases[] = {0, 1, -1ULL / 2, -1ULL};
	size_t size = sizeof(test_cases) / sizeof(uint64_t);

	struct bit_array_interleave_lookup_table *tables =
			bit_array_interleave_new_lookup_tables(&pool, dim);

	for (size_t i = 0; i < size; i++) {
		bit_array_clear_all(arr, dim);
		keys[0] = test_cases[i];
		bit_array_interleave(tables, keys, arr);
		assert(bit_array_get_word(arr, 0) == keys[0]);
	}

	bit_array_interleave_free_lookup_tables(tables);
	bit_array_free(&pool, arr);
	mempool_destroy(&pool);
}

static void
test_two_dims()
{
	const size_t dim = 2;
	struct mempool pool;
	mempool_create(&pool, &cache, bit_array_bsize(dim));

	bit_array *arr = bit_array_create(&pool, dim);
	uint64_t keys[dim];

	struct bit_array_interleave_lookup_table *tables =
			bit_array_interleave_new_lookup_tables(&pool, dim);

	keys[0] = keys[1] = 1;
	bit_array_interleave(tables, keys, arr);
	/* 0..01 and 0..01 -> 0..011 */
	assert(bit_array_get_word(arr, 0) == 3);
	assert(bit_array_get_word(arr, 1) == 0);
	bit_array_clear_all(arr, dim);

	keys[0] = keys[1] = -1ULL;
	bit_array_interleave(tables, keys, arr);
	/* 1..11 and 1..11 -> 1..11 */
	assert(bit_array_get_word(arr, 0) == -1ULL);
	assert(bit_array_get_word(arr, 1) == -1ULL);
	bit_array_clear_all(arr, dim);

	/* 1..11 */
	keys[0] = -1ULL;
	/* 0..00 */
	keys[1] = 0;
	bit_array_interleave(tables, keys, arr);
	/* 010..101 */
	assert(bit_array_get_word(arr, 0) == 6148914691236517205ULL);
	assert(bit_array_get_word(arr, 1) == 6148914691236517205ULL);
	bit_array_clear_all(arr, dim);

	/* 111 */
	keys[0] = 7;
	/* 100 */
	keys[1] = 4;
	bit_array_interleave(tables, keys, arr);
	/* 110101 */
	assert(bit_array_get_word(arr, 0) == 53);
	assert(bit_array_get_word(arr, 1) == 0);
	bit_array_clear_all(arr, dim);

	bit_array_interleave_free_lookup_tables(tables);
	bit_array_free(&pool, arr);
	mempool_destroy(&pool);
}

static void
test_20_dims()
{
	const size_t dim = 20;
	struct mempool pool;
	mempool_create(&pool, &cache, bit_array_bsize(dim));

	bit_array *arr = bit_array_create(&pool, dim);
	uint64_t keys[dim];

	for (size_t i = 0; i < dim; i++) {
		keys[i] = i;
	}

	struct bit_array_interleave_lookup_table *tables =
			bit_array_interleave_new_lookup_tables(&pool, dim);
	bit_array_interleave(tables, keys, arr);
	bit_array_interleave_free_lookup_tables(tables);
	bit_array_free(&pool, arr);
	mempool_destroy(&pool);
}

int
main()
{
	quota_init(&quota, QUOTA_MAX);
	slab_arena_create(&arena, &quota, 0, SLAB_MIN_SIZE, SLAB_ARENA_PRIVATE);
	slab_cache_create(&cache, &arena);

	test_one_dim();
	test_two_dims();
	test_20_dims();

	slab_cache_destroy(&cache);
	slab_arena_destroy(&arena);

	return 0;
}
