#include<stdlib.h>
#include<stdio.h>
#include "ds_set.h"
#include "value.h"
#include "memory.h"

Set* init_set(){
	Set *set = malloc(sizeof(Set));
	set->size = 0;
	set->capacity = 0;
	set->elements = malloc(sizeof(Value) * set->capacity);
	return set;
}

int is_empty_set(Set *set){
	return (set->size == 0);
}

bool is_element_in_set(Set *set, Value* element){
	for (int i = 0; i < set->size; ++i)
		if (is_equal_value(set->elements[i], element))
			return true;
	return false;
}

void insert_set(Set* set, Value* element){
	for (int i = 0; i < set->size; ++i)
		if (is_equal_value(set->elements[i], element))
			return;

	if (set->size == set->capacity){
		set->capacity = GROW_CAPACITY(set->capacity);
		set->elements = GROW_ARRAY(Value*, set->elements, set->capacity);
	}

	set->elements[set->size++] = element;
}

Set* union_set(Set* a, Set* b){
	Set* c = init_set();

	for (int i = 0; i < a->size; ++i)
		insert_set(c, a->elements[i]);

	for (int i = 0; i < b->size; ++i)
		insert_set(c, b->elements[i]);

	return c;
}

void display_set(Set* set){
	for (int i = 0; i < set->size; i++)
		printf("%.2lf ", get_data(set->elements[i]));

	printf("\n");
}

void free_set(Set* set){
	free(set->elements);
	free(set);
}
