#include "bit_array.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define WORD_MAX  (~(word_t)0)
#define WORD_SIZE 64

size_t
bit_array_bsize(word_addr_t num_of_words)
{
	assert(num_of_words > 0);
	return sizeof(bit_array) + num_of_words * sizeof(word_t);
}

bit_array *bit_array_create(word_addr_t num_of_words)
{
	size_t size = bit_array_bsize(num_of_words);

	bit_array *bitarr = (bit_array*)calloc(1, size);
	if (bitarr == NULL) {
		return NULL;
	}

	bitarr->words = (void*)(bitarr + 1);
	bitarr->num_of_words = num_of_words;
	return bitarr;
}

void bit_array_free(bit_array *array)
{
	free(array);
}

void bit_array_add(bit_array* src, const bit_array* add)
{
	word_addr_t max_words = src->num_of_words;

	char carry = 0;

	word_addr_t i;
	word_t word1, word2;

	for(i = 0; i < max_words; i++) {
		word1 = src->words[i];
		word2 = add->words[i];

		src->words[i] = word1 + word2 + carry;
		carry = WORD_MAX - word1 < word2 || WORD_MAX - word1 - word2 < (word_t)carry;
	}
}

void bit_array_add_uint64(bit_array* bitarr, uint64_t value)
{
	if(value == 0) {
		return;
	}

	word_addr_t i;

	for(i = 0; i < bitarr->num_of_words; i++) {
		if(WORD_MAX - bitarr->words[i] < value) {
			bitarr->words[i] += value;
			value = 1;
		} else {
			// Carry is absorbed
			bitarr->words[i] += value;
			break;
		}
	}
}

int bit_array_cmp(const bit_array* left, const bit_array* right)
{
	assert(left->num_of_words == right->num_of_words);
	word_addr_t i;
	word_t word1, word2;
	word_addr_t num_of_words = left->num_of_words;

	for(i = num_of_words - 1;; i--)
	{
		word1 = left->words[i];
		word2 = right->words[i];
		if (word1 != word2)
			return (word1 > word2 ? 1 : -1);
		if (i == 0)
			break;
	}

	return 0;
}

void bit_array_set_all(bit_array* bitarr)
{
	memset(bitarr->words, 0xFF, bitarr->num_of_words * sizeof(word_t));
}

void bit_array_clear_all(bit_array* bitarr)
{
	memset(bitarr->words, 0, bitarr->num_of_words * sizeof(word_t));
}


bit_index_t bit_array_length(const bit_array* bit_arr)
{
	return bit_arr->num_of_words * WORD_SIZE;
}

bit_array* bit_array_clone(const bit_array* bitarr)
{
	bit_array* cpy = bit_array_create(bitarr->num_of_words);

	if(cpy == NULL)
	{
		return NULL;
	}

	memcpy(cpy->words, bitarr->words, bitarr->num_of_words * sizeof(word_t));

	return cpy;
}
