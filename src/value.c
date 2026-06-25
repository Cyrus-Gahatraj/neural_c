#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "value.h"
#include "ds_set.h"
#include "ds_array.h"

Value* init_value(double data){
	Value* value = malloc(sizeof(Value));
	value->data = data;
	value->_prev = init_set();
	value->grad = 0.0;
	value->op = '\0';

	return value;
}

double get_data(Value *value){
	return value->data;
}

void display_value(Value *value){
	printf("Value: %lf\n", value->data);
}
void backward_add(Value* out){
	Value* a = out->_prev->elements[0];
	Value* b = out->_prev->elements[1];
	a->grad += 1.0 * out->grad;
	b->grad += 1.0 * out->grad;
}

Value* add_value(Value *a, Value *b){
	Set* set = init_set();	
	insert_set(set, a);
	insert_set(set, b);

	Value* out = malloc(sizeof(Value));
	out->data = a->data + b->data;
	out->_prev = set;
	out->op = '+';

	return out;
}

void backward_sub(Value* out){
	Value* a = out->_prev->elements[0];
	Value* b = out->_prev->elements[1];
	a->grad += 1.0 * out->grad;
	b->grad -= 1.0 * out->grad;
}

Value* sub_value(Value *a, Value *b){
	Set* set = init_set();	
	insert_set(set, a);
	insert_set(set, b);

	Value* out = malloc(sizeof(Value));
	out->data = a->data - b->data;
	out->_prev = set;
	out->op = '-';

	return out;
}

void backward_mul(Value* out){
	Value* a = out->_prev->elements[0];
	Value* b = out->_prev->elements[1];
	a->grad += b->data * out->grad;
	b->grad += a->data * out->grad;
}

Value* mul_value(Value *a, Value *b){
	Set* set = init_set();	
	insert_set(set, a);
	insert_set(set, b);

	Value* out = malloc(sizeof(Value));
	out->data = a->data * b->data;
	out->_prev = set;
	out->op = '*';

	return out;
}

void backward_pow(Value* out){
	Value* a = out->_prev->elements[0];
	a->grad += out->param * pow(a->data, out->param - 1) * out->grad;
}

Value* pow_value(Value *a, double power){
	Set* set = init_set();	
	insert_set(set, a);

	Value* out = malloc(sizeof(Value));
	out->data = pow(a->data, power);
	out->param = power;
	out->_prev = set;
	out->op = '^';

	return out;
}

void backward_tanh(Value* out){
	Value* a = out->_prev->elements[0];
	a->grad += (1 - out->data * out->data) * out->grad;
}

Value* tanh_value(Value *a){
	Set* set = init_set();	
	insert_set(set, a);

	double data = a->data;
	double tanh = (exp(2*data) - 1)/(exp(2*data) + 1);
	Value* out = malloc(sizeof(Value));
	out->data = tanh;
	out->_prev = set;
	out->op = 'T';

	return out;
}

void topo_sort(Array* topo, Set* visited, Value* value){
	if (!is_element_in_set(visited, value)){
		insert_set(visited, value);
		for (int i = 0; i < value->_prev->size; ++i)
			topo_sort(topo, visited, value->_prev->elements[i]);		
		array_insert(topo, value);
	}
}

Array* build_topo(Value* value){
	Array* topo = init_array();
	Set* visited = init_set();
	topo_sort(topo, visited, value);

	return topo;
}

void _backward(Value* out) {
	if (out == NULL || out->op == '\0')
		return;

	switch (out->op){
		case '+': backward_add(out); break;
		case '-': backward_sub(out); break;
		case '*': backward_mul(out); break;
		case '^': backward_pow(out); break;
		case 'T': backward_tanh(out); break;
		default:
			perror("No such operation");
			exit(1);
	}
}

void backward(Value* out) {
	Array* topo = build_topo(out);

	out->grad = 1.0;
	for (int i = topo->size - 1; i >= 0; --i)
		_backward(topo->elements[i]);
}

void free_value(Value* value){
	free_set(value->_prev);
}

int is_equal_value(Value *a, Value *b){
	return (a == b);
}
