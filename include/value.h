#pragma once
typedef struct Set Set;
typedef struct Array Array;

typedef struct Value{
	double data;
	Set* _prev;
	double grad;
	char op;
	double param;
} Value;

Value* init_value(double data);
double get_data(Value* value);
void display_value(Value* value);
Value* add_value(Value* a, Value* b);
Value* sub_value(Value *a, Value *b);
Value* mul_value(Value* a, Value* b);
Value* pow_value(Value *a, double power);
Value* tanh_value(Value* a);
void backward(Value* value);
Array* build_topo(Value* value);
void free_value(Value* value);
int is_equal_value(Value* a, Value* b);
