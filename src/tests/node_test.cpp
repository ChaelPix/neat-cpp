#include <gtest/gtest.h>
#include "../config.h"
#include "../node.h"
#include "../connection_gene.h"

class TestNode : public ::testing::Test
{
protected:
    NeatConfig config;
    Node *node;

    void SetUp() override
    {
        config = load_config_from_file("default_config.txt");
        node = new Node(1, "relu", 1);
    }

    void TearDown() override
    {
        delete node;
    }

    void test_initialization()
    {
        EXPECT_EQ(node->id, 1);
        EXPECT_EQ(node->input_sum, 0);
        EXPECT_EQ(node->output_value, 0);
        EXPECT_EQ(node->output_connections.size(), 0);
        EXPECT_EQ(node->layer, 1);
        EXPECT_EQ(node->activation_function, "relu");
    }

    void test_activation()
    {
        // Test activate method when layer is not 0
        node->input_sum = 0.5;
        node->activate();
        node->propagate_output();
        // Output value should be updated
        EXPECT_NE(node->output_value, 0);
    }

    void test_mutate()
    {
        // Test mutation of a bias node
        config.bias_mutate_rate = 1.0;
        config.bias_replace_rate = 0.0;
        node->output_value = 0.0;
        node->mutate(config, true); // true indicates bias node
        EXPECT_NE(node->output_value, 0.0);

        // Test mutation of a non-bias node
        config.bias_mutate_rate = 0.0;
        config.bias_replace_rate = 1.0;
        node->output_value = 0.0;
        node->mutate(config, true);
        EXPECT_NE(node->output_value, 0.0);

        // Test activation mutation
        config.activation_mutate_rate = 1.0;
        node->mutate(config);
        EXPECT_NE(node->activation_function, "relu");
    }

    void test_is_connected_to()
    {
        Node *node1 = new Node(2, "step", 2);
        Node *node2 = new Node(3, "sigmoid", 3);
        node->output_connections.push_back(new ConnectionGene(node, node1, 1.0, 1, true));
        node->output_connections.push_back(new ConnectionGene(node, node2, 1.0, 1, true));
        // Validate if the node is connected to the provided node
        EXPECT_TRUE(node->is_connected_to(node1));
        EXPECT_TRUE(node->is_connected_to(node2));

        Node *node3 = new Node(4, "tanh", 1);
        // Can't be connected if is on the same layer
        EXPECT_FALSE(node->is_connected_to(node3));

        Node *node4 = new Node(4, "relu", 0);
        node4->output_connections.push_back(new ConnectionGene(node4, node, 1.0, 1, true));
        // Connected because of the connection gene
        EXPECT_TRUE(node->is_connected_to(node4));

        Node *node5 = new Node(5, "softmax", 0);
        node5->output_connections = {};
        // Can't be connected if is on a previous layer and no connection gene
        EXPECT_FALSE(node->is_connected_to(node5));

        delete node1;
        delete node2;
        delete node3;
        delete node4;
        delete node5;
    }

    void test_clone()
    {
        Node *cloned_node = node->clone();
        EXPECT_TRUE(node->is_equal(cloned_node));
        delete cloned_node;
    }
};

TEST_F(TestNode, Initialization)
{
    test_initialization();
}

TEST_F(TestNode, Activation)
{
    test_activation();
}

TEST_F(TestNode, Mutate)
{
    test_mutate();
}

TEST_F(TestNode, IsConnectedTo)
{
    test_is_connected_to();
}

TEST_F(TestNode, Clone)
{
    test_clone();
}
