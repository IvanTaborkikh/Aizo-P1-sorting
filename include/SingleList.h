#ifndef SINGLELIST_H
#define SINGLELIST_H

#include <iostream>
#include <random>
#include <limits>

template <typename T>
class SingleList {
private:
    struct Node {
        T value;
        Node* next;
        Node(T val) : value(val), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    SingleList() : head(nullptr), size(0) {}

    ~SingleList() {
        clear();
    }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;
    }

    void pushBack(T value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    T& operator[](int index) {
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->value;
    }

    int getSize() const {
        return size;
    }

    void fillRandom(int count) {
        clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<T> dist(
            std::numeric_limits<T>::min(),
            std::numeric_limits<T>::max()
        );
        for (int i = 0; i < count; i++) {
            pushBack(dist(gen));
        }
    }

    bool isSorted() const {
        if (head == nullptr || head->next == nullptr) return true;
        Node* current = head;
        while (current->next != nullptr) {
            if (current->value > current->next->value) return false;
            current = current->next;
        }
        return true;
    }

    void print() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif