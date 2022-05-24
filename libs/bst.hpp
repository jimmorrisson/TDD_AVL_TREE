#pragma once

#include <memory>

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
    }
    return node;
}

template <typename T>
[[nodiscard]] Node<T>* search(Node<T> *node, T data)
{
    auto *current = node;

    while(current->data != data)
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
}
