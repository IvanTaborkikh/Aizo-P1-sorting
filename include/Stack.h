#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <random>
#include <limits>
#include <type_traits>


template <typename T>
class Stack {
private:
    struct Node {
        T value;
        Node* next;
        Node(T val) : value(val), next(nullptr) {}
    };

    Node* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}
    ~Stack() {
        clear();
    }

    void clear() {
        // free all nodes
        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
        size = 0;
    }

    void push(T value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        size++;
    }

    T pop() {
        T value = top->value;
        Node* temp = top;
        top = top->next;
        delete temp;
        size--;
        return value;
    }


    T peek() const {
        return top->value;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getSize() const {
        return size;
    }


    void fillRandom(int count) {
        // fill with random values depending on type
        clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        if constexpr (std::is_same<T, std::string>::value) {
            std::uniform_int_distribution<int> lenDist(1, 10);
            std::uniform_int_distribution<int> charDist(33, 126);
            for (int i = 0; i < count; i++) {
                int len = lenDist(gen);
                std::string s;
                for (int j = 0; j < len; j++) {
                    s += static_cast<char>(charDist(gen));
                }

                push(s);
            }
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(-1000000.0, 1000000.0);
            for (int i = 0; i < count; i++) {
                push(dist(gen));
            }
        } else {
            std::uniform_int_distribution<T> dist(
                std::numeric_limits<T>::min(),
                std::numeric_limits<T>::max()
            );

            for (int i = 0; i < count; i++) {
                push(dist(gen));
            }
        }
    }

    // copy stack to array
    void toArray(T* arr) const {
        Node* current = top;
        int i = size - 1;
        while (current != nullptr) {
            arr[i--] = current->value;
            current = current->next;
        }
    }

    // rebuild stack from array
    void fromArray(const T* arr, int count) {
        clear();
        for (int i = 0; i < count; i++) {
            push(arr[i]);
        }
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
