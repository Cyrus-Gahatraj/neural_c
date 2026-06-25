#pragma once
#include<stdbool.h>
typedef struct Value Value;

typedef struct Set {
	Value **elements;
	int size;
	int capacity;
} Set;

Set* init_set();
int is_empty_set(Set *set);
void insert_set(Set* set, Value* element);
Set* union_set(Set* a, Set* b);
void display_set(Set* set);
void free_set(Set* set);
bool is_element_in_set(Set *set, Value* element);
