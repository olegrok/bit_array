#include <assert.h>
#include "bit_array.h"
#include "stdlib.h"

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
	for (size_t i = 0; i < 5 * 64; i++) {
		bit_array_set(array, i);
	}
	bit_array_free(array);
}

static void
test_get()
{
	bit_array *array = bit_array_create(5);
	for (size_t i = 0; i < 5 * 64; i++) {
		bit_array_set(array, i);
	}

	for (size_t i = 0; i < 5 * 64; i++) {
		int bit = bit_array_get(array, i);
		assert(bit == 1);
	}

	bit_array_free(array);
}

static void
test_clear()
{
	bit_array *array = bit_array_create(5);
	for (size_t i = 0; i < 5 * 64; i++) {
		bit_array_set(array, i);
	}

	for (size_t i = 0; i < 5 * 64; i++) {
		int bit = bit_array_get(array, i);
		assert(bit == 1);
	}

	for (size_t i = 0; i < 5 * 64; i++) {
		bit_array_clear(array, i);
		for (size_t j = 0; i < 5 * 64; i++) {
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
	for (size_t i = 0; i < 5 * 64; i += 2) {
		bit_array_set(array, i);
	}

	for (size_t i = 0; i < 5 * 64; i++) {
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
	for (size_t i = 0; i < 5 * 64; i++) {
		bit_array_assign(array, i, i % 2 == 0);
		assert(bit_array_get(array, i) == (i % 2 == 0));
	}

	bit_array_free(array);
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
	return 0;
}