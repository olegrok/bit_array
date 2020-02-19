#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <inttypes.h>
#include <stdlib.h>
#include <small/mempool.h>

typedef uint64_t bit_array;
typedef uint64_t word_t;
typedef uint16_t bit_index_t;
typedef uint8_t word_size_t;

#include "bit_array_macros.h"

size_t
bit_array_bsize(word_size_t num_of_words);

bit_array *
bit_array_create(struct mempool *pool, word_size_t num_of_words);

bit_array *
bit_array_copy(bit_array *restrict dst, const bit_array *restrict src, word_size_t num_of_words);

void
bit_array_free(struct mempool *pool, bit_array *array);

void
bit_array_add(bit_array *src, const bit_array *add, word_size_t num_of_words);

void
bit_array_add_word(bit_array *bitarr, word_t value, word_size_t num_of_words);

int
bit_array_cmp(const bit_array *left, const bit_array *right, word_size_t num_of_words);

void
bit_array_set_all(bit_array *bitarr, word_size_t num_of_words);

void
bit_array_clear_all(bit_array *bitarr, word_size_t num_of_words);

void
bit_array_shift_left(bit_array *bitarr, bit_index_t shift_dist, word_size_t num_of_words);

bit_index_t
bit_array_length(word_size_t num_of_words);

bit_array *
bit_array_clone(struct mempool *pool, const bit_array *bitarr, word_size_t num_of_words);

void
bit_array_or(bit_array *dst, const bit_array *src, word_size_t num_of_words);

void
bit_array_and(bit_array *dst, const bit_array *src, word_size_t num_of_words);

uint64_t
bit_array_get_word(const bit_array *bitarr, word_size_t num);

#endif //BIT_ARRAY_H
