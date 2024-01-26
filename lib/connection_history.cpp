#include <iostream>
#include <vector>
#include "node.h"
#include "genome.h"
#include "connection_history.h"

ConnectionHistory::ConnectionHistory(Node *from_node, Node *to_node, int innovation_nb) : from_node(from_node), to_node(to_node), innovation_nb(innovation_nb) {}

bool ConnectionHistory::matches(Node *from_node, Node *to_node)
{
    return from_node->id == this->from_node->id && to_node->id == this->to_node->id;
}
