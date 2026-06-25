#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include "ds_array.h"
#include "value.h"
#include "nn.h"

#define NIN 3
#define OUT 1

typedef struct Loss{
	Array* ypred;
	double* yactual;
	size_t ysize;
	Value* value;
} Loss;

typedef struct XData {
	size_t row;
	Array** data;
} XData;

Value* get_loss(Array* ypred, double* y, size_t y_size){
	Value* loss = init_value(0.0);
	for (int i = 0; i < y_size; ++i){
		Value* actual = init_value(y[i]);
		Value* predicted = get_at_index(ypred, i);

		Value* diff = sub_value(predicted, actual);
		Value* sq = pow_value(diff, 2);	
		loss = add_value(loss, sq);
	}

	return loss;
}

Loss* init_loss(Array* ypred, double* ys, size_t ys_size){
	Loss* loss = malloc(sizeof(Loss));
	loss->ypred = ypred;
	loss->yactual = ys;
	loss->ysize = ys_size;
	loss->value = get_loss(ypred, ys, ys_size);

	return loss;
}

void gradient_descent(XData* xdata, MLP* mlp, Loss* loss, int epochs){
	double lr = 0.01;
	Array* ypred;
	
	for (int e = 0; e < epochs; ++e){
		ypred = init_array();
		for (int i = 0; i < xdata->row; ++i){
			Array* ioutput = forward_mlp(mlp, xdata->data[i], NIN);	
			for (int j = 0; j < OUT; ++j){
				array_insert(ypred, get_at_index(ioutput, j));
			}
		}	

		Array* params = mlp_parameters(mlp);
		for (int i = 0; i < params->size; ++i){
			get_at_index(params, i)->grad = 0.0;
		}

		loss->value = get_loss(ypred, loss->yactual, loss->ysize);
		backward(loss->value);

		for (int i = 0; i < params->size; ++i){
			Value* v = get_at_index(params, i);
			v->data += -lr * v->grad;
		}

		// printf("%d. %f\n", (e+1), loss->value->data);
	}

	loss->ypred = ypred;
}

int main(){
	srand(time(NULL));
	
	double raw_xs[][NIN] = {
		{ 2.0, 3.0, -1.0 },
		{ 3.0, -1.0, 0.5 },
		{ 0.5, 1.0, 1.0 },
		{ 1.0, 1.0, -1.0 },
	};

	double ys[] = { 1.0, -1.0, -1.0, 1.0 };
	size_t ys_size = sizeof(ys)/sizeof(ys[0]);
	printf("Target: ");
	for (int i = 0; i < ys_size; ++i)
		printf("%lf ", ys[i]);
	printf("\n");

	size_t xs_row = sizeof(raw_xs)/sizeof(raw_xs[0]);
	Array** xs = malloc(sizeof(Array*) * xs_row);
	for (int i = 0; i < xs_row; ++i){
		xs[i] = init_array();
		for (int j = 0; j < NIN; ++j)
			array_insert(xs[i], init_value(raw_xs[i][j]));
	}

	XData* x_data = malloc(sizeof(XData));
	x_data->data = xs;
	x_data->row = xs_row;
	
	int nouts[] = { 4, 4, OUT };
	size_t nouts_size = sizeof(nouts)/sizeof(nouts[0]);
	MLP* mlp = init_mlp(NIN, nouts, nouts_size);
	
	Array* ypred = init_array();
	for (int i = 0; i < xs_row; ++i){
		Array* ioutput = forward_mlp(mlp, xs[i], NIN);	
		for (int j = 0; j < OUT; ++j)
			array_insert(ypred, get_at_index(ioutput, j));
	}	


	Loss* loss = init_loss(ypred, ys, ys_size);

	printf("Before: ");
	display_array(loss->ypred);

	printf("Applying gradient descent...\n");
	gradient_descent(x_data, mlp, loss, 100);

	printf("After: ");
	display_array(loss->ypred);

	return 0;
}

