#include <gtest/gtest.h>
#include <vector>
#include "../config.h"
#include "../genome.h"
#include "../node.h"
#include "../connection_history.h"
#include "../activation_functions.h"

class ConnectionHistoryTest : public ::testing::Test
{
protected:
    NeatConfig config;
    Genome *genome;
    Node *fromNode;
    Node *toNode;
    ConnectionHistory *connectionHistory;

    void SetUp() override
    {
        config = load_config_from_file("default_config.txt");
        genome = new Genome(config);
        fromNode = new Node(1, "sigmoid", 1);
        toNode = new Node(2, "sigmoid", 2);
        genome->genes.emplace_back(new ConnectionGene(fromNode, toNode, 0.5, 1, true));
        connectionHistory = new ConnectionHistory(fromNode, toNode, 1);
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
        ASSERT_EQ(connectionHistory->from_node, fromNode);
        ASSERT_EQ(connectionHistory->to_node, toNode);
    }

    void testMatchesWithExistingConnection()
    {
        bool result = connectionHistory->matches(fromNode, toNode);
        ASSERT_TRUE(result);
    }

    void testMatchesWithNonExistingConnection()
    {
        Node *node = new Node(3, "relu", 2);
        bool result = connectionHistory->matches(fromNode, node);
        delete node;
        ASSERT_FALSE(result);
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
