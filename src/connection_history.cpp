#include <iostream>
#include <vector>
#include "node.h"
#include "genome.h"
#include "connection_history.h"

ConnectionHistory::ConnectionHistory(Node *from_node, Node *to_node, int innovation_nb, std::vector<int> innovation_nbs)
    : from_node(from_node), to_node(to_node), innovation_nb(innovation_nb), innovation_nbs(innovation_nbs) {}

bool ConnectionHistory::matches(const Genome *genome, Node *from_node, Node *to_node)
{
    // If the number of connections is different, then the genomes aren't the same
    if (genome->genes.size() == innovation_nbs.size())
    {
        if (from_node->id == this->from_node->id && to_node->id == this->to_node->id)
        {
            // Next, check if all the innovation numbers match from the genome
            for (auto &g : genome->genes)
            {
                if (std::find(innovation_nbs.begin(), innovation_nbs.end(), g->innovation_nb) == innovation_nbs.end())
                {
                    return false;
                }
            }

            // If reached this far, then the innovation numbers match the genes' innovation numbers and
            // the connection is between the same nodes, so it does match
            return true;
        }
    }

    return false;
}
