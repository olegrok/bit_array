#include <stdio.h>
#include <time.h>

#include "bit_array.h"
#include "bit_array_interleave.h"

static const long NANOSECONDS_IN_MILLIS = 1e6;
static const long NANOSECONDS_IN_SECONDS = 1e9;

static void
test_alloc(size_t count, size_t num_of_words)
{
	struct timespec start, stop;
	clock_gettime (CLOCK_MONOTONIC, &start);

	for (size_t i = 0; i < count; i++) {
		bit_array *array = bit_array_create(num_of_words);
		bit_array_free(array);
	}
	clock_gettime (CLOCK_MONOTONIC, &stop);
	long delta = NANOSECONDS_IN_SECONDS * (stop.tv_sec - start.tv_sec) +
			(stop.tv_nsec - start.tv_nsec);
	delta = delta / NANOSECONDS_IN_MILLIS;
	printf("%s: %ld cycles were completed in %ld millis for size = %ld\n",
		   __FUNCTION__, count, delta, num_of_words);
}

static void
test_interleave(size_t count, size_t dim)
{
	struct timespec start, stop;

	bit_array *arr = bit_array_create(dim);
	uint64_t keys[dim];

	for (size_t i = 0; i < dim; i++) {
		keys[i] = i;
	}

	struct bit_array_interleave_lookup_table *tables =
			bit_array_interleave_new_lookup_tables(dim);

	clock_gettime (CLOCK_MONOTONIC, &start);
	bit_array_interleave(tables, keys, arr);
	for (size_t i = 0; i < count; i++) {
		bit_array_interleave(tables, keys, arr);
	}
	clock_gettime (CLOCK_MONOTONIC, &stop);

	long delta = NANOSECONDS_IN_SECONDS * (stop.tv_sec - start.tv_sec) +
				 (stop.tv_nsec - start.tv_nsec);
	delta = delta / NANOSECONDS_IN_MILLIS;
	printf("%s: %ld cycles were completed in %ld millis for size = %ld\n",
		   __FUNCTION__, count, delta, dim);
	bit_array_interleave_free_lookup_tables(tables);
	bit_array_free(arr);
}

int
main()
{
	size_t count = 1e7;
	test_alloc(count, 1);
	test_alloc(count, 2);
	test_alloc(count, 3);
	test_alloc(count, 5);
	test_alloc(count, 10);
	test_alloc(count, 20);

	test_interleave(count, 1);
	test_interleave(count, 2);
	test_interleave(count, 3);
	test_interleave(count, 5);
	test_interleave(count, 10);
	test_interleave(count, 20);
	return 0;
}
