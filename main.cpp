#include <iostream>
#include <cstdio> // For printf function
#include <vector>
#include <cmath>
#include "src/math_utils.h"
#include "src/config.h"
#include "src/genome.h"
#include "src/population.h"

int expected = 100;
std::vector<double> inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

// Assuming Genome has a fitness member variable
void evaluate_genome(Genome *g, int generation)
{
    std::vector<double> result = g->feed_forward(inputs);
    g->fitness = 1 / abs(expected - result[0]);
}

void callback_generation(int generation)
{
}

int main(int argc, char *argv[])
{
    // Initialize NEAT configuration
    NeatConfig config = load_config_from_file("default_config.txt");

    // Initialize population
    Population *p = new Population(config);

    // // Run NEAT algorithm
    int generations = 1000;
    p->run(&evaluate_genome, generations, &callback_generation);

    // Display the results of the best genome
    std::vector<double> result = p->best_genome->feed_forward(inputs);
    if (result[0] - expected >= 0)
        printf("Solution found: %f\n", expected - 1 / p->best_genome->fitness);
    else
        printf("Solution found: %f\n", expected + 1 / p->best_genome->fitness);
    p->best_genome->print_genome();

    delete p;

    return 0;
}
