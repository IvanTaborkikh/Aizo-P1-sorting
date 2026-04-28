#include <iostream>
#include <chrono>
#include "Parameters.h"
#include "Array.h"
#include "SingleList.h"
#include "QuickSort.h"
#include "ShellSort.h"
#include "BucketSort.h"
#include "FileHelper.h"
#include "CsvWriter.h"


template <typename T>
void sortArray(T* arr, int size) {
    using namespace Parameters;
    if (algorithm == Algorithms::quick) {
        int pivotType = 2;
        if (pivot != Pivots::undefined) {
            pivotType = static_cast<int>(pivot);
        }
        quickSort(arr, size - 1, 0, pivotType);
    } else if (algorithm == Algorithms::shell) {
        int formula = (shellParameter == ShellParameters::option2) ? 1 : 0;
        shellSort(arr, size, formula);
    } else if (algorithm == Algorithms::bucket) {
        bucketSort(arr, size);
    } else {
        std::cerr << "ERROR! Algorithm not implemented yet" << std::endl;
    }
}

template <typename T>
bool checkSorted(T* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

template <typename T>
void singleFileForType() {
    using namespace Parameters;

    T* arr = nullptr;
    int size = 0;

    if (!readArrayFromFile<T>(inputFile, arr, size)) {
        return;
    }

    std::cout << "Read " << size << " elements from " << inputFile << std::endl;

    sortArray(arr, size);

    bool sorted = checkSorted(arr, size);
    std::cout << "Sorted: " << (sorted ? "yes" : "no") << std::endl;

    if (!outputFile.empty()) {
        if (writeArrayToFile<T>(outputFile, arr, size)) {
            std::cout << "Saved to " << outputFile << std::endl;
        }
    }

    delete[] arr;
}

template <typename T>
void benchmarkForType() {
    using namespace Parameters;

    std::cout << "Running benchmark: "
              << iterations << " iterations, "
              << structureSize << " elements" << std::endl;

    long long minTime = -1;
    long long maxTime = 0;
    long long totalTime = 0;
    int successCount = 0;

    for (int it = 0; it < iterations; it++) {
        Array<T> arr(structureSize);
        arr.fillRandom();

        auto start = std::chrono::high_resolution_clock::now();
        sortArray(&arr[0], arr.getSize());
        auto end = std::chrono::high_resolution_clock::now();

        long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        if (arr.isSorted()) {
            successCount++;
            totalTime += elapsed;
            if (minTime < 0 || elapsed < minTime) minTime = elapsed;
            if (elapsed > maxTime) maxTime = elapsed;
        } else {
            std::cerr << "ERROR! Iteration " << it << " did not sort correctly" << std::endl;
        }
    }

    if (successCount == 0) {
        std::cerr << "ERROR! No successful sorts" << std::endl;
        return;
    }

    long long avgTime = totalTime / successCount;

    std::cout << "Successful sorts: " << successCount << "/" << iterations << std::endl;
    std::cout << "Min time: " << minTime << " us" << std::endl;
    std::cout << "Max time: " << maxTime << " us" << std::endl;
    std::cout << "Avg time: " << avgTime << " us" << std::endl;

    if (!resultsFile.empty()) {
        appendCsvRow(resultsFile,
                    algorithmToString(algorithm),
                    structureToString(structure),
                    dataTypeToString(dataType),
                    structureSize,
                    iterations,
                    minTime,
                    maxTime,
                    avgTime);
        std::cout << "Results saved to " << resultsFile << std::endl;
    }
}

void runSingleFile() {
    using namespace Parameters;

    if (inputFile.empty()) {
        std::cerr << "ERROR! No input file specified" << std::endl;
        return;
    }

    switch (dataType) {
        case DataTypes::typeInt:
            singleFileForType<int>();
            break;
        case DataTypes::typeFloat:
            singleFileForType<float>();
            break;
        case DataTypes::typeDouble:
            singleFileForType<double>();
            break;
        case DataTypes::typeUnsignedLong:
            singleFileForType<unsigned long>();
            break;
        default:
            std::cerr << "ERROR! Unsupported data type for single file" << std::endl;
            break;
    }
}

void runBenchmark() {
    using namespace Parameters;

    if (structureSize <= 0) {
        std::cerr << "ERROR! structureSize must be > 0 (use -l)" << std::endl;
        return;
    }
    if (iterations <= 0) {
        std::cerr << "ERROR! iterations must be > 0 (use -n)" << std::endl;
        return;
    }

    switch (dataType) {
        case DataTypes::typeInt:
            benchmarkForType<int>();
            break;
        case DataTypes::typeFloat:
            benchmarkForType<float>();
            break;
        case DataTypes::typeDouble:
            benchmarkForType<double>();
            break;
        case DataTypes::typeUnsignedLong:
            benchmarkForType<unsigned long>();
            break;
        default:
            std::cerr << "ERROR! Unsupported data type for benchmark" << std::endl;
            break;
    }
}

int main(int argc, char **argv) {
    if (Parameters::readParameters(argc - 1, argv + 1) != 0) {
        std::cerr << "Failed to read parameters!" << std::endl;
        return 1;
    }

    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            Parameters::help();
            break;
        case Parameters::RunModes::singleFile:
            runSingleFile();
            break;
        case Parameters::RunModes::benchmark:
            runBenchmark();
            break;
        default:
            std::cerr << "ERROR! No mode specified. Use --help for usage." << std::endl;
            return 1;
    }

    return 0;
}