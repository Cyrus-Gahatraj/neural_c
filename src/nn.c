#include "ds_array.h"
#include "nn.h"
#include "value.h" 
#include<stdlib.h>
#include<stdio.h>

double random_num(){
	return ((double) rand() / RAND_MAX) * 2.0 - 1.0;
}

Neuron* init_neuron(int nin){
	Neuron* neuron = malloc(sizeof(Neuron));
	neuron->nin = nin;
	neuron->weight = init_array(); 

	for(int i = 0; i < nin; ++i){
		double rn = random_num(); 
		array_insert(neuron->weight, init_value(rn));
	}

	neuron->bias = init_value(random_num()); 

	return neuron;
}

Value* forward_neuron(Neuron* neuron, Array* X, size_t x_size){
	if (x_size != neuron->nin){
		perror("Nin and size of X don't match\n");
		exit(1);
	}

	Value* out = init_value(0.0);
	for(int i = 0; i < x_size; ++i){
		Value* weight = get_at_index(neuron->weight, i);
		Value* prod = mul_value(weight, get_at_index(X, i)); 
		out = add_value(out, prod);
	}

	out = add_value(out, neuron->bias);
	out = tanh_value(out);
	return out;
}

Array* neuron_parameters(Neuron* neuron){
	Array* params = init_array();
	for (int i = 0; i < neuron->nin; ++i)
			array_insert(params, get_at_index(neuron->weight, i));
	array_insert(params, neuron->bias);

	return params;
}

Layer* init_layer(int nin, int nout){
	Layer* layer = malloc(sizeof(Layer));
	layer->neurons = malloc(sizeof(Neuron*) * nout);
	layer->nin = nin;
	layer->nout = nout;
	for (int i = 0; i < nout; ++i)
		layer->neurons[i] = init_neuron(nin);

	return layer;
}

Array* forward_layer(Layer* layer, Array* X, size_t x_size){
	Array* values = init_array();
	for (int i = 0; i < layer->nout; ++i){
		array_insert(values, forward_neuron(layer->neurons[i], X, x_size));
	}

	return values;
}

Array* layer_parameters(Layer* layer){
	Array* params = init_array();
	for (int i = 0; i < layer->nout; ++i){
		join_array(params, neuron_parameters(layer->neurons[i]));
	}

	return params;
}

MLP* init_mlp(int nin, int* nouts, size_t nouts_size){
	MLP* mlp = malloc(sizeof(MLP));
	mlp->nouts = malloc(sizeof(int) * nouts_size);
	mlp->nouts_size = nouts_size;
	mlp->layers = malloc(sizeof(Layer*) * nouts_size);

	int in = nin;
	for (int i = 0; i < nouts_size; ++i){
		mlp->nouts[i] = nouts[i];
		mlp->layers[i] = init_layer(in, nouts[i]);
		in = nouts[i];
	}

	return mlp;
}

Array* forward_mlp(MLP* mlp, Array* X, size_t x_size){
	Array* values = forward_layer(mlp->layers[0], X, x_size);

	for (int i = 1; i < mlp->nouts_size; ++i){
		int prev_value = mlp->layers[i - 1]->nout;
		Array* current_layer = init_array();

		for (int j = 0; j < prev_value; ++j)
			array_insert(current_layer, get_at_index(values, j));
		
		values = forward_layer(mlp->layers[i], current_layer, prev_value);
	}
	
	return values;
}

Array* mlp_parameters(MLP* mlp){
	Array* params = init_array();
	for (int i = 0; i < mlp->nouts_size; ++i){
		join_array(params, layer_parameters(mlp->layers[i]));
	}

	return params;
}
