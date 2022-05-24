#include <catch2/catch_test_macros.hpp>
#include "bst.hpp"

TEST_CASE("Inserting to the bst lower values", "[bst, new node]")
{
    SECTION("Inserting to the null bst")
    {
        static constexpr auto value = 1;
        std::unique_ptr<bst::Node<int>> node;
        auto returnedNode = bst::insertNode(std::move(node), value);
        REQUIRE(returnedNode->data == value);
    }
    SECTION("Inserting to the height == 1 tree lower value")
    {
        static constexpr auto value = 10;
        static constexpr auto leftChildValue = 5;
        auto node = bst::insertNode(std::make_unique<bst::Node<int>>(value, nullptr, nullptr), leftChildValue);
        REQUIRE(node->leftChild->data == leftChildValue);
    }
    SECTION("Inserting two lower values to the bst of height 1")
    {
        static constexpr auto value = 10;
        auto node = bst::insertNode(std::make_unique<bst::Node<int>>(value, nullptr, nullptr), 5);
        static constexpr auto leftChildValue = 3;
        node = bst::insertNode(std::move(node), leftChildValue);
        REQUIRE(node->leftChild->leftChild->data == 3);
    }
    SECTION("Inserting ten lower values to the bst of height 1")
    {
        static constexpr auto value = 10;
        auto node = std::make_unique<bst::Node<int>>(value, nullptr, nullptr);
        for (int i = value - 1; i > 0; --i)
        {
            node = bst::insertNode(std::move(node), i);
            auto *currentNode = node.get();
            while (currentNode->leftChild != nullptr)
            {
                currentNode = currentNode->leftChild.get();
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
        static constexpr auto rightChildValue = 15;
        auto node = bst::insertNode(std::make_unique<bst::Node<int>>(value, nullptr, nullptr), rightChildValue);
        REQUIRE(node->rightChild->data == rightChildValue);
    }
    SECTION("Inserting two higher values to the bst of height 1")
    {
        static constexpr auto value = 10;
        auto node = bst::insertNode(std::make_unique<bst::Node<int>>(value, nullptr, nullptr), 15);
        node = bst::insertNode(std::move(node), 20);
        REQUIRE(node->rightChild->rightChild->data == 20);
    }
    SECTION("Inserting ten lower values to the bst of height 1")
    {
        static constexpr auto value = 0;
        auto node = std::make_unique<bst::Node<int>>(value, nullptr, nullptr);
        for (int i = 1; i < 11; ++i)
        {
            node = bst::insertNode(std::move(node), i);
            auto *currentNode = node.get();
            while (currentNode->rightChild != nullptr)
            {
                currentNode = currentNode->rightChild.get();
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
        auto node = std::make_unique<bst::Node<int>>(value, nullptr, nullptr);
        for (int i = 9; i > 0; --i)
        {
            node = bst::insertNode(std::move(node), i);
        }
        for (int i = value + 1; i < 21; ++i)
        {
            node = bst::insertNode(std::move(node), i);
        }
        {
            REQUIRE(bst::search(node.get(), 15) != nullptr);
            auto *retNode = bst::search(node.get(), 15);
            REQUIRE(retNode->rightChild->data == 16);
        } 
        {
            REQUIRE(bst::search(node.get(), 7) != nullptr);
            auto *retNode = bst::search(node.get(), 7);
            REQUIRE(retNode->leftChild->data == 6);
        }
    }
}