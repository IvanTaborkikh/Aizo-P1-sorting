#ifndef DOUBLELIST_H
#define DOUBLELIST_H

#include <iostream>
#include <random>
#include <limits>
#include <type_traits>
#include <algorithm>

template <typename T>
class DoubleList {
private:
    struct Node {
        T value;
        Node* next;
        Node* prev;
        Node(T val) : value(val), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    DoubleList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoubleList() {
        clear();
    }


    void clear() {
        // free all nodes
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void pushBack(T value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
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
                pushBack(s);
            }
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(-1000000.0, 1000000.0);
            for (int i = 0; i < count; i++) {
                pushBack(dist(gen));
            }
        } else {
            std::uniform_int_distribution<T> dist(
                std::numeric_limits<T>::min(),
                std::numeric_limits<T>::max()
            );
            for (int i = 0; i < count; i++) {
                pushBack(dist(gen));
            }

        }
    }

    void fillAscending(int count) {
        clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        // generate random data
        T* tmp = new T[count];
        if constexpr (std::is_same<T, std::string>::value) {
            std::uniform_int_distribution<int> lenDist(1, 10);
            std::uniform_int_distribution<int> charDist(33, 126);
            for (int i = 0; i < count; i++) {
                int len = lenDist(gen);
                std::string s;
                for (int j = 0; j < len; j++) s += static_cast<char>(charDist(gen));
                tmp[i] = s;
            }
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(-1000000.0, 1000000.0);
            for (int i = 0; i < count; i++) tmp[i] = dist(gen);
        } else {
            std::uniform_int_distribution<T> dist(
                std::numeric_limits<T>::min(),
                std::numeric_limits<T>::max()
            );
            for (int i = 0; i < count; i++) tmp[i] = dist(gen);
        }

        // sort ascending
        std::sort(tmp, tmp + count);
        // load back to list
        for (int i = 0; i < count; i++) pushBack(tmp[i]);
        delete[] tmp;
    }

    void fillDescending(int count) {
        clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        // generate random data
        T* tmp = new T[count];
        if constexpr (std::is_same<T, std::string>::value) {
            std::uniform_int_distribution<int> lenDist(1, 10);
            std::uniform_int_distribution<int> charDist(33, 126);
            for (int i = 0; i < count; i++) {
                int len = lenDist(gen);
                std::string s;
                for (int j = 0; j < len; j++) s += static_cast<char>(charDist(gen));
                tmp[i] = s;
            }
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(-1000000.0, 1000000.0);
            for (int i = 0; i < count; i++) tmp[i] = dist(gen);
        } else {
            std::uniform_int_distribution<T> dist(
                std::numeric_limits<T>::min(),
                std::numeric_limits<T>::max()
            );
            for (int i = 0; i < count; i++) tmp[i] = dist(gen);
        }
        // sort then reverse
        std::sort(tmp, tmp + count);
        for (int i = 0; i < count / 2; i++) {
            T temp = tmp[i];
            tmp[i] = tmp[count - 1 - i];
            tmp[count - 1 - i] = temp;
        }
        // load back to list
        for (int i = 0; i < count; i++) pushBack(tmp[i]);
        delete[] tmp;
    }


    void fillAscending50(int count) {
        clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        // generate random data
        T* tmp = new T[count];
        if constexpr (std::is_same<T, std::string>::value) {
            std::uniform_int_distribution<int> lenDist(1, 10);
            std::uniform_int_distribution<int> charDist(33, 126);
            for (int i = 0; i < count; i++) {
                int len = lenDist(gen);
                std::string s;
                for (int j = 0; j < len; j++) s += static_cast<char>(charDist(gen));
                tmp[i] = s;
            }
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(-1000000.0, 1000000.0);
            for (int i = 0; i < count; i++) tmp[i] = dist(gen);
        } else {
            std::uniform_int_distribution<T> dist(
                std::numeric_limits<T>::min(),
                std::numeric_limits<T>::max()
            );
            for (int i = 0; i < count; i++) tmp[i] = dist(gen);
        }
        // sort only first half
        std::sort(tmp, tmp + count / 2);
        // load back to list
        for (int i = 0; i < count; i++) pushBack(tmp[i]);
        delete[] tmp;
    }

    
    // copy list to array
    void toArray(T* arr) const {
        Node* current = head;
        int i = 0;
        while (current != nullptr) {
            arr[i++] = current->value;
            current = current->next;
        }
    }

    // rebuild list from array
    void fromArray(const T* arr, int count) {
        clear();
        for (int i = 0; i < count; i++) {
            pushBack(arr[i]);
        }
    }

    // check if list is sorted
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
