# 🧠 NEAT Algorithm Implementation

This project implements the NEAT (NeuroEvolution of Augmenting Topologies) algorithm for evolving neural networks. NEAT is a genetic algorithm specifically designed for evolving neural networks with varying topologies.

## Overview

- **NEAT Algorithm**: The NEAT algorithm is a method for evolving artificial neural networks. It allows the evolution of both the structure and weights of neural networks, starting from minimal structures and incrementally adding complexity over generations.

## Quick Start

```cpp
#include <iostream>
#include <cstdio> // For printf function
#include <vector>
#include <cmath>
#include "src/math_utils.h"
#include "src/config.h"
#include "src/genome.h"
#include "src/population.h"

int expected = 10;
std::vector<double> inputs = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

// Assuming Genome has a fitness member variable
void evaluate_genome(Genome *g, int generation)
{
    std::vector<double> result = g->feed_forward(inputs);
    g->fitness = 1 / abs(expected - result[0]);
}

void callback_generation(Population *population, int generation)
{
    printf("Generation: %d\n", generation);
    printf("Best fitness: %f\n", population->best_fitness);
}

int main(int argc, char *argv[])
{
    // Initialize NEAT configuration
    NeatConfig config = load_config_from_file("default_config.txt");

    // Initialize population
    Population *p = new Population(config);

    // // Run NEAT algorithm
    int generations = 100;
    p->run(&evaluate_genome, generations, &callback_generation);

    p->best_genome->print();

    // Display the results of the best genome
    std::vector<double> result = p->best_genome->feed_forward(inputs);
    if (result[0] - expected >= 0)
        printf("Solution found: %f\n", expected - 1 / p->best_genome->fitness);
    else
        printf("Solution found: %f\n", expected + 1 / p->best_genome->fitness);

    // Save this genome
    std::string save_file = "./cache/genomes/genome_" + p->best_genome->id + ".json";
    p->best_genome->save(save_file);

    // Load the genome
    Genome *g = Genome::load(save_file);
    g->print();

    delete p;
    delete g;

    return 0;
}

```

## Customize NEAT Configuration

Adjust the NEAT configuration parameters in a text file config.txt file to suit your specific problem and preferences.

Now you're all set to evolve neural networks using the NEAT algorithm! Feel free to explore and experiment with different configurations.

The default_config.py file contains configuration parameters for the NEAT algorithm. Adjust these parameters to customize the algorithm's behavior.

- **Population Settings**

  - `population_size`: Number of individuals in each generation.
  - `fitness_threshold`: Termination threshold for the fitness criterion.
  - `no_fitness_termination`: If True, the evolution process won't terminate based on fitness.
  - `reset_on_extinction`: If True, a new random population is created when all species go extinct.

- **Genome Settings**

  - `num_inputs`: Number of input nodes in the neural network.
  - `num_outputs`: Number of output nodes in the neural network.

- **Connection Settings**

  - `compatibility_disjoint_coefficient`: Coefficient for compatibility calculation related to disjoint genes.
  - `compatibility_weight_coefficient`: Coefficient for compatibility calculation related to weight differences.
  - `conn_add_prob`: Probability of adding a new connection.
  - `conn_delete_prob`: Probability of deleting an existing connection.
  - `enabled_default`: Default state (enabled or disabled) for new connections.
  - `enabled_mutate_rate`: Mutation rate for connection enable/disable state.
  - `initial_connections`: Type of initial connections ("none" or "full").
  - `weight_init_mean`: Mean for the initialization of weight values.
  - `weight_init_stdev`: Standard deviation for the initialization of weight values.
  - `weight_init_type`: Initialization type for weight values ("normal" or "uniform").
  - `weight_max_value`: Maximum value for weight values.
  - `weight_min_value`: Minimum value for weight values.
  - `weight_mutate_rate`: Mutation rate for weight values.
  - `weight_replace_rate`: Replacement rate for weight values.

- **Node Settings**

  - `node_add_prob`: Probability of adding a new node.
  - `node_delete_prob`: Probability of deleting an existing node.
  - `activation_default`: Default activation function for neurons.
  - `activation_mutate_rate`: Mutation rate for activation functions.

- **Stagnation Settings**

  - `max_stagnation`: Maximum number of generations without improvement before species are removed.
  - `species_elitism`: Number of species protected from stagnation.

- **Reproduction Settings**

  - `elitism`: Number of most-fit individuals preserved from one generation to the next.
  - `survival_threshold`: Fraction of each species allowed to reproduce.
  - `min_species_size`: Minimum number of genomes per species after reproduction.

- **Species Settings**
  - `compatibility_threshold`: Genomic distance threshold for considering individuals in the same species.
  - `bad_species_threshold`: Threshold for average fitness, below which a species is considered bad.

## Contributing

Contributions are welcome! If you find issues or have improvements, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

Original NEAT Paper: Stanley, K. O., & Miikkulainen, R. (2009). Evolving Neural Networks through Augmenting Topologies.

Inspired by the NEAT algorithm developed by Kenneth O. Stanley and Risto Miikkulainen.

Happy evolving! 🚀
