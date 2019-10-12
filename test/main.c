#include <assert.h>
#include "bit_array.h"
#include "stdlib.h"

#define WORD_SIZE 64

static void
test_dummy()
{
	bit_array *array = bit_array_create(5);
	bit_array_free(array);
}

static void
test_set()
{
	bit_array *array = bit_array_create(5);
	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_set(array, i);
	}
	bit_array_free(array);
}

static void
test_get()
{
	bit_array *array = bit_array_create(5);
	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_set(array, i);
	}

	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		int bit = bit_array_get(array, i);
		assert(bit == 1);
	}

	bit_array_free(array);
}

static void
test_clear()
{
	bit_array *array = bit_array_create(5);
	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_set(array, i);
	}

	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		int bit = bit_array_get(array, i);
		assert(bit == 1);
	}

	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_clear(array, i);
		for (size_t j = 0; i < 5 * WORD_SIZE; i++) {
			int bit = bit_array_get(array, j);
			assert(bit == (j > i));
		}
	}

	bit_array_free(array);
}

static void
test_toggle()
{
	bit_array *array = bit_array_create(5);
	for (size_t i = 0; i < 5 * WORD_SIZE; i += 2) {
		bit_array_set(array, i);
	}

	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		int bit = bit_array_get(array, i);
		bit_array_toggle(array, i);
		assert(bit != bit_array_get(array, i));
	}

	bit_array_free(array);
}

static void
test_assign()
{
	bit_array *array = bit_array_create(5);
	for (size_t i = 0; i < 5 * WORD_SIZE; i++) {
		bit_array_assign(array, i, i % 2 == 0);
		assert(bit_array_get(array, i) == (i % 2 == 0));
	}

	bit_array_free(array);
}

static void
test_set_all()
{
	bit_array *array = bit_array_create(2);

	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		bit_array_clear(array, i);
		assert(bit_array_get(array, i) == 0);
	}

	bit_array_set_all(array);
	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == 1);
	}

	bit_array_free(array);
}

static void
test_clear_all()
{
	bit_array *array = bit_array_create(2);

	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		bit_array_set(array, i);
		assert(bit_array_get(array, i) == 1);
	}

	bit_array_clear_all(array);
	for (size_t i = 0; i < 2 * WORD_SIZE; i++) {
		assert(bit_array_get(array, i) == 0);
	}

	bit_array_free(array);
}

static void
test_length()
{
	bit_array *array1 = bit_array_create(1);
	bit_array *array2 = bit_array_create(2);
	bit_array *array3 = bit_array_create(3);

	assert(bit_array_length(array1) == 64);
	assert(bit_array_length(array2) == 128);
	assert(bit_array_length(array3) == 192);

	bit_array_free(array1);
	bit_array_free(array2);
	bit_array_free(array3);
}

static void
test_num_of_words()
{
	bit_array *array1 = bit_array_create(1);
	bit_array *array2 = bit_array_create(2);
	bit_array *array3 = bit_array_create(3);

	assert(bit_array_num_of_words(array1) == 1);
	assert(bit_array_num_of_words(array2) == 2);
	assert(bit_array_num_of_words(array3) == 3);

	bit_array_free(array1);
	bit_array_free(array2);
	bit_array_free(array3);
}

static void
test_clone()
{
	bit_array *array = bit_array_create(5);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		bit_array_set(array, i);
		bit_array_set(array, 3 * WORD_SIZE + i);
	}

	bit_array *clone_array = bit_array_clone(array);
	assert(array != clone_array);
	for (size_t i = 0; i < WORD_SIZE; i++) {
		assert(bit_array_get(clone_array, i) == 1);
		assert(bit_array_get(array, 3 * WORD_SIZE + i) == 1);
	}
}

int
main()
{
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
	return 0;
}