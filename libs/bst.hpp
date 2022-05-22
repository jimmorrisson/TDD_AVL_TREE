#pragma once

namespace bst
{
template <typename T>
struct Node
{
    T data;
    Node* leftChild;
    Node* rightChild;
};

template <typename T>
Node<T>* insertNode(Node<T> *node, T data)
{
    if (node == nullptr) {
        return new Node<T>{data, nullptr, nullptr};
    }
    Node<T>* current = node;
    Node<T>* parent = nullptr;
    while (true)
    {
        parent = current;
        if (data < parent->data)
        {
            current = current->leftChild;
            if (current == nullptr) {
                parent->leftChild = new Node<T>{data, nullptr, nullptr};
                return node;
            }
        }
        if (data > parent->data)
        {
            current = current->rightChild;
            if (current == nullptr) {
                parent->rightChild = new Node<T>{data, nullptr, nullptr};
                return node;
            }
        }
    }
    return node;
}

template <typename T>
Node<T>* search(Node<T> *node, T data)
{
    auto *current = node;

    while(current->data != data)
    {
        if (current->data > data) {
            current = current->leftChild;
        }
        if (current->data < data) {
            current = current->rightChild;
        }
        if (current == nullptr) {
            return nullptr;
        }
    }
    return current;
}
}
