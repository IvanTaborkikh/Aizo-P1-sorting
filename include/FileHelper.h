#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <fstream>
#include <iostream>
#include <string>

template <typename T>
bool readArrayFromFile(const std::string& filename, T*& arr, int& size) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR! Could not open file: " << filename << std::endl;
        return false;
    }

    file >> size;
    if (size <= 0) {
        std::cerr << "ERROR! Invalid size in file: " << size << std::endl;
        file.close();
        return false;
    }

    arr = new T[size];
    for (int i = 0; i < size; i++) {
        if (!(file >> arr[i])) {
            std::cerr << "ERROR! Failed to read element " << i << std::endl;
            delete[] arr;
            arr = nullptr;
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

template <typename T>
bool writeArrayToFile(const std::string& filename, T* arr, int size) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR! Could not open file for writing: " << filename << std::endl;
        return false;
    }

    file << size << "\n";
    for (int i = 0; i < size; i++) {
        file << arr[i] << "\n";
    }

    file.close();
    return true;
}

#endif