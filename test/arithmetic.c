#include <assert.h>
#include <small/mempool.h>
#include <small/slab_cache.h>
#include <small/slab_arena.h>
#include <small/quota.h>
#include "bit_array.h"

#define WORD_SIZE 64

struct quota quota;
struct slab_arena arena;
struct slab_cache cache;

struct mempool pool_1;
struct mempool pool_2;

static void
test_simple()
{
	const size_t dim = 1;
	bit_array *src = bit_array_create(&pool_1, dim);
	bit_array *add = bit_array_create(&pool_1, dim);

	bit_array_set(src, 0);
	bit_array_set(add, 0);

	bit_array_add(src, add);
	assert(bit_array_get(src, 0) == 0);
	assert(bit_array_get(src, 1) == 1);

	bit_array_clear(src, 1);
	bit_array_set(src, 1);
	bit_array_add(src, add);

	assert(bit_array_get(src, 0) == 1);
	assert(bit_array_get(src, 1) == 1);

	bit_array_free(&pool_1, src);
	bit_array_free(&pool_1, add);
}

static void
test_carry()
{
	const size_t dim = 1;
	bit_array *src = bit_array_create(&pool_1, dim);
	bit_array *add = bit_array_create(&pool_1, dim);

	bit_array_set(add, 0);
	for (size_t i = 0; i < WORD_SIZE; ++i) {
		bit_array_set(src, i);
	}

	bit_array_add(src, add);

	for (size_t i = 0; i < WORD_SIZE; ++i) {
		assert(bit_array_get(src, i) == 0);
	}

	bit_array_free(&pool_1, src);
	bit_array_free(&pool_1, add);
}

static void
test_carry2()
{
	const size_t dim = 2;
	bit_array *src = bit_array_create(&pool_2, dim);
	bit_array *add = bit_array_create(&pool_2, dim);

	bit_array_set(add, 0);
	for (size_t i = 0; i < WORD_SIZE; ++i) {
		bit_array_set(src, i);
	}

	bit_array_add(src, add);

	for (size_t i = 0; i < 2 * WORD_SIZE; ++i) {
		assert(bit_array_get(src, i) == (i == WORD_SIZE));
	}

	bit_array_free(&pool_2, src);
	bit_array_free(&pool_2, add);
}

static void
test_add_uint64()
{
	const size_t dim = 1;
	bit_array *array = bit_array_create(&pool_1, dim);
	bit_array_add_uint64(array, 5);
	assert(bit_array_get(array, 0) == 1);
	assert(bit_array_get(array, 1) == 0);
	assert(bit_array_get(array, 2) == 1);
	assert(bit_array_get(array, 3) == 0);

	bit_array_set_all(array);
	bit_array_add_uint64(array, 1);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		assert(bit_array_get(array, 0) == 0);
	}
	bit_array_free(&pool_1, array);
}

static void
test_add_uint64_carry()
{
	const size_t dim = 2;
	bit_array *array = bit_array_create(&pool_2, dim);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		bit_array_set(array, i);
	}
	bit_array_add_uint64(array, 1);
	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == (i == WORD_SIZE));
	}
	bit_array_free(&pool_2, array);
}

static void
test_add_uint64_carry2()
{
	const size_t dim = 2;
	bit_array *array = bit_array_create(&pool_2, dim);
	bit_array_set_all(array);
	bit_array_clear(array, 0);
	bit_array_clear(array, WORD_SIZE);
	bit_array_add_uint64(array, 2);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == 0);
		assert(bit_array_get(array, i + WORD_SIZE) == 1);
	}
	bit_array_free(&pool_2, array);
}

static void
test_or()
{
	const size_t dim = 2;
	bit_array *first = bit_array_create(&pool_2, dim);
	bit_array *second = bit_array_create(&pool_2, dim);
	bit_array_set_all(first);

	bit_array_or(first, second);
	assert(bit_array_get_word(first, 0) == -1ULL);
	assert(bit_array_get_word(first, 1) == -1ULL);


	bit_array_clear_all(first);
	bit_array_clear_all(second);

	bit_array_set(first, 90);
	bit_array_set(first, 80);
	bit_array_set(first, 20);
	bit_array_set(first, 10);
	bit_array_set(second, 100);
	bit_array_set(second, 80);
	bit_array_set(second, 30);
	bit_array_set(second, 10);
	bit_array_or(first, second);

	assert(bit_array_get(first, 100) == 1);
	assert(bit_array_get(first, 90) == 1);
	assert(bit_array_get(first, 80) == 1);
	assert(bit_array_get(first, 30) == 1);
	assert(bit_array_get(first, 20) == 1);
	assert(bit_array_get(first, 10) == 1);

	bit_array_free(&pool_2, first);
	bit_array_free(&pool_2, second);
}

static void
test_and()
{
	const size_t dim = 2;
	bit_array *first = bit_array_create(&pool_2, dim);
	bit_array *second = bit_array_create(&pool_2, dim);
	bit_array_set_all(first);

	bit_array_and(first, second);
	assert(bit_array_get_word(first, 0) == 0);
	assert(bit_array_get_word(first, 1) == 0);

	bit_array_clear_all(first);
	bit_array_clear_all(second);

	bit_array_set(first, 90);
	bit_array_set(first, 80);
	bit_array_set(first, 20);
	bit_array_set(first, 10);
	bit_array_set(second, 100);
	bit_array_set(second, 80);
	bit_array_set(second, 30);
	bit_array_set(second, 10);
	bit_array_and(first, second);

	assert(bit_array_get(first, 100) == 0);
	assert(bit_array_get(first, 90) == 0);
	assert(bit_array_get(first, 80) == 1);
	assert(bit_array_get(first, 30) == 0);
	assert(bit_array_get(first, 20) == 0);
	assert(bit_array_get(first, 10) == 1);

	bit_array_free(&pool_2, first);
	bit_array_free(&pool_2, second);
}

int
main()
{
	quota_init(&quota, QUOTA_MAX);
	slab_arena_create(&arena, &quota, 0, SLAB_MIN_SIZE, SLAB_ARENA_PRIVATE);
	slab_cache_create(&cache, &arena);

	mempool_create(&pool_1, &cache, bit_array_bsize(1));
	mempool_create(&pool_2, &cache, bit_array_bsize(2));

	test_simple();
	test_carry();
	test_carry2();
	test_add_uint64();
	test_add_uint64_carry();
	test_add_uint64_carry2();
	test_or();
	test_and();

	mempool_destroy(&pool_1);
	mempool_destroy(&pool_2);
	slab_cache_destroy(&cache);
	slab_arena_destroy(&arena);

	return 0;
}
