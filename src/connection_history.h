#ifndef CONNECTION_HISTORY_H
#define CONNECTION_HISTORY_H

#include <vector>
#include "node.h"
#include "genome.h"

/**
 * @brief Represents the history of a connection innovation in the NEAT (NeuroEvolution of Augmenting Topologies) algorithm.
 */
class ConnectionHistory
{
public:
    Node *from_node;                 // Pointer to the source node.
    Node *to_node;                   // Pointer to the target node.
    int innovation_nb;               // Innovation number of the connection.
    std::vector<int> innovation_nbs; // List of innovation numbers in the genome when the connection was added.

    /**
     * @brief Constructor for ConnectionHistory.
     * @param from_node Pointer to the source node.
     * @param to_node Pointer to the target node.
     * @param innovation_nb Innovation number of the connection.
     * @param innovation_nbs List of innovation numbers in the genome when the connection was added.
     */
    ConnectionHistory(Node *from_node, Node *to_node, int innovation_nb, std::vector<int> &innovation_nbs);

    /**
     * @brief Checks if the connection history matches a specific genome, source node, and target node.
     * @param genome Pointer to the genome to check against.
     * @param from_node Pointer to the source node.
     * @param to_node Pointer to the target node.
     * @return True if the connection history matches, false otherwise.
     */
    bool matches(const Genome *genome, Node *from_node, Node *to_node);
};

#endif
