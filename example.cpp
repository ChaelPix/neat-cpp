#include <iostream>
#include <cstdio> // For printf function
#include <vector>
#include <cmath>
#include "lib/math_utils.h"
#include "lib/config.h"
#include "lib/genome.h"
#include "lib/population.h"

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
    int generations = 1000;
    p->run(&evaluate_genome, generations, &callback_generation);

    p->best_genome->print_genome();

    // Display the results of the best genome
    std::vector<double> result = p->best_genome->feed_forward(inputs);
    if (result[0] - expected >= 0)
        printf("Solution found: %f\n", expected - 1 / p->best_genome->fitness);
    else
        printf("Solution found: %f\n", expected + 1 / p->best_genome->fitness);

    // Save this genome
    std::string id = p->best_genome->id;
    p->best_genome->save("./cache/genomes/genome_" + id + ".pkl");

    delete p;

    return 0;
}
