#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include "Parameters.h"

inline std::string algorithmToString(Parameters::Algorithms algo) {
    switch (algo) {
        case Parameters::Algorithms::bubble: return "bubble";
        case Parameters::Algorithms::cocktail: return "cocktail";
        case Parameters::Algorithms::merge: return "merge";
        case Parameters::Algorithms::insertion: return "insertion";
        case Parameters::Algorithms::bucket: return "bucket";
        case Parameters::Algorithms::quick: return "quick";
        case Parameters::Algorithms::shell: return "shell";
        default: return "unknown";
    }
}

inline std::string structureToString(Parameters::Structures s) {
    switch (s) {
        case Parameters::Structures::array: return "array";
        case Parameters::Structures::singleList: return "singleList";
        case Parameters::Structures::doubleList: return "doubleList";
        case Parameters::Structures::queue: return "queue";
        case Parameters::Structures::stack: return "stack";
        case Parameters::Structures::binaryTree: return "binaryTree";
        default: return "unknown";
    }
}

inline std::string dataTypeToString(Parameters::DataTypes t) {
    switch (t) {
        case Parameters::DataTypes::typeInt: return "int";
        case Parameters::DataTypes::typeFloat: return "float";
        case Parameters::DataTypes::typeDouble: return "double";
        case Parameters::DataTypes::typeChar: return "char";
        case Parameters::DataTypes::typeString: return "string";
        case Parameters::DataTypes::typeUnsignedLong: return "unsignedLong";
        case Parameters::DataTypes::typeUnsignedChar: return "unsignedChar";
        default: return "unknown";
    }
}

inline std::string getCurrentDateTime() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
    return std::string(buf);
}


// TODO: add more columns later
inline void appendCsvRow(const std::string& filename,
                        const std::string& algorithm,
                        const std::string& structure,
                        const std::string& dataType,
                        int size,
                        int iterations,
                        long long minTime,
                        long long maxTime,
                        long long avgTime) {
    std::ifstream check(filename);
    bool exists = check.good();
    check.close();

    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "ERROR! Could not open results file: " << filename << std::endl;
        return;
    }

    if (!exists) {
        file << "date,algorithm,structure,dataType,size,iterations,min_us,max_us,avg_us\n";
    }

    file << getCurrentDateTime() << ","
         << algorithm << ","
         << structure << ","
         << dataType << ","
         << size << ","
         << iterations << ","
         << minTime << ","
         << maxTime << ","
         << avgTime << "\n";

    file.close();
}

#endif