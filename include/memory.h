#pragma once
#include<stdlib.h>

#define GROW_CAPACITY(capacity) \
	((capacity < 10) ? 10 : capacity * 2)

#define GROW_ARRAY(type, pointer, new_size) \
	(type*) reallocate(pointer, sizeof(type) * new_size ) 

void* reallocate(void* pointer, size_t new_size);

