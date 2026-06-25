#include<stdlib.h>
#include<stdio.h>
#include "ds_array.h"
#include "memory.h"
#include "value.h"

Array* init_array(){
	Array *array = malloc(sizeof(Array));
	array->size = 0;
	array->capacity = 0;
	array->elements = malloc(sizeof(Value) * array->capacity);
	return array;
}

void array_insert(Array* array, Value* element){
	if (array->size == array->capacity){
		array->capacity = GROW_CAPACITY(array->capacity);
		array->elements = GROW_ARRAY(Value*, array->elements, array->capacity);
	}

	array->elements[array->size++] = element;
}

Value* get_at_index(Array* array, int idx){
	if (array->size == 0){
		perror("No indexing on an empty array\n");
		exit(1);
	}

	else if (idx > array->size || idx < 0){
		perror("Incorrect indexing\n");
		exit(1);
	}

	return array->elements[idx];
}

void join_array(Array* a, Array* b){
	for (int i = 0; i < b->size; ++i)
		array_insert(a, get_at_index(b, i));
}

void display_array(Array* array){
	for (int i = 0; i < array->size; i++)
		printf("%.2lf ", get_data(array->elements[i]));

	printf("\n");
}

void free_array(Array* array){
	free(array->elements);
}
