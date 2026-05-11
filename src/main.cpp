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
#include "DoubleList.h"
#include "BinaryTree.h"
#include "Stack.h"
#include <string>


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
        if constexpr (std::is_same<T, std::string>::value) {
            std::cerr << "BucketSort does not support strings, using ShellSort instead" << std::endl;
            shellSort(arr, size, 0);
        } else {
            bucketSort(arr, size);
        }
    } else {
        std::cerr << "ERROR: No algorithm" << std::endl;
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
    std::cout << "Sorted: " << (sorted ? "yes:)" : "no:(") << std::endl;

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

    if (!resultsFile.empty()) {
        writeCsvHeaderIfNeeded(resultsFile);
    }

    long long minTime = -1;
    long long maxTime = 0;
    long long totalTime = 0;
    int successCount = 0;
    for (int it = 0; it < iterations; it++) {
        long long elapsed = 0;
        bool sorted = false;

        if (structure == Structures::array || structure == Structures::undefined) {
            Array<T> arr(structureSize);
            if (distribution == Distribution::ascending) {
                arr.fillAscending();
            } else if (distribution == Distribution::descending) {
                arr.fillDescending();
            } else if (distribution == Distribution::ascending50Per) {
                arr.fillAscending50();
            } else {
                arr.fillRandom();
            }

            auto start = std::chrono::high_resolution_clock::now();
            sortArray(&arr[0], arr.getSize());
            auto end = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            sorted = arr.isSorted();
        }
        else if (structure == Structures::singleList) {
            SingleList<T> list;
            if (distribution == Distribution::ascending) {
                list.fillAscending(structureSize);
            } else if (distribution == Distribution::descending) {
                list.fillDescending(structureSize);
            } else if (distribution == Distribution::ascending50Per) {
                list.fillAscending50(structureSize);
            } else {
                list.fillRandom(structureSize);
            }

            T* tmpArr = new T[structureSize];
            list.toArray(tmpArr);
            auto start = std::chrono::high_resolution_clock::now();
            sortArray(tmpArr, structureSize);
            auto end = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            list.fromArray(tmpArr, structureSize);
            delete[] tmpArr;
            sorted = list.isSorted();
        }

        else if (structure == Structures::doubleList) {
            DoubleList<T> list;
            if (distribution == Distribution::ascending) {
                list.fillAscending(structureSize);
            } else if (distribution == Distribution::descending) {
                list.fillDescending(structureSize);
            } else if (distribution == Distribution::ascending50Per) {
                list.fillAscending50(structureSize);
            } else {
                list.fillRandom(structureSize);
            }

            T* tmpArr = new T[structureSize];
            list.toArray(tmpArr);

            auto start = std::chrono::high_resolution_clock::now();
            sortArray(tmpArr, structureSize);
            auto end = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            list.fromArray(tmpArr, structureSize);
            delete[] tmpArr;
            sorted = list.isSorted();
        }
        else if (structure == Structures::stack) {
            Stack<T> stack;
            stack.fillRandom(structureSize);

            T* tmpArr = new T[structureSize];
            stack.toArray(tmpArr);

            auto start = std::chrono::high_resolution_clock::now();
            sortArray(tmpArr, structureSize);
            auto end = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            stack.fromArray(tmpArr, structureSize);
            delete[] tmpArr;
            sorted = stack.isSorted();
        }
        else if (structure == Structures::binaryTree) {
            Array<T> tmpData(structureSize);
            tmpData.fillRandom();

            T* tmpArr = new T[structureSize];
            BinaryTree<T> tree;

            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < structureSize; i++) {
                tree.insert(tmpData[i]);
            }
            tree.toArray(tmpArr);
            auto end = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            sorted = tree.isSorted();
            delete[] tmpArr;
        }
        else {
            std::cerr << "ERROR: no structure" << std::endl;
            return;
        }

        // std::cout << "it " << it << ": " << elapsed << " us" << std::endl;
        if (sorted) {
            successCount++;
            totalTime += elapsed;
            if (minTime < 0 || elapsed < minTime) minTime = elapsed;
            if (elapsed > maxTime) maxTime = elapsed;

            if (!resultsFile.empty()) {
                appendCsvRow(resultsFile,
                            algorithmToString(algorithm),
                            structureToString(structure),
                            dataTypeToString(dataType),
                            distributionToString(distribution),
                            structureSize,
                            it + 1,
                            elapsed);
            }
        } else {
            std::cerr << "ERROR: Iteration " << it << " did't sort correctly" << std::endl;
        }
    }


    if (successCount == 0) {
        std::cerr << "ERROR: no successful sorts" << std::endl;
        return;
    }

    long long avgTime = totalTime / successCount;

    std::cout << "Successful sorts: " << successCount << "/" << iterations << std::endl;
    std::cout << "Min time: " << minTime << " us" << std::endl;
    std::cout << "Max time: " << maxTime << " us" << std::endl;
    std::cout << "Avg time: " << avgTime << " us" << std::endl;
    if (!resultsFile.empty()) {
        std::cout << "Results saved to " << resultsFile << std::endl;
    }
}

void runSingleFile() {
    using namespace Parameters;

    if (inputFile.empty()) {
        std::cerr << "ERROR: No input file" << std::endl;
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
        case DataTypes::typeString:
            singleFileForType<std::string>();
            break;
        default:
            std::cerr << "ERROR: Unsupported data type for single file" << std::endl;
            break;

    }
}

void runBenchmark() {
    using namespace Parameters;
    if (structureSize <= 0) {
        std::cerr << "ERROR: structureSize must be > 0 (use -l)" << std::endl;
        return;
    }
    if (iterations <= 0) {
        std::cerr << "ERROR: iterations must be > 0 (use -n)" << std::endl;
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
        case DataTypes::typeString:
            benchmarkForType<std::string>();
            break;
        default:
            std::cerr << "ERROR: Unsupported data type for benchmark" << std::endl;
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
            std::cerr << "ERROR: No mode specified. Use --help for usage." << std::endl;
            return 1;
    }

    return 0;
}