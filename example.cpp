#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include "lib/math_utils.hpp"
#include "lib/config.hpp"
#include "lib/genome.hpp"
#include "lib/population.hpp"

int expected = 10;
std::vector<double> inputs = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

// Assuming Genome has a fitness member variable
void evaluate_genome(neat::Genome *g, int generation)
{
    std::vector<double> result = g->feed_forward(inputs);
    g->fitness = 1 / abs(expected - result[0]);
}

void callback_generation(neat::Population *p, int generation)
{
    printf("Generation: %d\n", generation);
    printf("Best fitness: %f\n", p->best_fitness);
}

int main(int argc, char *argv[])
{
    // Initialize NEAT configuration
    neat::Config config = neat::load_config("default_config.txt");

    // Initialize population
    neat::Population *p = new neat::Population(config);

    // Run NEAT algorithm
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
    neat::Genome *g = neat::Genome::load(save_file);
    g->print();

    delete p;
    delete g;

    return 0;
}
