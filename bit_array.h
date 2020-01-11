#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <inttypes.h>
#include <stdlib.h>
#include <small/mempool.h>

typedef struct bit_array bit_array;
typedef uint64_t word_t, word_addr_t, bit_index_t;

#ifdef __cplusplus
extern "C" {
#endif

#include "bit_array_macros.h"

struct bit_array
{
	word_addr_t num_of_words;
	word_t *words;
};

size_t
bit_array_bsize(word_addr_t num_of_words);

bit_array *
bit_array_create(struct mempool *pool, word_addr_t num_of_words);

bit_array *
bit_array_copy(bit_array *restrict dst, const bit_array *restrict src);

void
bit_array_free(struct mempool *pool, bit_array *array);

void
bit_array_add(bit_array *src, const bit_array *add);

void
bit_array_add_uint64(bit_array *bitarr, uint64_t value);

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
bit_array_clone(struct mempool *pool, const bit_array *bitarr);

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

#ifdef __cplusplus
}
#endif

#endif //BIT_ARRAY_H
