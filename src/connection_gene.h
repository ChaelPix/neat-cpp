#ifndef CONNECTION_GENE_H
#define CONNECTION_GENE_H

#include "config.h"

class Node; // Forward declaration

/**
 * @brief Represents a connection gene in the NEAT (NeuroEvolution of Augmenting Topologies) algorithm.
 */
class ConnectionGene
{
public:
    Node *from_node;   // Pointer to the source node.
    Node *to_node;     // Pointer to the target node.
    double weight;     // Weight of the connection.
    int innovation_nb; // Innovation number of the connection.
    bool enabled;      // Flag indicating whether the connection is enabled or disabled.

    /**
     * @brief Constructor for ConnectionGene.
     * @param from Pointer to the source node.
     * @param to Pointer to the target node.
     * @param w Weight of the connection.
     * @param innovation Innovation number of the connection.
     * @param enabled Flag indicating whether the connection is enabled or disabled.
     */
    ConnectionGene(Node *from, Node *to, double w, int innovation, bool enabled);

    /**
     * @brief Mutates the connection gene based on the NEAT configuration.
     * @param config Reference to the NEAT configuration.
     */
    void mutate(const NeatConfig &config);

    /**
     * @brief Creates a clone of the connection gene with specified source and target nodes.
     * @param from Pointer to the new source node.
     * @param to Pointer to the new target node.
     * @return Pointer to the cloned ConnectionGene.
     */
    ConnectionGene *clone(Node *from, Node *to);
};

#endif
