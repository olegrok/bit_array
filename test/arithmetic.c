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
	bit_array_free(array);
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
	bit_array_free(array);
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
	bit_array_free(array);
}

static void
test_or()
{
	bit_array *first = bit_array_create(2);
	bit_array *second = bit_array_create(2);
	bit_array *result = bit_array_create(2);
	bit_array_set_all(first);

	bit_array_or(result, first, second);
	assert(bit_array_cmp(result, first) == 0);

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
	bit_array_or(result, first, second);

	assert(bit_array_get(result, 100) == 1);
	assert(bit_array_get(result, 90) == 1);
	assert(bit_array_get(result, 80) == 1);
	assert(bit_array_get(result, 30) == 1);
	assert(bit_array_get(result, 20) == 1);
	assert(bit_array_get(result, 10) == 1);

	bit_array_free(first);
	bit_array_free(second);
	bit_array_free(result);
}

static void
test_and()
{
	bit_array *first = bit_array_create(2);
	bit_array *second = bit_array_create(2);
	bit_array *result = bit_array_create(2);
	bit_array_set_all(first);

	bit_array_and(result, first, second);
	assert(bit_array_cmp(result, second) == 0);

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
	bit_array_and(result, first, second);

	assert(bit_array_get(result, 100) == 0);
	assert(bit_array_get(result, 90) == 0);
	assert(bit_array_get(result, 80) == 1);
	assert(bit_array_get(result, 30) == 0);
	assert(bit_array_get(result, 20) == 0);
	assert(bit_array_get(result, 10) == 1);

	bit_array_free(first);
	bit_array_free(second);
	bit_array_free(result);
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
	test_or();
	test_and();
	return 0;
}
