#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "config.h"
#include "activation_functions.h"
#include "math_utils.h"
#include "connection_gene.h"
#include "node.h"

Node::Node(int id, ActivationFunctions activation_function, int layer) : id(id), input_sum(0), output_value(0), layer(layer), activation_function(activation_function) {}

void Node::activate()
{
    if (layer != 0)
    {
        ActivationFunctionPointer activation = get_function(activation_function);
        output_value = activation(input_sum);
    }
}

void Node::propagate_output()
{
    for (auto &c : output_connections)
    {
        if (c->enabled)
        {
            c->to_node->input_sum += c->weight * output_value;
        }
    }
}

void Node::mutate(const NeatConfig &config, bool is_bias_node)
{
    if (is_bias_node)
    {
        if (randrange() < config.bias_replace_rate)
        {
            output_value = uniform(config.bias_min_value, config.bias_max_value);
        }
        else if (randrange() < config.bias_mutate_rate)
        {
            output_value += normal(config.bias_init_mean, config.bias_init_stdev) / 50;
            output_value = std::min(std::max(output_value, config.bias_min_value), config.bias_max_value);
        }
    }

    if (randrange() < config.activation_mutate_rate)
    {
        std::vector<ActivationFunctions> activations_functions = {
            "step",
            "sigmoid",
            "tanh",
            "relu",
            "leaky_relu",
            "prelu",
            "elu",
            "softmax",
            "linear",
            "swish"};
        ActivationFunctions random_function = activations_functions[floor(randrange() * activations_functions.size())];
        activation_function = random_function;
    }
}

bool Node::is_connected_to(Node *node)
{
    if (node->layer == layer)
    {
        return false;
    }

    if (node->layer < layer)
    {
        for (auto &c : node->output_connections)
        {
            if (c->to_node == this)
            {
                return true;
            }
        }
    }
    else
    {
        for (auto &c : output_connections)
        {
            if (c->to_node == node)
            {
                return true;
            }
        }
    }

    return false;
}

Node *Node::clone()
{
    return new Node(id, activation_function, layer);
}

ActivationFunctionPointer Node::get_function(ActivationFunctions function)
{
    if (activation_function == "step")
    {
        return step;
    }
    else if (activation_function == "sigmoid")
    {
        return sigmoid;
    }
    else if (activation_function == "tanh")
    {
        return tanh;
    }
    else if (activation_function == "relu")
    {
        return relu;
    }
    else if (activation_function == "leaky_relu")
    {
        return leaky_relu;
    }
    else if (activation_function == "prelu")
    {
        return prelu;
    }
    else if (activation_function == "elu")
    {
        return elu;
    }
    else if (activation_function == "softmax")
    {
        return softmax;
    }
    else if (activation_function == "linear")
    {
        return linear;
    }
    else if (activation_function == "swish")
    {
        return swish;
    }
    else if (activation_function == "swish")
    {
        return swish;
    }
    else
    {
        return sigmoid;
    }
}
