#include <assert.h>
#include <small/mempool.h>
#include <small/slab_cache.h>
#include <small/slab_arena.h>
#include <small/quota.h>
#include "bit_array.h"

struct quota quota;
struct slab_arena arena;
struct slab_cache cache;

static void
test_one()
{
	const size_t dim = 1;
	struct mempool pool;
	mempool_create(&pool, &cache, bit_array_bsize(dim));

	bit_array *src = bit_array_create(&pool, dim);
	bit_array *add = bit_array_create(&pool, dim);

	assert(bit_array_cmp(src, add) == 0);
	assert(bit_array_cmp(add, src) == 0);

	bit_array_set(src, 0);
	bit_array_set(add, 0);
	assert(bit_array_cmp(src, add) == 0);
	assert(bit_array_cmp(add, src) == 0);

	bit_array_add(src, add);
	assert(bit_array_cmp(src, add) > 0);
	assert(bit_array_cmp(add, src) < 0);

	bit_array_free(&pool, src);
	bit_array_free(&pool, add);
}

static void
test_two()
{
	const size_t dim = 2;
	struct mempool pool;
	mempool_create(&pool, &cache, bit_array_bsize(dim));

	bit_array *src = bit_array_create(&pool, dim);
	bit_array *add = bit_array_create(&pool, dim);
	assert(bit_array_cmp(src, add) == 0);

	const size_t WORD_SIZE = 64;

	bit_array_set(src, WORD_SIZE);
	bit_array_set(add, 0);
	assert(bit_array_cmp(src, add) > 0);
	assert(bit_array_cmp(add, src) < 0);

	bit_array_add(add, src);
	assert(bit_array_cmp(src, add) < 0);
	assert(bit_array_cmp(add, src) > 0);

	bit_array_free(&pool, src);
	bit_array_free(&pool, add);
}

int
main()
{
	quota_init(&quota, QUOTA_MAX);
	slab_arena_create(&arena, &quota, 0, SLAB_MIN_SIZE, SLAB_ARENA_PRIVATE);
	slab_cache_create(&cache, &arena);

	test_one();
	test_two();

	slab_cache_destroy(&cache);
	slab_arena_destroy(&arena);

	return 0;
}
