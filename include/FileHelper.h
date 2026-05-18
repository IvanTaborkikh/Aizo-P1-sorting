#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <fstream>
#include <iostream>
#include <string>


// read array from file, first line is size, then elements
template <typename T>
bool readArrayFromFile(const std::string& filename, T*& arr, int& size) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open the file: " << filename << std::endl;
        return false;
    }

    // read size from first line
    file >> size;
    if (size <= 0) {
        std::cerr << "ERROR: Invalid size in the file: " << size << std::endl;
        file.close();
        return false;
    }

    // allocate memory and read elements
    arr = new T[size];
    for (int i = 0; i < size; i++) {
        if (!(file >> arr[i])) {
            std::cerr << "ERROR: Failed to read element: " << i << std::endl;
            delete[] arr;
            arr = nullptr;
            file.close();
            return false;
        }
    }


    file.close();
    return true;
}

// write array to file, first line is size, then elements
template <typename T>
bool writeArrayToFile(const std::string& filename, T* arr, int size) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open the file to write in: " << filename << std::endl;
        return false;
    }

    // write size then each element in new line
    file << size << "\n";
    for (int i = 0; i < size; i++) {
        file << arr[i] << "\n";
    }
    file.close();
    return true;
}

#endif