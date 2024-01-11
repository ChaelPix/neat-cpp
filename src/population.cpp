#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <thread>
#include "config.h"
#include "genome.h"
#include "connection_history.h"
#include "species.h"
#include "population.h"

Population::Population(const NeatConfig &config) : config(config)
{
    for (int i = 0; i < config.population_size; ++i)
    {
        Genome *genome = new Genome(config);
        // genome->mutate(innovation_history);
        genome->generate_network();

        if (config.initial_connections == "full")
        {
            genome->fully_connect(innovation_history);
        }
        genomes.push_back(genome);
    }
}

void Population::set_best_genome()
{
    Genome *temp_best = species[0]->genomes[0];
    if (temp_best->fitness >= best_fitness)
    {
        best_genome = temp_best;
        best_fitness = temp_best->fitness;
    }
}

void Population::run(std::function<void(Genome *, int)> evaluate_genome, int nb_generations, std::function<void(int)> callback_generation)
{
    for (int i = 0; i < nb_generations; ++i)
    {
        // Use threads to evaluate genomes in parallel
        std::vector<std::thread> threads;
        for (size_t j = 0; j < genomes.size(); ++j)
        {
            threads.emplace_back([&, j]()
                                 { evaluate_genome(genomes[j], i); });
        }

        // Wait for all threads to finish
        for (auto &thread : threads)
        {
            thread.join();
        }

        speciate();
        sort_species();
        update_species();
        set_best_genome();
        set_average_fitness();
        kill_stagnant_species();
        kill_bad_species();
        reproduce_species();
        reset_on_extinction();
        printf("Best genome : %f\n", best_genome->fitness);

        if (callback_generation)
        {
            callback_generation(i);
        }

        if (!config.no_fitness_termination && best_genome->fitness > config.fitness_threshold)
        {
            break;
        }
    }
}

void Population::speciate()
{
    // Reset the genomes in each species
    for (auto &s : species)
    {
        s->genomes.clear();
    }

    // Group the genomes by species
    for (auto &g : genomes)
    {
        bool species_found = false;
        for (auto &s : species)
        {
            if (s->same_species(g, config))
            {
                s->add_to_species(g);
                species_found = true;
                break;
            }
        }
        if (!species_found)
        {
            Species *new_species = new Species(g);
            species.push_back(new_species);
        }
    }
}

void Population::reproduce_species()
{
    float average_fitness_sum = get_average_fitness_sum();
    int population_size = config.population_size;

    std::vector<Genome *> children;
    for (auto &s : species)
    {
        children.push_back(s->champion->clone());
        // get the calculated amount of children from this species
        int nb_of_children = (average_fitness_sum == 0) ? 0 : std::floor((s->average_fitness / average_fitness_sum) * population_size) - 1;
        for (int i = 0; i < nb_of_children; ++i)
        {
            children.push_back(s->give_me_baby(innovation_history));
        }
    }

    Genome *previous_best = genomes[0];
    if (children.size() < static_cast<size_t>(population_size))
    {
        children.push_back(previous_best->clone());
    }

    // if not enough babies (due to flooring the number of children to get a whole var) get babies from the best species
    while (children.size() < static_cast<size_t>(population_size))
    {
        children.push_back(species[0]->give_me_baby(innovation_history));
    }

    genomes = children;
    ++generation;
    for (auto &g : genomes)
    {
        g->generate_network();
    }
    set_best_genome();

    // Check the number of genomes for each species
    species.erase(std::remove_if(species.begin(), species.end(), [this](const Species *s)
                                 { return s->genomes.size() < config.population_size; }),
                  species.end());
}

void Population::sort_species()
{
    for (auto &s : species)
    {
        s->sort_genomes();
    }

    std::sort(species.begin(), species.end(), [](const Species *s1, const Species *s2)
              { return s1->best_fitness > s2->best_fitness; });
}

void Population::kill_stagnant_species()
{
    // Use remove_if along with a lambda function to filter out stagnant species
    auto it = std::remove_if(config.species_elitism < species.size() ? species.begin() + config.species_elitism : species.begin(), species.end(), [this](const Species *s)
                             { return s->stagnation >= config.max_stagnation; });

    // Erase the removed elements from the vector
    species.erase(it, species.end());
}

void Population::kill_bad_species()
{
    float species_average_fitness = get_average_fitness_sum() / species.size();
    auto it = std::remove_if(species.size() > 1 ? species.begin() + 1 : species.begin(), species.end(), [this, species_average_fitness](const Species *s)
                             { return s->average_fitness < species_average_fitness * config.bad_species_threshold; });
    species.erase(it, species.end());
}

void Population::reset_on_extinction()
{
    if (species.empty())
    {
        genomes.clear();

        // Reinitialize with new genomes
        for (int i = 0; i < config.population_size; ++i)
        {
            genomes.push_back(new Genome(config));
        }
    }
}

double Population::get_average_fitness_sum()
{
    float average_sum = 0;
    for (const auto &s : species)
    {
        average_sum += s->average_fitness;
    }
    return average_sum;
}

void Population::set_average_fitness()
{
    average_fitness = get_average_fitness_sum() / species.size();
}

void Population::update_species()
{
    for (auto &s : species)
    {
        s->kill_genomes(config);
        s->fitness_sharing();
        s->set_average_fitness();
    }
}

Genome *Population::get_genome(const std::string &id)
{
    auto it = std::find_if(genomes.begin(), genomes.end(), [&id](const Genome *g)
                           { return g->id == id; });
    if (it != genomes.end())
    {
        return *it;
    }
    // Return a default-constructed Genome if not found (you may want to handle this differently based on your design)
    return new Genome(config);
}
