#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <random>
#include "../config.h"
#include "../population.h"

class PopulationTest : public testing::Test
{
protected:
    NeatConfig config;

    void SetUp() override
    {
        // Mock NeatConfig
        config = load_config_from_file("default_config.txt");
        config.num_inputs = 5;
        config.num_outputs = 2;
        config.population_size = 10;
        config.species_elitism = 2;
        config.max_stagnation = 5;
        config.bad_species_threshold = 0.5;
        config.no_fitness_termination = false;
        config.min_species_size = 2;
        config.fitness_threshold = 100;
    }
};

TEST_F(PopulationTest, PopulationInitialization)
{
    Population *population = new Population(config);
    ASSERT_EQ(population->genomes.size(), config.population_size);
    ASSERT_EQ(population->species.size(), 0);
    ASSERT_EQ(population->generation, 0);
}

TEST_F(PopulationTest, SetBestGenome)
{
    Population *population = new Population(config);

    // Assume best fitness is set to 10 for simplicity
    population->best_fitness = 10;

    // Mock species and genomes
    Genome *genome = new Genome(config);
    genome->fitness = 20;
    Species *species = new Species(genome);
    species->genomes = {genome};
    population->species = {species};

    // Set best genome
    population->set_best_genome();

    // Assert that the best genome is set
    ASSERT_EQ(population->best_genome, genome);
}

TEST_F(PopulationTest, Speciate)
{
    Population *population = new Population(config);

    // Mock genomes
    Genome *genome1 = new Genome(config);
    Genome *genome2 = new Genome(config);
    population->genomes = {genome1, genome2};

    // Mock species
    Species *species1 = new Species(genome1);
    Species *species2 = new Species(genome2);
    population->species = {species1, species2};

    // Run speciation
    population->speciate();

    // Assert that genomes are grouped into species
    ASSERT_GT(population->species.size(), 0);
}

TEST_F(PopulationTest, ReproduceSpecies)
{
    Population *population = new Population(config);

    // Mock species
    Genome *genome = new Genome(config);
    Species *species = new Species(genome);
    population->species = {species};

    // Run reproduction
    population->reproduce_species();

    // Assert that the population's genomes are updated
    ASSERT_EQ(population->generation, 1);
    ASSERT_EQ(population->best_genome, genome);
    ASSERT_EQ(population->genomes.size(), config.population_size);
}

TEST_F(PopulationTest, SortSpecies)
{
    Population *population = new Population(config);

    // Mock species
    for (int i = 0; i < 5; ++i)
    {
        Genome *genome = new Genome(config);
        genome->fitness = random() % 100;
        Species *species = new Species(genome);
        population->species.push_back(species);
    }

    // Run species sorting
    population->sort_species();

    // Assert species are sorted
    const auto &speciesList = population->species;
    ASSERT_EQ(population->species.size(), 5);

    auto getBestFitness = [](const Species *s)
    { return s->best_fitness; };
    ASSERT_TRUE(std::is_sorted(speciesList.begin(), speciesList.end(), [&getBestFitness](const Species *s1, const Species *s2)
                               { return getBestFitness(s1) > getBestFitness(s2); }));
}

TEST_F(PopulationTest, KillStagnantSpecies)
{
    Population *population = new Population(config);

    // Mock species
    Species *speciesToKeep1 = new Species(new Genome(config));
    speciesToKeep1->stagnation = 2;

    Species *speciesToKeep2 = new Species(new Genome(config));
    speciesToKeep2->stagnation = 4;

    Species *speciesToRemove1 = new Species(new Genome(config));
    speciesToRemove1->stagnation = 6;

    Species *speciesToRemove2 = new Species(new Genome(config));
    speciesToRemove2->stagnation = 8;

    population->species.push_back(speciesToKeep1);
    population->species.push_back(speciesToKeep2);
    population->species.push_back(speciesToRemove1);
    population->species.push_back(speciesToRemove2);

    // Run killing stagnant species
    population->kill_stagnant_species();

    // Assert stagnant species are removed
    const auto &remainingSpecies = population->species;
    ASSERT_EQ(remainingSpecies.size(), 2);
    ASSERT_EQ(remainingSpecies[0], speciesToKeep1);
    ASSERT_EQ(remainingSpecies[1], speciesToKeep2);

    // Cleanup
    delete speciesToKeep1;
    delete speciesToKeep2;
    delete speciesToRemove1;
    delete speciesToRemove2;
}

TEST_F(PopulationTest, KillBadSpecies)
{
    Population *population = new Population(config);

    // Mock species
    Species *goodSpecies = new Species(new Genome(config));
    goodSpecies->average_fitness = config.bad_species_threshold + 1;

    Species *badSpecies1 = new Species(new Genome(config));
    badSpecies1->average_fitness = config.bad_species_threshold - 1;

    Species *badSpecies2 = new Species(new Genome(config));
    badSpecies2->average_fitness = config.bad_species_threshold - 2;

    population->species.push_back(goodSpecies);
    population->species.push_back(badSpecies1);
    population->species.push_back(badSpecies2);

    // Run killing bad species
    population->kill_bad_species();

    // Assert bad species are removed
    const auto &remainingSpecies = population->species;
    ASSERT_EQ(remainingSpecies.size(), 1);
    ASSERT_EQ(remainingSpecies[0], goodSpecies);

    // Cleanup
    delete goodSpecies;
    delete badSpecies1;
    delete badSpecies2;
}

TEST_F(PopulationTest, UpdateSpecies)
{
    Population *population = new Population(config);

    // Mock species
    Species *species = new Species(new Genome(config));
    for (int i = 0; i < 10; ++i)
        species->add_to_species(new Genome(config));
    population->species.push_back(species);

    // Run updating species
    population->update_species();

    // Assert species is updated
    const auto &updatedSpecies = population->species;
    ASSERT_EQ(updatedSpecies.size(), 1);

    const auto &updatedGenomes = updatedSpecies[0]->genomes;
    ASSERT_EQ(updatedGenomes.size(), config.min_species_size);
}

TEST_F(PopulationTest, GetGenomeById)
{
    Population *population = new Population(config);

    // Mock genomes
    Genome *genome1 = new Genome(config);
    Genome *genome2 = new Genome(config);
    genome1->id = "1";
    genome2->id = "2";
    population->genomes.push_back(genome1);
    population->genomes.push_back(genome2);

    // Run getting genome by id
    auto resultGenome = population->get_genome("2");

    // Assert correct genome is returned
    ASSERT_EQ(resultGenome, genome2);
}
