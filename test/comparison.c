#include <assert.h>
#include "bit_array.h"
#include "stdlib.h"

static void
test_one()
{
	bit_array *src = bit_array_create(1);
	bit_array *add = bit_array_create(1);

	assert(bit_array_cmp(src, add) == 0);
	assert(bit_array_cmp(add, src) == 0);

	bit_array_set(src, 0);
	bit_array_set(add, 0);
	assert(bit_array_cmp(src, add) == 0);
	assert(bit_array_cmp(add, src) == 0);

	bit_array_add(src, add);
	assert(bit_array_cmp(src, add) > 0);
	assert(bit_array_cmp(add, src) < 0);

	bit_array_free(src);
	bit_array_free(add);
}

static void
test_two()
{
	bit_array *src = bit_array_create(2);
	bit_array *add = bit_array_create(2);

	const size_t WORD_SIZE = 64;

	bit_array_set(src, WORD_SIZE);
	bit_array_set(add, 0);
	assert(bit_array_cmp(src, add) > 0);
	assert(bit_array_cmp(add, src) < 0);

	bit_array_add(add, src);
	assert(bit_array_cmp(src, add) < 0);
	assert(bit_array_cmp(add, src) > 0);

	bit_array_free(src);
	bit_array_free(add);
}

int
main()
{
	test_one();
	test_two();
	return 0;
}
