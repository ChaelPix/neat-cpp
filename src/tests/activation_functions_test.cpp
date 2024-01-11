#include <gtest/gtest.h>
#include <cmath>
#include "../activation_functions.h"

class ActivationFunctionsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Optional: Add any setup code specific to your tests
    }

    void TearDown() override
    {
        // Optional: Add any cleanup code specific to your tests
    }

    void testStep()
    {
        EXPECT_EQ(step(1.0), 1.0);
        EXPECT_EQ(step(0.0), 0.0);
        EXPECT_EQ(step(-1.0), 0.0);
    }

    void testSigmoid()
    {
        EXPECT_DOUBLE_EQ(sigmoid(0.0), 0.5);
        EXPECT_DOUBLE_EQ(sigmoid(1.0), 1.0 / (1.0 + exp(-1.0)));
    }

    void testTanh()
    {
        EXPECT_DOUBLE_EQ(tanh(0.0), 0.0);
        EXPECT_DOUBLE_EQ(tanh(1.0), std::tanh(1.0));
    }

    void testRelu()
    {
        EXPECT_DOUBLE_EQ(relu(1.0), 1.0);
        EXPECT_DOUBLE_EQ(relu(0.0), 0.0);
        EXPECT_DOUBLE_EQ(relu(-1.0), 0.0);
    }

    void testLeakyRelu()
    {
        EXPECT_DOUBLE_EQ(leaky_relu(1.0), 1.0);
        EXPECT_DOUBLE_EQ(leaky_relu(0.0), 0.0);
        EXPECT_DOUBLE_EQ(leaky_relu(-1.0), -0.01);
    }

    void testPrelu()
    {
        EXPECT_DOUBLE_EQ(prelu(1.0), 1.0);
        EXPECT_DOUBLE_EQ(prelu(0.0), 0.0);
        EXPECT_DOUBLE_EQ(prelu(-1.0), -0.01);
    }

    void testElu()
    {
        EXPECT_DOUBLE_EQ(elu(1.0), 1.0);
        EXPECT_DOUBLE_EQ(elu(0.0), 0.0);
        EXPECT_DOUBLE_EQ(elu(-1.0), -0.6321);
    }

    void testSoftmax()
    {
        EXPECT_DOUBLE_EQ(softmax(1.0), 1.0 / (1.0 + std::exp(-1.0)));
        EXPECT_DOUBLE_EQ(softmax(0.0), 1.0 / (1.0 + std::exp(0.0)));
    }

    void testLinear()
    {
        EXPECT_DOUBLE_EQ(linear(1.0), 1.0);
        EXPECT_DOUBLE_EQ(linear(0.0), 0.0);
        EXPECT_DOUBLE_EQ(linear(-1.0), -1.0);
    }

    void testSwish()
    {
        EXPECT_DOUBLE_EQ(swish(1.0), 0.73106);
        EXPECT_DOUBLE_EQ(swish(0.0), 0.0);
        EXPECT_DOUBLE_EQ(swish(-1.0), -0.26894);
    }
};

TEST_F(ActivationFunctionsTest, Step)
{
    testStep();
}

TEST_F(ActivationFunctionsTest, Sigmoid)
{
    testSigmoid();
}

TEST_F(ActivationFunctionsTest, Tanh)
{
    testTanh();
}

TEST_F(ActivationFunctionsTest, Relu)
{
    testRelu();
}

TEST_F(ActivationFunctionsTest, LeakyRelu)
{
    testLeakyRelu();
}

TEST_F(ActivationFunctionsTest, Prelu)
{
    testPrelu();
}

TEST_F(ActivationFunctionsTest, Elu)
{
    testElu();
}

TEST_F(ActivationFunctionsTest, Softmax)
{
    testSoftmax();
}

TEST_F(ActivationFunctionsTest, Linear)
{
    testLinear();
}

TEST_F(ActivationFunctionsTest, Swish)
{
    testSwish();
}
