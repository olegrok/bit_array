#include <assert.h>
#include <small/mempool.h>
#include <small/slab_cache.h>
#include <small/slab_arena.h>
#include <small/quota.h>
#include "bit_array.h"
#include "stdlib.h"

#define WORD_SIZE 64

struct quota quota;
struct slab_arena arena;
struct slab_cache cache;

struct mempool pool_5;
struct mempool pool_2;

static void
test_dummy()
{
	const size_t size = 5;
	bit_array *array = bit_array_create(&pool_5, size);
	bit_array_free(&pool_5, array);
}

static void
test_set()
{
	const size_t size = 5;
	bit_array *array = bit_array_create(&pool_5, size);
	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_set(array, i);
	}
	bit_array_free(&pool_5, array);
}

static void
test_get()
{
	bit_array *array = bit_array_create(&pool_5, 5);
	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_set(array, i);
	}

	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == 1);
	}

	bit_array_free(&pool_5, array);
}

static void
test_clear()
{
	bit_array *array = bit_array_create(&pool_5, 5);
	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_set(array, i);
	}

	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == 1);
	}

	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_clear(array, i);
		for (size_t j = 0; i < 5 * WORD_SIZE; i++) {
			assert(bit_array_get(array, j) == (j > i));
		}
	}

	bit_array_free(&pool_5, array);
}

static void
test_toggle()
{
	bit_array *array = bit_array_create(&pool_5, 5);
	for (size_t i = 0; i < 5 * WORD_SIZE; i += 2) {
		bit_array_set(array, i);
	}

	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		uint8_t bit = bit_array_get(array, i);
		bit_array_toggle(array, i);
		assert(bit != bit_array_get(array, i));
	}

	bit_array_free(&pool_5, array);
}

static void
test_assign()
{
	bit_array *array = bit_array_create(&pool_5, 5);
	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_assign(array, i, i % 2 == 0);
		assert(bit_array_get(array, i) == (i % 2 == 0));
	}

	bit_array_free(&pool_5, array);
}

static void
test_set_all()
{
	bit_array *array = bit_array_create(&pool_2, 2);

	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		bit_array_clear(array, i);
		assert(bit_array_get(array, i) == 0);
	}

	bit_array_set_all(array);
	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == 1);
	}

	bit_array_free(&pool_2, array);
}

static void
test_clear_all()
{
	bit_array *array = bit_array_create(&pool_2, 2);

	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		bit_array_set(array, i);
		assert(bit_array_get(array, i) == 1);
	}

	bit_array_clear_all(array);
	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == 0);
	}

	bit_array_free(&pool_2, array);
}

static void
test_length()
{
	bit_array *array2 = bit_array_create(&pool_2, 2);
	bit_array *array5 = bit_array_create(&pool_5, 5);

	assert(bit_array_length(array2) == 128);
	assert(bit_array_length(array5) == 320);

	bit_array_free(&pool_2, array2);
	bit_array_free(&pool_5, array5);
}

static void
test_num_of_words()
{
	bit_array *array2 = bit_array_create(&pool_2, 2);
	bit_array *array5 = bit_array_create(&pool_5, 5);

	assert(bit_array_num_of_words(array2) == 2);
	assert(bit_array_num_of_words(array5) == 5);

	bit_array_free(&pool_2, array2);
	bit_array_free(&pool_5, array5);
}

static void
test_clone()
{
	bit_array *array = bit_array_create(&pool_5, 5);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		bit_array_set(array, i);
		bit_array_set(array, 3 * WORD_SIZE + i);
	}

	bit_array *clone_array = bit_array_clone(&pool_5, array);
	assert(array != clone_array);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		assert(bit_array_get(clone_array, i) == 1);
		assert(bit_array_get(array, 3 * WORD_SIZE + i) == 1);
	}

	bit_array_free(&pool_5, array);
	bit_array_free(&pool_5, clone_array);
}

static void
test_copy()
{
	bit_array *array = bit_array_create(&pool_5, 5);
	bit_array *copy_array = bit_array_create(&pool_5, 5);

	for (size_t i = 0; i < WORD_SIZE; i++) {
		bit_array_set(array, i);
		bit_array_set(array, 3 * WORD_SIZE + i);
	}

	bit_array_copy(copy_array, array);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		assert(bit_array_get(copy_array, i) == 1);
		assert(bit_array_get(array, 3 * WORD_SIZE + i) == 1);
	}

	bit_array_free(&pool_5, array);
	bit_array_free(&pool_5, copy_array);
}

static void
test_shift()
{
	bit_array *array = bit_array_create(&pool_2, 2);

	bit_array_set(array, 0);
	bit_array_shift_left(array, 0);

	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == (i == 0));
	}

	bit_array_shift_left(array, 1);

	assert(bit_array_get(array, 0) == 0);
	assert(bit_array_get(array, 1) == 1);

	bit_array_shift_left(array, WORD_SIZE);

	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == (i == (WORD_SIZE + 1)));
	}

	bit_array_set(array, 0);
	bit_array_shift_left(array, 1);

	assert(bit_array_get(array, 1) == 1);
	assert(bit_array_get(array, WORD_SIZE + 2) == 1);
	bit_array_free(&pool_2, array);

	array = bit_array_create(&pool_2, 2);
	bit_array_set(array, 0);
	bit_array_shift_left(array, 2 * WORD_SIZE);
	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == 0);
	}

	bit_array_clear_all(array);
	bit_array_add_word(array, -1ULL);
	assert(bit_array_get_word(array, 0) == -1ULL);
	assert(bit_array_get_word(array, 1) == 0);
	bit_array_shift_left(array, WORD_SIZE);
	assert(bit_array_get_word(array, 0) == 0);
	assert(bit_array_get_word(array, 1) == -1ULL);
	bit_array_shift_left(array, WORD_SIZE);
	assert(bit_array_get_word(array, 0) == 0);
	assert(bit_array_get_word(array, 1) == 0);

	bit_array_free(&pool_2, array);
}

static void
test_get_word()
{
	bit_array *array = bit_array_create(&pool_5, 5);

	bit_array_set(array, 4 * 64 + 5);
	bit_array_set(array, 3 * 64 + 4);
	bit_array_set(array, 2 * 64 + 3);
	bit_array_set(array, 1 * 64 + 2);
	bit_array_set(array, 0 * 64 + 1);

	assert(bit_array_get_word(array, 4) == 1 << 5);
	assert(bit_array_get_word(array, 3) == 1 << 4);
	assert(bit_array_get_word(array, 2) == 1 << 3);
	assert(bit_array_get_word(array, 1) == 1 << 2);
	assert(bit_array_get_word(array, 0) == 1 << 1);

	bit_array_free(&pool_5, array);
}

int
main()
{
	quota_init(&quota, QUOTA_MAX);
	slab_arena_create(&arena, &quota, 0, SLAB_MIN_SIZE, SLAB_ARENA_PRIVATE);
	slab_cache_create(&cache, &arena);

	mempool_create(&pool_5, &cache, bit_array_bsize(5));
	mempool_create(&pool_2, &cache, bit_array_bsize(2));

	test_dummy();
	test_set();
	test_get();
	test_clear();
	test_toggle();
	test_assign();
	test_set_all();
	test_clear_all();
	test_length();
	test_num_of_words();
	test_clone();
	test_copy();
	test_shift();
	test_get_word();

	mempool_destroy(&pool_5);
	mempool_destroy(&pool_2);
	slab_cache_destroy(&cache);
	slab_arena_destroy(&arena);

	return 0;
}
