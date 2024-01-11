#include <gtest/gtest.h>
#include <vector>
#include "../config.h"
#include "../genome.h"
#include "../node.h"
#include "../connection_history.h"   // Include your connection_history.h file here
#include "../activation_functions.h" // Include your activation_functions.h file here

class ConnectionHistoryTest : public ::testing::Test
{
protected:
    NeatConfig config;
    Genome *genome;
    Node *fromNode;
    Node *toNode;
    std::vector<int> innovationNbs;
    ConnectionHistory *connectionHistory;

    void SetUp() override
    {
        config = load_config_from_file("default_config.txt");
        genome = new Genome(config);
        fromNode = new Node(1, "sigmoid", 1);
        toNode = new Node(2, "sigmoid", 2);
        innovationNbs = {1};
        genome->genes.emplace_back(new ConnectionGene(fromNode, toNode, 0.5, 1, true));
        connectionHistory = new ConnectionHistory(fromNode, toNode, 1, innovationNbs);
    }

    void TearDown() override
    {
        delete genome;
        delete fromNode;
        delete toNode;
        delete connectionHistory;
    }

    void testInit()
    {
        EXPECT_EQ(connectionHistory->from_node, fromNode);
        EXPECT_EQ(connectionHistory->to_node, toNode);
        EXPECT_EQ(connectionHistory->innovation_nbs, innovationNbs);
    }

    void testMatchesWithExistingConnection()
    {
        bool result = connectionHistory->matches(genome, fromNode, toNode);
        EXPECT_TRUE(result);
    }

    void testMatchesWithNonExistingConnection()
    {
        Node *node = new Node(3, "relu", 2);
        bool result = connectionHistory->matches(genome, fromNode, node);
        delete node;
        EXPECT_FALSE(result);
    }

    void testMatchesWithNotSameGenome()
    {
        std::vector<int> innovationNbs = {2};
        ConnectionHistory *otherConnectionHistory = new ConnectionHistory(fromNode, toNode, 1, innovationNbs);
        bool result = otherConnectionHistory->matches(genome, fromNode, toNode);
        delete otherConnectionHistory;
        EXPECT_FALSE(result);
    }
};

TEST_F(ConnectionHistoryTest, Init)
{
    testInit();
}

TEST_F(ConnectionHistoryTest, MatchesWithExistingConnection)
{
    testMatchesWithExistingConnection();
}

TEST_F(ConnectionHistoryTest, MatchesWithNonExistingConnection)
{
    testMatchesWithNonExistingConnection();
}

TEST_F(ConnectionHistoryTest, MatchesWithNotSameGenome)
{
    testMatchesWithNotSameGenome();
}
