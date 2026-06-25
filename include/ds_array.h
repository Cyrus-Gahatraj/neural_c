#pragma once
#include<stdbool.h>

typedef struct Value Value;

typedef struct Array {
	Value **elements;
	int size;
	int capacity;
} Array;

Array* init_array();
void array_insert(Array* array, Value* element);
void display_array(Array* array);
Value* get_at_index(Array* array, int idx);
void free_array(Array* array);
void join_array(Array* a, Array* b);

