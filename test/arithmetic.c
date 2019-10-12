#include <assert.h>
#include "bit_array.h"
#include "stdlib.h"

#define WORD_SIZE 64

static void
test_simple()
{
	bit_array *src = bit_array_create(1);
	bit_array *add = bit_array_create(1);

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

	bit_array_free(src);
	bit_array_free(add);
}

static void
test_carry()
{
	bit_array *src = bit_array_create(1);
	bit_array *add = bit_array_create(1);

	bit_array_set(add, 0);
	for (size_t i = 0; i < WORD_SIZE; ++i) {
		bit_array_set(src, i);
	}

	bit_array_add(src, add);

	for (size_t i = 0; i < WORD_SIZE; ++i) {
		assert(bit_array_get(src, i) == 0);
	}

	bit_array_free(src);
	bit_array_free(add);
}

static void
test_carry2()
{
	bit_array *src = bit_array_create(2);
	bit_array *add = bit_array_create(2);

	bit_array_set(add, 0);
	for (size_t i = 0; i < WORD_SIZE; ++i) {
		bit_array_set(src, i);
	}

	bit_array_add(src, add);

	for (size_t i = 0; i < 2 * WORD_SIZE; ++i) {
		assert(bit_array_get(src, i) == (i == WORD_SIZE));
	}

	bit_array_free(src);
	bit_array_free(add);
}

static void
test_add_uint64()
{
	bit_array *array = bit_array_create(1);
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
}

static void
test_add_uint64_carry()
{
	bit_array *array = bit_array_create(2);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		bit_array_set(array, i);
	}
	bit_array_add_uint64(array, 1);
	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == (i == WORD_SIZE));
	}
}

static void
test_add_uint64_carry2()
{
	bit_array *array = bit_array_create(2);
	bit_array_set_all(array);
	bit_array_clear(array, 0);
	bit_array_clear(array, WORD_SIZE);
	bit_array_add_uint64(array, 2);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == 0);
		assert(bit_array_get(array, i + WORD_SIZE) == 1);
	}
}

int
main()
{
	test_simple();
	test_carry();
	test_carry2();
	test_add_uint64();
	test_add_uint64_carry();
	test_add_uint64_carry2();
	return 0;
}
