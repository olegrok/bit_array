#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <inttypes.h>
#include <stdlib.h>

typedef struct bit_array bit_array;
typedef uint64_t word_t;
typedef uint8_t bit_index_t;
typedef uint8_t word_size_t;

#include "bit_array_macros.h"

struct bit_array
{
	word_size_t num_of_words;
	word_t *words;
};

size_t
bit_array_bsize(word_size_t num_of_words);

bit_array *
bit_array_create(word_size_t num_of_words);

bit_array *
bit_array_copy(bit_array *restrict dst, const bit_array *restrict src);

void
bit_array_free(bit_array *);

void
bit_array_add(bit_array *src, const bit_array *add);

void
bit_array_add_word(bit_array *bitarr, word_t value);

int
bit_array_cmp(const bit_array *left, const bit_array *right);

void
bit_array_set_all(bit_array *bitarr);

void
bit_array_clear_all(bit_array *bitarr);

void
bit_array_shift_left(bit_array *bitarr, bit_index_t shift_dist);

bit_index_t
bit_array_length(const bit_array *bit_arr);

bit_array *
bit_array_clone(const bit_array *bitarr);

void
bit_array_or(bit_array *dst, const bit_array *src);

void
bit_array_and(bit_array *dst, const bit_array *src);

uint64_t
bit_array_get_word(const bit_array *bitarr, bit_index_t num);

#define bit_array_get(arr,i)      bitset_get((arr)->words, i)
#define bit_array_set(arr,i)      bitset_set((arr)->words, i)
#define bit_array_clear(arr,i)    bitset_del((arr)->words, i)
#define bit_array_toggle(arr,i)   bitset_tgl((arr)->words, i)
// c must be 0 or 1
#define bit_array_assign(arr,i,c) bitset_cpy((arr)->words,i,c)

#endif //BIT_ARRAY_H
