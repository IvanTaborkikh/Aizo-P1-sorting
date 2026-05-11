#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <random>
#include <limits>
#include <type_traits>

template <typename T>
class BinaryTree {
private:
    struct Node {
        T value;
        Node* left;
        Node* right;
        Node(T val) : value(val), left(nullptr), right(nullptr) {}
    };


    Node* root;
    int size;

    // recursive insert
    Node* insert(Node* node, T value) {
        if (node == nullptr) {
            size++;
            return new Node(value);
        }
        if (value < node->value) {
            node->left = insert(node->left, value);
        } else {
            node->right = insert(node->right, value);
        }
        return node;
    }

    // in-order traversal gives sorted result

    void inOrder(Node* node, T* arr, int& idx) const {
        if (node == nullptr) return;
        inOrder(node->left, arr, idx);
        arr[idx++] = node->value;
        inOrder(node->right, arr, idx);
    }

    // free nodes recursively
    void deleteTree(Node* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    BinaryTree() : root(nullptr), size(0) {}

    ~BinaryTree() {
        deleteTree(root);
    }


    void clear() {
        deleteTree(root);
        root = nullptr;
        size = 0;
    }

    void insert(T value) {
        root = insert(root, value);
    }

    int getSize() const {
        return size;
    }

    // in-order traversal
    void toArray(T* arr) const {
        int idx = 0;
        inOrder(root, arr, idx);
    }

    // check if sorted
    bool isSorted() const {
        T* arr = new T[size];
        toArray(arr);
        bool sorted = true;
        for (int i = 0; i < size - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                sorted = false;
                break;
            }
        }

        delete[] arr;
        return sorted;
    }

    void print() const {
        T* arr = new T[size];
        toArray(arr);
        for (int i = 0; i < size; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
        delete[] arr;
        
    }
};

#endif
