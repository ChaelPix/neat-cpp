#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include "math_utils.h"
#include "config.h"
#include "node.h"
#include "connection_gene.h"
#include "connection_history.h"
#include "genome.h"

std::string generate_uid(int size)
{
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string uid;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    for (int i = 0; i < size; ++i)
    {
        uid += characters[dis(gen)];
    }

    return uid;
}

int next_connection_nb = 1;

Genome::Genome(){};

Genome::Genome(const NeatConfig &config, bool crossover) : config(config), inputs(config.num_inputs), outputs(config.num_outputs), layers(2), next_node(0), fitness(0)
{
    id = generate_uid(8);

    if (crossover)
    {
        return;
    }

    // create input nodes
    for (int i = 0; i < inputs; ++i)
    {
        nodes.push_back(new Node(i, config.activation_default, 0));
        ++next_node;
    }

    // create output nodes
    for (int i = 0; i < outputs; ++i)
    {
        nodes.push_back(new Node(i + inputs, config.activation_default, 1));
        ++next_node;
    }

    // bias node
    nodes.push_back(new Node(next_node, config.activation_default, 0));
    bias_node = next_node;
    ++next_node;
}

Genome::~Genome()
{
    // Clean up memory for nodes
    for (auto &node : nodes)
    {
        delete node;
    }

    // Clean up memory for genes
    for (auto &gene : genes)
    {
        delete gene;
    }
}

void Genome::fully_connect(std::vector<ConnectionHistory> &innovation_history)
{
    for (int i = 0; i < inputs; ++i)
    {
        for (int j = 0; j < outputs; ++j)
        {
            int connection_innovation_nb = get_innovation_number(
                innovation_history,
                nodes[i],
                nodes[inputs + j]);

            genes.push_back(
                new ConnectionGene(
                    nodes[i],
                    nodes[inputs + j],
                    new_connection_weight(),
                    connection_innovation_nb,
                    config.enabled_default));
        }
    }

    for (int i = 0; i < outputs; ++i)
    {
        int connection_innovation_nb = get_innovation_number(
            innovation_history,
            nodes[bias_node],
            nodes[inputs + i]);

        genes.push_back(
            new ConnectionGene(
                nodes[bias_node],
                nodes[inputs + i],
                new_connection_weight(),
                connection_innovation_nb,
                config.enabled_default));
    }

    connect_nodes();
}

Node *Genome::get_node(int id)
{
    for (auto &n : nodes)
    {
        if (n->id == id)
        {
            return n;
        }
    }
    return nullptr;
}

void Genome::connect_nodes()
{
    // Clear the connections for each node
    for (auto &n : nodes)
    {
        n->output_connections.clear();
    }

    // Add the connections to the nodes
    for (auto &g : genes)
    {
        g->from_node->output_connections.push_back(g);
    }
}

std::vector<double> Genome::feed_forward(std::vector<double> input_values)
{
    try
    {
        // Set the outputs of the input nodes
        for (int i = 0; i < inputs; ++i)
        {
            nodes[i]->output_value = input_values[i];
        }

        // Output of bias is 1
        nodes[bias_node]->output_value = 1;

        // Engage each node in the network
        for (auto &n : network)
        {
            n->activate();
            n->propagate_output();
        }

        // The outputs are nodes[inputs] to nodes[inputs+outputs-1]
        std::vector<double> outs(outputs, 0.0);
        for (int i = 0; i < outputs; ++i)
        {
            outs[i] = nodes[inputs + i]->output_value;
        }

        // Reset all the nodes for the next feed forward
        for (auto &n : nodes)
        {
            n->input_sum = 0;
        }

        return outs;
    }
    catch (const std::exception &e)
    {
        // Handle the exception here or re-throw if needed
        std::cerr << "An error occurred during feed_forward: " << e.what() << std::endl;
        // Optionally, re-throw the exception to propagate it further
        // throw;
        return {}; // Return an empty vector or handle the error accordingly
    }
}

void Genome::generate_network()
{
    connect_nodes();
    network.clear();

    // For each layer, add the nodes in that layer to the network
    for (int l = 0; l < layers; ++l)
    {
        for (auto &n : nodes)
        {
            if (n->layer == l)
            {
                network.push_back(n);
            }
        }
    }
}

void Genome::add_node(std::vector<ConnectionHistory> &innovation_history)
{
    // Pick a random connection to create a node between
    if (genes.empty())
    {
        add_connection(innovation_history);
        return;
    }

    int random_connection = rand() % genes.size();

    while (genes[random_connection]->from_node == nodes[bias_node] && genes.size() != 1)
    {
        // Don't disconnect bias
        random_connection = rand() % genes.size();
    }

    genes[random_connection]->enabled = false; // Disable the connection

    int new_node_nb = next_node;
    nodes.push_back(new Node{new_node_nb, config.activation_default});
    ++next_node;

    // Add a new connection to the new node with a weight of 1
    int connection_innovation_nb = get_innovation_number(
        innovation_history,
        genes[random_connection]->from_node,
        get_node(new_node_nb));

    genes.push_back(
        new ConnectionGene{
            genes[random_connection]->from_node,
            get_node(new_node_nb),
            1,
            connection_innovation_nb,
            config.enabled_default});

    connection_innovation_nb = get_innovation_number(
        innovation_history,
        get_node(new_node_nb),
        genes[random_connection]->to_node);

    // Add a new connection from the new node with a weight the same as the disabled connection
    genes.push_back(
        new ConnectionGene{
            get_node(new_node_nb),
            genes[random_connection]->to_node,
            genes[random_connection]->weight,
            connection_innovation_nb,
            config.enabled_default});

    get_node(new_node_nb)->layer = genes[random_connection]->from_node->layer + 1;

    connection_innovation_nb = get_innovation_number(
        innovation_history,
        nodes[bias_node],
        get_node(new_node_nb));

    // Get the value for the bias node
    double bias_value = 0.0;
    if (config.bias_init_type == "normal")
    {
        bias_value = normal(
            config.bias_init_mean, config.bias_init_stdev);

        // Keep value between bounds
        if (bias_value > config.bias_max_value)
        {
            bias_value = config.bias_max_value;
        }
        if (bias_value < config.bias_min_value)
        {
            bias_value = config.bias_min_value;
        }
    }
    else if (config.bias_init_type == "uniform")
    {
        bias_value = uniform(
            config.bias_min_value, config.bias_max_value);
    }

    // Connect the bias to the new node
    genes.push_back(
        new ConnectionGene{
            nodes[bias_node],
            get_node(new_node_nb),
            bias_value,
            connection_innovation_nb,
            config.enabled_default});

    // If the layer of the new node is equal to the layer of the output node of the old connection,
    // then a new layer needs to be created
    if (get_node(new_node_nb)->layer == genes[random_connection]->to_node->layer)
    {
        for (auto &n : nodes)
        {
            // Don't include this newest node
            if (n->layer >= get_node(new_node_nb)->layer)
            {
                n->layer += 1;
            }
        }
        ++layers;
    }

    connect_nodes();
}

void Genome::remove_node()
{
    // Select a random node by excluding inputs, outputs, and bias nodes
    auto it = std::find_if(nodes.begin() + bias_node, nodes.end(), [&](const Node *n)
                           { return n->layer != 0; });

    if (it != nodes.end())
    {
        Node *random_node = *it;
        nodes.erase(it);

        // Remove the connections that are connected to the random node selected
        auto removeCondition = [&](const ConnectionGene *g)
        {
            return g->from_node == random_node || g->to_node == random_node;
        };

        genes.erase(std::remove_if(genes.begin(), genes.end(), removeCondition), genes.end());
    }
}

void Genome::add_connection(std::vector<ConnectionHistory> &innovation_history)
{
    // Cannot add a connection to a fully connected network
    if (fully_connected())
    {
        return;
    }

    auto random_connection_nodes_are_valid = [&](int rand1, int rand2)
    {
        return (nodes[rand1]->layer != nodes[rand2]->layer) && !nodes[rand1]->is_connected_to(nodes[rand2]);
    };

    // Get random nodes
    int random_node_1 = rand() % nodes.size();
    int random_node_2 = rand() % nodes.size();

    while (!random_connection_nodes_are_valid(random_node_1, random_node_2))
    {
        // While the random nodes are not valid, get new ones
        random_node_1 = rand() % nodes.size();
        random_node_2 = rand() % nodes.size();
    }

    int temp;
    if (nodes[random_node_1]->layer > nodes[random_node_2]->layer)
    {
        // If the first random node is after the second, then switch
        temp = random_node_2;
        random_node_2 = random_node_1;
        random_node_1 = temp;
    }

    // Get the innovation number of the connection
    // This will be a new number if no identical genome has mutated in the same way
    int connection_innovation_nb = get_innovation_number(
        innovation_history,
        nodes[random_node_1],
        nodes[random_node_2]);

    // Add the connection with a random weight
    genes.push_back(
        new ConnectionGene(
            nodes[random_node_1],
            nodes[random_node_2],
            new_connection_weight(),
            connection_innovation_nb,
            config.enabled_default));

    connect_nodes();
}

void Genome::remove_connection()
{
    if (!genes.empty())
    {
        auto random_gene = genes.begin() + rand() % genes.size();
        genes.erase(random_gene);
    }
}

double Genome::new_connection_weight() const
{
    double weight = 0.0;

    if (config.weight_init_type == "normal")
    {
        weight = normal(config.weight_init_mean, config.weight_init_stdev);

        // Keep the value between bounds
        if (weight > config.weight_max_value)
        {
            weight = config.weight_max_value;
        }
        if (weight < config.weight_min_value)
        {
            weight = config.weight_min_value;
        }
    }
    else if (config.weight_init_type == "uniform")
    {
        weight = uniform(config.weight_min_value, config.weight_max_value);
    }

    return weight;
}

int Genome::get_innovation_number(std::vector<ConnectionHistory> &innovation_history, Node *from_node, Node *to_node) const
{
    bool is_new = true;
    int connection_innovation_nb = next_connection_nb;

    for (auto &history : innovation_history)
    {
        // For each previous mutation
        if (history.matches(this, from_node, to_node))
        {
            // If a match is found
            is_new = false; // It's not a new mutation
            connection_innovation_nb = history.innovation_nb;
            break;
        }
    }

    if (is_new)
    {
        // If the mutation is new, create a vector of connection history representing the current state of the genome
        std::vector<int> innovation_numbers;
        for (const auto &gene : genes)
        {
            // Set the innovation numbers
            innovation_numbers.push_back(gene->innovation_nb);
        }

        // Then, add this mutation to the innovation history
        innovation_history.emplace_back(from_node, to_node, connection_innovation_nb, innovation_numbers);
        ++next_connection_nb;
    }

    return connection_innovation_nb;
}

bool Genome::fully_connected() const
{
    int max_connections = 0;

    // Array that stores the number of nodes in each layer
    std::vector<int> nodes_in_layers(layers, 0);

    // Populate the array
    for (const auto &node : nodes)
    {
        ++nodes_in_layers[node->layer];
    }

    // For each layer, calculate the maximum number of connections
    for (int i = 0; i < layers - 1; ++i)
    {
        int nodes_in_front = 0;
        for (int j = i + 1; j < layers; ++j)
        {
            // For each layer in front of this layer
            nodes_in_front += nodes_in_layers[j]; // Add up nodes
        }

        max_connections += nodes_in_layers[i] * nodes_in_front;
    }

    return max_connections <= static_cast<int>(genes.size());
}

void Genome::mutate(std::vector<ConnectionHistory> &innovation_history)
{
    if (genes.empty())
    {
        add_connection(innovation_history);
    }

    for (auto &node : nodes)
    {
        bool is_bias_node = (&node == &nodes[bias_node]);
        node->mutate(config, is_bias_node);
    }

    for (auto &gene : genes)
    {
        gene->mutate(config);
    }

    if (rand() < RAND_MAX * config.conn_add_prob)
    {
        add_connection(innovation_history);
    }

    if (rand() < RAND_MAX * config.conn_delete_prob)
    {
        remove_connection();
    }

    if (rand() < RAND_MAX * config.node_add_prob)
    {
        add_node(innovation_history);
    }

    if (rand() < RAND_MAX * config.node_delete_prob)
    {
        remove_node();
    }
}

Genome *Genome::crossover(Genome *parent) const
{
    Genome *child = new Genome(config, true);
    child->genes.clear();
    child->nodes.clear();
    child->layers = layers;
    child->next_node = next_node;
    child->bias_node = bias_node;

    std::vector<ConnectionGene *> child_genes;
    std::vector<bool> is_enabled;

    // All inherited genes
    for (const auto &gene : genes)
    {
        bool set_enabled = true; // Is this connection in the child going to be enabled

        int parent_gene_index = matching_gene(parent, gene->innovation_nb);
        if (parent_gene_index != -1)
        {
            // If the genes match
            if (!gene->enabled || !parent->genes[parent_gene_index]->enabled)
            {
                // If either of the matching genes is disabled
                if (rand() < 0.75 * RAND_MAX)
                {
                    // 75% of the time disable the child gene
                    set_enabled = false;
                }
            }

            if (rand() < 0.5 * RAND_MAX)
            {
                child_genes.push_back(gene);
            }
            else
            {
                // Get gene from the parent
                child_genes.push_back(parent->genes[parent_gene_index]);
            }
        }
        else
        {
            // Disjoint or excess gene
            child_genes.push_back(gene);
            set_enabled = gene->enabled;
        }

        is_enabled.push_back(set_enabled);
    }

    // Since all excess and disjoint genes are inherited from the more fit parent (this Genome),
    // the child structure is no different from this parent, except for dormant connections being enabled,
    // but this won't affect nodes.
    // So, all the nodes can be inherited from this parent.
    for (auto &node : nodes)
    {
        child->nodes.push_back(node->clone());
    }

    // Clone all the connections so that they connect the child new nodes
    for (size_t i = 0; i < child_genes.size(); ++i)
    {
        child->genes.push_back(child_genes[i]->clone(
            child->get_node(child_genes[i]->from_node->id),
            child->get_node(child_genes[i]->to_node->id)));
        child->genes[i]->enabled = is_enabled[i];
    }

    child->connect_nodes();
    return child;
}

int Genome::matching_gene(Genome *parent, int innovation) const
{
    for (size_t i = 0; i < parent->genes.size(); ++i)
    {
        if (parent->genes[i]->innovation_nb == innovation)
        {
            return static_cast<int>(i);
        }
    }
    return -1; // No matching gene found
}

void Genome::print_genome() const
{
    std::cout << "------------------------------ GENOME ----------------------------\n";
    std::cout << "⚪️ Resume: {"
              << "layers: " << layers << ", bias nodes: " << bias_node << ", nodes: " << nodes.size() << "}\n";
    std::cout << "⚪️ Connection genes:\n";

    for (const auto &gene : genes)
    {
        std::cout << "{"
                  << "innovation_nb: " << gene->innovation_nb << ", "
                  << "from_node: " << gene->from_node->id << ", "
                  << "to_node: " << gene->to_node->id << ", "
                  << "enabled: " << gene->enabled << ", "
                  << "from_layer: " << gene->from_node->layer << ", "
                  << "to_layer: " << gene->to_node->layer << ", "
                  << "weight: " << gene->weight << "}\n";
    }

    std::cout << std::endl;
}

Genome *Genome::clone()
{
    Genome *clone = new Genome(config, true);

    // Copy nodes
    for (auto node : nodes)
    {
        clone->nodes.push_back(node->clone());
    }

    // Copy genes and connect them to the clone new nodes
    for (auto gene : genes)
    {
        clone->genes.push_back(gene->clone(
            clone->get_node(gene->from_node->id),
            clone->get_node(gene->to_node->id)));
    }

    clone->layers = layers;
    clone->next_node = next_node;
    clone->bias_node = bias_node;
    clone->fitness = fitness;
    clone->connect_nodes();

    return clone;
}

void Genome::save(const std::string &file_path)
{
    // Check if the directory exists
    size_t pos = file_path.find_last_of('/');
    std::string dir = file_path.substr(0, pos);
    if (!dir.empty() && !std::__fs::filesystem::exists(dir))
    {
        std::__fs::filesystem::create_directories(dir);
    }

    // Serialize and save the model to a file
    std::ofstream file(file_path, std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<const char *>(this), sizeof(*this));
        file.close();
        std::cout << "Genome saved to '" << file_path << "'" << std::endl;
    }
    else
    {
        std::cerr << "Failed to save genome to '" << file_path << "'" << std::endl;
    }
}

Genome Genome::load(const std::string &file_path)
{
    // Load the saved genome
    Genome loadedGenome;
    std::ifstream file(file_path, std::ios::binary);
    if (file.is_open())
    {
        file.read(reinterpret_cast<char *>(&loadedGenome), sizeof(loadedGenome));
        file.close();
        std::cout << "Genome loaded from '" << file_path << "'" << std::endl;
    }
    else
    {
        std::cerr << "Failed to load genome from '" << file_path << "'" << std::endl;
    }
    return loadedGenome;
}
