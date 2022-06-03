#pragma once

#include <memory>
#include <string>
#include <iostream>
namespace bst
{
template <typename T>
struct Node
{
    Node(T data, std::unique_ptr<Node<T>> leftChild, std::unique_ptr<Node<T>> rightChild)
    : data{data},
    leftChild{std::move(leftChild)},
    rightChild{std::move(rightChild)}
    {}

    T data;
    std::unique_ptr<Node<T>> leftChild;
    std::unique_ptr<Node<T>> rightChild;
};

template <typename T>
[[nodiscard]] std::unique_ptr<Node<T>> insertNode(std::unique_ptr<Node<T>> node, T data)
{
    if (node == nullptr) {
        return std::make_unique<Node<T>>(data, nullptr, nullptr);
    }
    Node<T>* current = node.get();
    Node<T>* parent = nullptr;
    while (true)
    {
        parent = current;
        if (data < parent->data)
        {
            current = current->leftChild.get();
            if (current == nullptr) {
                parent->leftChild = std::make_unique<Node<T>>(data, nullptr, nullptr);
                return node;
            }
        }
        if (data > parent->data)
        {
            current = current->rightChild.get();
            if (current == nullptr) {
                parent->rightChild = std::make_unique<Node<T>>(data, nullptr, nullptr);
                return node;
            }
        }
        if (data == parent->data) {
            break;
        }
    }
    return node;
}

template <typename T>
[[nodiscard]] Node<T>* search(Node<T> *node, T data)
{
    auto *current = node;

    while(current != nullptr && current->data != data)
    {
        if (current->data > data) {
            current = current->leftChild.get();
        }
        if (current->data < data) {
            current = current->rightChild.get();
        }
        if (current == nullptr) {
            return nullptr;
        }
    }
    return current;
}

template <typename T>
void print(const std::unique_ptr<Node<T>> &node)
{
    print("", node, false);
}

template <typename T>
void print(const std::string& prefix, const std::unique_ptr<Node<T>> &node, bool isLeft)
{
    if (node) 
    {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──" );
        std::cout << node->data << std::endl;
        print((isLeft ? "│   " : "    "), node->leftChild, true);
        print((isLeft ? "│   " : "    "), node->rightChild, false);
    }
}
}
