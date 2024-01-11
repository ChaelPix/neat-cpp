#include <iostream>
#include <cstdlib> // For rand function
#include <cstdio>  // For printf function
#include <vector>
#include <cmath>
#include "src/math_utils.h"
#include "src/config.h"
#include "src/genome.h"
#include "src/population.h"

// Define XOR inputs and expected outputs
int expected = 100;
std::vector<double> inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// Assuming Genome has a fitness member variable
void evaluate_genome(Genome *g, int generation)
{
    std::vector<double> result = g->feed_forward(inputs);
    g->fitness = abs(expected - result[0]);
}

void callback_generation(int generation)
{
    printf("Generation: %d\n", generation);
}

int main(int argc, char *argv[])
{
    // Initialize NEAT configuration
    NeatConfig config = load_config_from_file("default_config.txt");

    // Initialize population
    Population *p = new Population(config);

    // for (auto& g: p->genomes) {
    //     g->print_genome();
    // }

    // // Run NEAT algorithm
    int generations = 100;
    p->run(&evaluate_genome, generations, callback_generation);
    // printf("Best genome : %f\n", p->best_genome->fitness);

    // delete p;

    return 0;
}
