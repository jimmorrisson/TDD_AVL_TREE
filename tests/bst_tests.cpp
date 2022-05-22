#include <catch2/catch_test_macros.hpp>
#include "bst.hpp"

TEST_CASE("Inserting to the bst lower values", "[bst, new node]")
{
    bst::Node<int> node;
    SECTION("Inserting to the null bst")
    {
        static constexpr auto value = 1;
        bst::Node<int> *node = nullptr;
        auto *returnedNode = bst::insertNode(node, value);
        REQUIRE(returnedNode->data == value);
    }
    SECTION("Inserting to the height == 1 tree lower value")
    {
        static constexpr auto value = 10;
        bst::Node<int> node{value, nullptr, nullptr};
        static constexpr auto leftChildValue = 5;
        bst::insertNode(&node, leftChildValue);
        REQUIRE(node.leftChild->data == leftChildValue);
    }
    SECTION("Inserting two lower values to the bst of height 1")
    {
        static constexpr auto value = 10;
        bst::Node<int> node{value, nullptr, nullptr};
        bst::insertNode(&node, 5);
        static constexpr auto leftChildValue = 3;
        bst::insertNode(&node, leftChildValue);
        REQUIRE(node.leftChild->leftChild->data == 3);
    }
    SECTION("Inserting ten lower values to the bst of height 1")
    {
        static constexpr auto value = 10;
        bst::Node<int> node{value, nullptr, nullptr};
        for (int i = value - 1; i > 0; --i)
        {
            bst::insertNode(&node, i);
            auto *currentNode = &node;
            while (currentNode->leftChild != nullptr)
            {
                currentNode = currentNode->leftChild;
            }
            REQUIRE(currentNode->data == i);
        }
    }
}

TEST_CASE("Inserting to the bst higher values", "[bst, new node]")
{
    SECTION("Inserting to the height == 1 tree higher value")
    {
        static constexpr auto value = 10;
        bst::Node<int> node{value, nullptr, nullptr};
        static constexpr auto rightChildValue = 15;
        bst::insertNode(&node, rightChildValue);
        REQUIRE(node.rightChild->data == rightChildValue);
    }
    SECTION("Inserting two higher values to the bst of height 1")
    {
        static constexpr auto value = 10;
        bst::Node<int> node{value, nullptr, nullptr};
        bst::insertNode(&node, 15);
        bst::insertNode(&node, 20);
        REQUIRE(node.rightChild->rightChild->data == 20);
    }
    SECTION("Inserting ten lower values to the bst of height 1")
    {
        static constexpr auto value = 0;
        bst::Node<int> node{value, nullptr, nullptr};
        for (int i = 1; i < 11; ++i)
        {
            bst::insertNode(&node, i);
            auto *currentNode = &node;
            while (currentNode->rightChild != nullptr)
            {
                currentNode = currentNode->rightChild;
            }
            REQUIRE(currentNode->data == i);
        }
    }
}

TEST_CASE("Search for data", "[bst, data]")
{
    SECTION("Find value in the root of bst")
    {
        static constexpr auto value = 1;
        bst::Node<int> node{value, nullptr, nullptr};
        REQUIRE(bst::search(&node, value) == &node);
    }
    SECTION("Find value in the leafs")
    {
        static constexpr auto value = 10;
        bst::Node<int> node{value, nullptr, nullptr};
        for (int i = 9; i > 0; --i)
        {
            bst::insertNode(&node, i);
        }
        for (int i = value + 1; i < 21; ++i)
        {
            bst::insertNode(&node, i);
        }
        {
            REQUIRE(bst::search(&node, 15) != nullptr);
            auto *retNode = bst::search(&node, 15);
            REQUIRE(retNode->rightChild->data == 16);
        } 
        {
            REQUIRE(bst::search(&node, 7) != nullptr);
            auto *retNode = bst::search(&node, 7);
            REQUIRE(retNode->leftChild->data == 6);
        }
    }
}