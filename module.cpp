#include "pybind11/pybind11.h"
#include "pybind11/functional.h"
#include "pybind11/iostream.h"
#include "pybind11/stl.h"
#include <thread>
#include "lib/config.h"
#include "lib/population.h"
#include "lib/genome.h"

namespace py = pybind11;

PYBIND11_MODULE(neat, m)
{
    m.doc() = "NEAT algorithm module for Python";

    py::class_<NeatConfig>(m, "NeatConfig")
        .def(py::init<>())
        .def("__getitem__", [](const NeatConfig &config, std::string key) -> py::object
             {
                 if (key == "population_size")
                     return py::cast(config.population_size);
                 else if (key == "fitness_threshold")
                     return py::cast(config.fitness_threshold);
                 else if (key == "no_fitness_termination")
                     return py::cast(config.no_fitness_termination);
                 else if (key == "reset_on_extinction")
                     return py::cast(config.reset_on_extinction);
                 else if (key == "activation_default")
                     return py::cast(config.activation_default);
                 else if (key == "activation_mutate_rate")
                     return py::cast(config.activation_mutate_rate);
                 else if (key == "num_inputs")
                     return py::cast(config.num_inputs);
                 else if (key == "num_outputs")
                     return py::cast(config.num_outputs);
                 else if (key == "compatibility_disjoint_coefficient")
                     return py::cast(config.compatibility_disjoint_coefficient);
                 else if (key == "compatibility_weight_coefficient")
                     return py::cast(config.compatibility_weight_coefficient);
                 else if (key == "conn_add_prob")
                     return py::cast(config.conn_add_prob);
                 else if (key == "conn_delete_prob")
                     return py::cast(config.conn_delete_prob);
                 else if (key == "enabled_default")
                     return py::cast(config.enabled_default);
                 else if (key == "enabled_mutate_rate")
                     return py::cast(config.enabled_mutate_rate);
                 else if (key == "initial_connections")
                     return py::cast(config.initial_connections);
                 else if (key == "node_add_prob")
                     return py::cast(config.node_add_prob);
                 else if (key == "node_delete_prob")
                     return py::cast(config.node_delete_prob);
                 else if (key == "weight_init_mean")
                     return py::cast(config.weight_init_mean);
                 else if (key == "weight_init_stdev")
                     return py::cast(config.weight_init_stdev);
                 else if (key == "weight_init_type")
                     return py::cast(config.weight_init_type);
                 else if (key == "weight_max_value")
                     return py::cast(config.weight_max_value);
                 else if (key == "weight_min_value")
                     return py::cast(config.weight_min_value);
                 else if (key == "weight_mutate_rate")
                     return py::cast(config.weight_mutate_rate);
                 else if (key == "weight_replace_rate")
                     return py::cast(config.weight_replace_rate);
                 else if (key == "max_stagnation")
                     return py::cast(config.max_stagnation);
                 else if (key == "species_elitism")
                     return py::cast(config.species_elitism);
                 else if (key == "elitism")
                     return py::cast(config.elitism);
                 else if (key == "survival_threshold")
                     return py::cast(config.survival_threshold);
                 else if (key == "min_species_size")
                     return py::cast(config.min_species_size);
                 else if (key == "compatibility_threshold")
                     return py::cast(config.compatibility_threshold);
                 else if (key == "bad_species_threshold")
                     return py::cast(config.bad_species_threshold);
                 else
                     throw std::invalid_argument("Invalid key"); })

        .def("__setitem__", [](NeatConfig &config, std::string key, py::object value)
             {
                if (key == "population_size")
                    config.population_size = value.cast<decltype(config.population_size)>();
                else if (key == "fitness_threshold")
                     config.fitness_threshold = value.cast<decltype(config.fitness_threshold)>();
                else if (key == "no_fitness_termination")
                    config.no_fitness_termination = value.cast<decltype(config.no_fitness_termination)>();
                else if (key == "reset_on_extinction")
                    config.reset_on_extinction = value.cast<decltype(config.reset_on_extinction)>();
                else if (key == "activation_default")
                    config.activation_default = value.cast<decltype(config.activation_default)>();
                else if (key == "activation_mutate_rate")
                    config.activation_mutate_rate = value.cast<decltype(config.activation_mutate_rate)>();
                else if (key == "num_inputs")
                    config.num_inputs = value.cast<decltype(config.num_inputs)>();
                else if (key == "num_outputs")
                    config.num_outputs = value.cast<decltype(config.num_outputs)>();
                else if (key == "compatibility_disjoint_coefficient")
                    config.compatibility_disjoint_coefficient = value.cast<decltype(config.compatibility_disjoint_coefficient)>();
                else if (key == "compatibility_weight_coefficient")
                    config.compatibility_weight_coefficient = value.cast<decltype(config.compatibility_weight_coefficient)>();
                else if (key == "conn_add_prob")
                    config.conn_add_prob = value.cast<decltype(config.conn_add_prob)>();
                else if (key == "conn_delete_prob")
                    config.conn_delete_prob = value.cast<decltype(config.conn_delete_prob)>();
                else if (key == "enabled_default")
                    config.enabled_default = value.cast<decltype(config.enabled_default)>();
                else if (key == "enabled_mutate_rate")
                    config.enabled_mutate_rate = value.cast<decltype(config.enabled_mutate_rate)>();
                else if (key == "initial_connections")
                    config.initial_connections = value.cast<decltype(config.initial_connections)>();
                else if (key == "node_add_prob")
                    config.node_add_prob = value.cast<decltype(config.node_add_prob)>();
                else if (key == "node_delete_prob")
                    config.node_delete_prob = value.cast<decltype(config.node_delete_prob)>();
                else if (key == "weight_init_mean")
                    config.weight_init_mean = value.cast<decltype(config.weight_init_mean)>();
                else if (key == "weight_init_stdev")
                    config.weight_init_stdev = value.cast<decltype(config.weight_init_stdev)>();
                else if (key == "weight_init_type")
                    config.weight_init_type = value.cast<decltype(config.weight_init_type)>(); 
                else if (key == "weight_max_value") 
                    config.weight_max_value = value.cast<decltype(config.weight_max_value)>();
                else if (key == "weight_min_value")
                    config.weight_min_value = value.cast<decltype(config.weight_min_value)>();
                else if (key == "weight_mutate_rate")
                    config.weight_mutate_rate = value.cast<decltype(config.weight_mutate_rate)>();
                else if (key == "weight_replace_rate")
                    config.weight_replace_rate = value.cast<decltype(config.weight_replace_rate)>();
                else if (key == "max_stagnation")
                    config.max_stagnation = value.cast<decltype(config.max_stagnation)>();
                else if (key == "species_elitism")
                    config.species_elitism = value.cast<decltype(config.species_elitism)>();
                else if (key == "elitism")
                    config.elitism = value.cast<decltype(config.elitism)>();
                else if (key == "survival_threshold")
                    config.survival_threshold = value.cast<decltype(config.survival_threshold)>();
                else if (key == "min_species_size")
                    config.min_species_size = value.cast<decltype(config.min_species_size)>();
                else if (key == "compatibility_threshold")
                    config.compatibility_threshold = value.cast<decltype(config.compatibility_threshold)>();
                else if (key == "bad_species_threshold")
                    config.bad_species_threshold = value.cast<decltype(config.bad_species_threshold)>();
                else
                    throw std::invalid_argument("Invalid key"); });

    m.def("load_config", &load_config, "Loads NEAT configuration from a file.");

    py::class_<Population>(m, "Population")
        .def(py::init<const NeatConfig &>())
        .def_readonly("generation", &Population::generation, "Current generation number.")
        .def_readonly("best_fitness", &Population::best_fitness, "Fitness of the best genome.")
        .def_readonly("average_fitness", &Population::average_fitness, "Average fitness of the population.")
        .def_readonly("best_genome", &Population::best_genome, "Best genome in the population.")
        .def_readonly("genomes", &Population::genomes, "List of genomes in the population.")
        .def("run", &Population::run, py::call_guard<py::gil_scoped_release>(), "Runs the NEAT algorithm for a specified number of generations.");

    py::class_<Genome>(m, "Genome")
        .def(py::init<const NeatConfig &, bool>())
        .def_readonly("id", &Genome::id, "Unique identifier for the genome.")
        .def_readwrite("fitness", &Genome::fitness, "Fitness score of the genome.")
        .def("feed_forward", &Genome::feed_forward, "Performs a feed-forward operation on the neural network.")
        .def("print", &Genome::print, "Print the genome's structure.")
        .def("save", &Genome::save, "Saves the genome to a binary file.")
        .def_static("load", &Genome::load, "Loads the genome from a binary file.");
}