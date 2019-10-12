#ifndef BIT_ARRAY_BIT_ARRAY_H
#define BIT_ARRAY_BIT_ARRAY_H

#include <inttypes.h>

typedef struct bit_array bit_array;
typedef uint64_t word_t, word_addr_t, bit_index_t;


#define BIT_INDEX_MIN 0
#define BIT_INDEX_MAX (~(bit_index_t)0)

#ifdef __cplusplus
extern "C" {
#endif

#include "bit_array_macros.h"

struct bit_array
{
	word_addr_t num_of_words;
	word_t *words;
};

bit_array *bit_array_create(word_addr_t num_of_words);
void bit_array_free(bit_array *);

#define bit_array_get(arr,i)      bitset_get((arr)->words, i)
#define bit_array_set(arr,i)      bitset_set((arr)->words, i)
#define bit_array_clear(arr,i)    bitset_del((arr)->words, i)
#define bit_array_toggle(arr,i)   bitset_tgl((arr)->words, i)
// c must be 0 or 1
#define bit_array_assign(arr,i,c) bitset_cpy((arr)->words,i,c)

#ifdef __cplusplus
}
#endif

#endif //BIT_ARRAY_BIT_ARRAY_H