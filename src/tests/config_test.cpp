#include <gtest/gtest.h>
#include <fstream>
#include "../config.h"

class ConfigTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // You can create temporary files for testing if needed
        // or use existing ones in your project.
        temp_file_name = "temp_config_file.txt";
    }

    void TearDown() override
    {
        // Remove any temporary files or cleanup after the test if needed
        remove(temp_file_name.c_str());
    }

    std::string temp_file_name;
};

// Helper function to create a temporary config file with the given content
void createTempConfigFile(const std::string &content)
{
    std::ofstream temp_file;
    temp_file.open("temp_config_file.txt");
    temp_file << content;
    temp_file.close();
}

// Test cases for load_config_from_file function
TEST_F(ConfigTest, ValidConfigFile)
{
    // Create a temporary config file with valid content
    std::string valid_config_content = R"(
        # NEAT Configuration File

        # NEAT
        population_size = 50
        fitness_threshold = 100
        no_fitness_termination = true
        reset_on_extinction = true

        # GENOME
        activation_default = sigmoid
        activation_mutate_rate = 0.1
        num_inputs = 10
        num_outputs = 2
        bias_init_mean = 0.0
        bias_init_type = normal
        bias_init_stdev = 1.0
        bias_max_value = 1.0
        bias_min_value = -1.0
        bias_mutate_rate = 0.5
        bias_replace_rate = 0.1
        compatibility_disjoint_coefficient = 1.0
        compatibility_weight_coefficient = 0.5
        conn_add_prob = 0.1
        conn_delete_prob = 0.0
        enabled_default = true
        enabled_mutate_rate = 0.1
        initial_connections = full
        node_add_prob = 0.1
        node_delete_prob = 0.0
        weight_init_mean = 0.0
        weight_init_stdev = 1.0
        weight_init_type = normal
        weight_max_value = 1.0
        weight_min_value = -1.0
        weight_mutate_rate = 0.9
        weight_replace_rate = 0.1

        # STAGNATION
        max_stagnation = 15
        species_elitism = 2

        # REPRODUCTION
        elitism = 2
        survival_threshold = 0.2
        min_species_size = 2

        # SPECIES
        compatibility_threshold = 3.0
        bad_species_threshold = 0.25
    )";
    createTempConfigFile(valid_config_content);

    // Load config from the temporary file
    NeatConfig config = load_config_from_file(temp_file_name);

    // Assert specific values from the loaded config
    EXPECT_EQ(config.population_size, 50);
    EXPECT_DOUBLE_EQ(config.fitness_threshold, 100);
    EXPECT_FALSE(config.no_fitness_termination);
    EXPECT_TRUE(config.reset_on_extinction);
    EXPECT_EQ(config.activation_default, "sigmoid");
    EXPECT_EQ(config.activation_mutate_rate, 0.1);
    EXPECT_EQ(config.num_inputs, 10);
    EXPECT_EQ(config.num_outputs, 2);
    EXPECT_EQ(config.bias_init_mean, 0.0);
    EXPECT_EQ(config.bias_init_type, "normal");
    EXPECT_EQ(config.bias_init_stdev, 1.0);
    EXPECT_EQ(config.bias_max_value, 1.0);
    EXPECT_EQ(config.bias_min_value, -1.0);
    EXPECT_EQ(config.bias_mutate_rate, 0.5);
    EXPECT_EQ(config.bias_replace_rate, 0.1);
    EXPECT_EQ(config.compatibility_disjoint_coefficient, 1.0);
    EXPECT_EQ(config.compatibility_weight_coefficient, 0.5);
    EXPECT_EQ(config.conn_add_prob, 0.1);
    EXPECT_EQ(config.conn_delete_prob, 0.0);
    EXPECT_EQ(config.enabled_default, true);
    EXPECT_EQ(config.enabled_mutate_rate, 0.1);
    EXPECT_EQ(config.initial_connections, "full");
    EXPECT_EQ(config.node_add_prob, 0.1);
    EXPECT_EQ(config.node_delete_prob, 0.0);
    EXPECT_EQ(config.weight_init_mean, 0.0);
    EXPECT_EQ(config.weight_init_stdev, 1.0);
    EXPECT_EQ(config.weight_init_type, "normal");
    EXPECT_EQ(config.weight_max_value, 1.0);
    EXPECT_EQ(config.weight_min_value, -1.0);
    EXPECT_EQ(config.weight_mutate_rate, 0.9);
    EXPECT_EQ(config.weight_replace_rate, 0.1);
    EXPECT_EQ(config.max_stagnation, 15);
    EXPECT_EQ(config.species_elitism, 2);
    EXPECT_EQ(config.elitism, 2);
    EXPECT_EQ(config.survival_threshold, 0.2);
    EXPECT_EQ(config.min_species_size, 2);
    EXPECT_EQ(config.compatibility_threshold, 3.0);
    EXPECT_EQ(config.bad_species_threshold, 0.25);
}

TEST_F(ConfigTest, UnknownKeyInConfig)
{
    // Create a temporary config file with an unknown key
    std::string invalid_config_content = "unknown_key = 42";
    createTempConfigFile(invalid_config_content);

    // Suppress cerr output during the test
    testing::internal::CaptureStderr();

    // Load config from the temporary file
    NeatConfig config = load_config_from_file(temp_file_name);

    // Get cerr output during the test
    std::string output = testing::internal::GetCapturedStderr();

    // Assert that an unknown key prints an error message
    EXPECT_TRUE(output.find("Unknown key: unknown_key") != std::string::npos);
}
