#pragma once
#include <stddef.h>

typedef struct Array Array;
typedef struct Value Value;

typedef struct Neuron{
	Array* weight;
	Value* bias;
	int nin;
} Neuron;

typedef struct Layer{
	int nin;
	int nout;
	Neuron** neurons;
} Layer;

typedef struct MLP{
	int* nouts;
	int nouts_size;
	Layer** layers;
} MLP;

Neuron* init_neuron(int nin);
Value* forward_neuron(Neuron* neuron, Array* X, size_t x_size);
Layer* init_layer(int nin, int nout);
Array* forward_layer(Layer* layer, Array* X, size_t x_size);
MLP* init_mlp(int nin, int* nouts, size_t nouts_size);
Array* forward_mlp(MLP* mlp, Array* X, size_t x_size);
Array* neuron_parameters(Neuron* neuron);
Array* layer_parameters(Layer* layer);
Array* mlp_parameters(MLP* mlp);
