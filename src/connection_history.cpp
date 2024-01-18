#include <iostream>
#include <vector>
#include "node.h"
#include "genome.h"
#include "connection_history.h"

ConnectionHistory::ConnectionHistory(Node *from_node, Node *to_node, int innovation_nb) : from_node(from_node), to_node(to_node), innovation_nb(innovation_nb) {}

bool ConnectionHistory::matches(const Genome *genome, Node *from_node, Node *to_node)
{
    if (from_node->id == this->from_node->id && to_node->id == this->to_node->id)
        // If reached this far, then the innovation numbers match the genes' innovation numbers and
        // the connection is between the same nodes, so it does match
        return true;

    return false;
}
