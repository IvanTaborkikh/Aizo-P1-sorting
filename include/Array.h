#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <random>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <string>


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
        // fill with random values depending on type

        std::random_device rd;
        std::mt19937 gen(rd());
        if constexpr (std::is_same<T, std::string>::value) {
            std::uniform_int_distribution<int> lenDist(1, 10);
            std::uniform_int_distribution<int> charDist(33, 126);
            for (int i = 0; i < size; i++) {
                int len = lenDist(gen);
                std::string s;
                for (int j = 0; j < len; j++) {
                    s += static_cast<char>(charDist(gen));
                }
                data[i] = s;
            }
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(-1000000.0, 1000000.0);
            for (int i = 0; i < size; i++) {
                data[i] = dist(gen);
            }
        } else {
            std::uniform_int_distribution<T> dist(
                std::numeric_limits<T>::min(),
                std::numeric_limits<T>::max()
            );
            for (int i = 0; i < size; i++) {
                data[i] = dist(gen);
            }
        }
    }


    void fillAscending() {
        // generate random data
        std::random_device rd;
        std::mt19937 gen(rd());
        if constexpr (std::is_same<T, std::string>::value) {
            std::uniform_int_distribution<int> lenDist(1, 10);
            std::uniform_int_distribution<int> charDist(33, 126);
            for (int i = 0; i < size; i++) {
                int len = lenDist(gen);
                std::string s;
                for (int j = 0; j < len; j++) {
                    s += static_cast<char>(charDist(gen));
                }
                data[i] = s;
            }
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(-1000000.0, 1000000.0);
            for (int i = 0; i < size; i++) {
                data[i] = dist(gen);
            }
        } else {
            std::uniform_int_distribution<T> dist(
                std::numeric_limits<T>::min(),
                std::numeric_limits<T>::max()
            );
            for (int i = 0; i < size; i++) {
                data[i] = dist(gen);
            }
        }
        // sort ascending
        std::sort(data, data + size);
    }

    void fillDescending() {
        fillAscending();
        // std::cout << data[0] << " " << data[size - 1] << std::endl;
        // reverse the array
        for (int i = 0; i < size / 2; i++) {
            T temp = data[i];
            data[i] = data[size - 1 - i];
            data[size - 1 - i] = temp;
        }
    }


    void fillAscending50() {
        // generate random data
        std::random_device rd;
        std::mt19937 gen(rd());
        if constexpr (std::is_same<T, std::string>::value) {
            std::uniform_int_distribution<int> lenDist(1, 10);
            std::uniform_int_distribution<int> charDist(33, 126);
            for (int i = 0; i < size; i++) {
                int len = lenDist(gen);
                std::string s;
                for (int j = 0; j < len; j++) {
                    s += static_cast<char>(charDist(gen));
                }
                data[i] = s;
            }
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(-1000000.0, 1000000.0);
            for (int i = 0; i < size; i++) {
                data[i] = dist(gen);
            }

        } else {
            std::uniform_int_distribution<T> dist(
                std::numeric_limits<T>::min(),
                std::numeric_limits<T>::max()
            );
            for (int i = 0; i < size; i++) {
                data[i] = dist(gen);
            }
        }
        // sort only first half
        std::sort(data, data + size / 2);
    }

    // check if array is sorted
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
