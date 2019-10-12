#include "bit_array.h"

#include <stdlib.h>

bit_array *bit_array_create(word_addr_t num_of_words)
{
	bit_array *bitarr = (bit_array*)malloc(sizeof(bit_array));
	if (bitarr == NULL) {
		return NULL;
	}
	bitarr->num_of_words = num_of_words;
	bitarr->words = (word_t*)calloc(bitarr->num_of_words, sizeof(word_t));
	if (bitarr->words == NULL) {
		free(bitarr);
		return NULL;
	}
	return bitarr;
}

void bit_array_free(bit_array *array)
{
	free(array->words);
	free(array);
}
