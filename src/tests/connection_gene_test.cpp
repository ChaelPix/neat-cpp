#include <gtest/gtest.h>
#include "../config.h"
#include "../node.h"
#include "../connection_gene.h"
#include "../activation_functions.h"

class ConnectionGeneTest : public ::testing::Test
{
protected:
    NeatConfig config;
    Node *fromNode;
    Node *toNode;
    ConnectionGene *gene;

    void SetUp() override
    {
        config = load_config_from_file("default_config.txt");
        fromNode = new Node(1, "sigmoid", 1);
        toNode = new Node(2, "sigmoid", 2);
        gene = new ConnectionGene(fromNode, toNode, 0.5, 1, true);
    }

    void TearDown() override
    {
        delete fromNode;
        delete toNode;
        delete gene;
    }

    // Add your test functions for ConnectionGene
    void testInit()
    {
        EXPECT_EQ(gene->from_node, fromNode);
        EXPECT_EQ(gene->to_node, toNode);
        EXPECT_DOUBLE_EQ(gene->weight, 0.5);
        EXPECT_EQ(gene->innovation_nb, 1);
        EXPECT_TRUE(gene->enabled);
    }

    void testMutateWeightReplace()
    {
        config.weight_replace_rate = 1.0;
        gene->mutate(config);
        EXPECT_TRUE(gene->weight >= -1.0 && gene->weight <= 1.0);
    }

    void testMutateWeightMutate()
    {
        config.weight_replace_rate = 0.0;
        config.weight_mutate_rate = 1.0;
        gene->mutate(config);
        EXPECT_NE(gene->weight, 0.5);
    }

    void testMutateEnabledMutate()
    {
        config.enabled_mutate_rate = 1.0;
        gene->mutate(config);
        EXPECT_FALSE(gene->enabled);
    }

    void testClone()
    {
        ConnectionGene *clonedConnectionGene = gene->clone(fromNode, toNode);
        EXPECT_EQ(clonedConnectionGene->from_node, fromNode);
        EXPECT_EQ(clonedConnectionGene->to_node, toNode);
        EXPECT_DOUBLE_EQ(clonedConnectionGene->weight, gene->weight);
        EXPECT_EQ(clonedConnectionGene->innovation_nb, gene->innovation_nb);
        EXPECT_EQ(clonedConnectionGene->enabled, gene->enabled);
        delete clonedConnectionGene;
    }
};

TEST_F(ConnectionGeneTest, Init)
{
    testInit();
}

TEST_F(ConnectionGeneTest, MutateWeightReplace)
{
    testMutateWeightReplace();
}

TEST_F(ConnectionGeneTest, MutateWeightMutate)
{
    testMutateWeightMutate();
}

TEST_F(ConnectionGeneTest, MutateEnabledMutate)
{
    testMutateEnabledMutate();
}

TEST_F(ConnectionGeneTest, Clone)
{
    testClone();
}
