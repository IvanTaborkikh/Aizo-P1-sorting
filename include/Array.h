#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <random>
#include <limits>

template <typename T>
class Array {
private:
    T* data;
    int size;

public:
    explicit Array(int size) : size(size) {
        data = new T[size];
    }

    ~Array() {
        delete[] data;
    }

    T& operator[](int index) {
        return data[index];
    }

    int getSize() const {
        return size;
    }

    void fillRandom() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<T> dist(
            std::numeric_limits<T>::min(),
            std::numeric_limits<T>::max()
        );
        for (int i = 0; i < size; i++) {
            data[i] = dist(gen);
        }
    }

    bool isSorted() const {
        for (int i = 0; i < size - 1; i++) {
            if (data[i] > data[i + 1]) return false;
        }
        return true;
    }

    void print() const {
        for (int i = 0; i < size; i++) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

#endif
