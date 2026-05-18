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


// sort array using algorithm selected in parameters
template <typename T>
void sortArray(T* arr, int size) {
    using namespace Parameters;
    if (algorithm == Algorithms::quick) {
        // default pivot is right (2) if not specified
        int pivotType = 2;
        if (pivot != Pivots::undefined) {
            pivotType = static_cast<int>(pivot);
        }
        quickSort(arr, size - 1, 0, pivotType);
    } else if (algorithm == Algorithms::shell) {
        int formula = (shellParameter == ShellParameters::option2) ? 1 : 0;
        shellSort(arr, size, formula);
    } else if (algorithm == Algorithms::bucket) {
        // strings need a separate bucket sort implementation
        if constexpr (std::is_same<T, std::string>::value) {
            bucketSortString(arr, size);
        } else {
            bucketSort(arr, size);
        }
    } else {
        std::cerr << "ERROR: No algorithm" << std::endl;
    }
}

// check if array is sorted ascending
template <typename T>
bool checkSorted(T* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}


// read array from file, sort it, optionally save result to output file
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

    // save sorted array to output file if path was given
    if (!outputFile.empty()) {
        if (writeArrayToFile<T>(outputFile, arr, size)) {
            std::cout << "Saved to " << outputFile << std::endl;
        }
    }

    delete[] arr;
}

// run sorting benchmark for given type, measures time for each iteration
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
            // fill array with selected distribution
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
            // fill list with selected distribution
            if (distribution == Distribution::ascending) {
                list.fillAscending(structureSize);
            } else if (distribution == Distribution::descending) {
                list.fillDescending(structureSize);
            } else if (distribution == Distribution::ascending50Per) {
                list.fillAscending50(structureSize);
            } else {
                list.fillRandom(structureSize);
            }

            // copy to temp array for sorting, then copy back
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
            // fill list with selected distribution
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

            // inserting into BST sorts the data, measure only that
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

            // save result row to csv if file was given
            if (!resultsFile.empty()) {
                appendCsvRow(resultsFile,
                            algorithmToString(algorithm),
                            structureToString(structure),
                            dataTypeToString(dataType),
                            distributionToString(distribution),
                            structureSize,
                            std::to_string(it + 1),
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

    // calculate average from successful iterations only
    long long avgTime = totalTime / successCount;

    std::cout << "Successful sorts: " << successCount << "/" << iterations << std::endl;
    std::cout << "Min time: " << minTime << " us" << std::endl;
    std::cout << "Max time: " << maxTime << " us" << std::endl;
    std::cout << "Avg time: " << avgTime << " us" << std::endl;

    // save summary rows to csv
    if (!resultsFile.empty()) {
        std::string algo = algorithmToString(algorithm);
        std::string str  = structureToString(structure);
        std::string dtype = dataTypeToString(dataType);
        std::string dist  = distributionToString(distribution);
        appendCsvRow(resultsFile, algo, str, dtype, dist, structureSize, "avg", avgTime);
        appendCsvRow(resultsFile, algo, str, dtype, dist, structureSize, "min", minTime);
        appendCsvRow(resultsFile, algo, str, dtype, dist, structureSize, "max", maxTime);
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

void help()
{
  std::cout << "Options:\n";
  std::cout << "\n";
  std::cout << "Run modes (mutually exclusive):\n";
  std::cout << "  -f, --singleFile   Test a single file\n";
  std::cout << "  -b, --benchmark    Benchmark test (random values every iteration)\n";
  std::cout << "  -h, --help         Show help message\n";
  std::cout << "\n";
  std::cout << "Common options: \n";
  std::cout << "  -a, --algorithm VAL   4 - Bucket sort\n";
  std::cout << "                        5 - Quick sort\n";
  std::cout << "                        6 - Shell sort\n";
  std::cout << "  -s, --structure VAL   0 - Array\n";
  std::cout << "                        1 - Single linked list\n";
  std::cout << "                        2 - Double linked list\n";
  std::cout << "                        4 - Stack\n";
  std::cout << "                        5 - Binary tree\n";
  std::cout << "  -t, --dataType VAL    0 - int\n";
  std::cout << "                        1 - float\n";
  std::cout << "                        4 - std::string\n";
  std::cout << "                        6 - unsigned long\n";
  std::cout << "  -p, --pivot VAL       0 - random\n";
  std::cout << "                        1 - left\n";
  std::cout << "                        2 - right\n";
  std::cout << "                        3 - middle\n";
  std::cout << "  -e, --shellParameter VAL  0 - option1 (gap = gap/2)\n";
  std::cout << "                            1 - option2 (Knuth (gap = 3*gap + 1))\n";
  std::cout << "\n";
  std::cout << "Single file options:\n";
  std::cout << "  -i, --inputFile FILE    File with values to sort\n";
  std::cout << "  -o, --outputFile FILE   Sorted values will be saved  to this file\n";
  std::cout << "\n";
  std::cout << "Benchmark options:\n";
  std::cout << "  -r, --resultsFile FILE   Results (time and parameters) will be saved to this file\n";
  std::cout << "  -d, --distribution VAL   0 - random\n";
  std::cout << "                           1 - ascending\n";
  std::cout << "                           2 - sorted (ascending) in 50%\n";
  std::cout << "                           3 - descending\n";
  std::cout << "  -l, --structureSize LEN  How many elements in the structure.\n";
  std::cout << "  -n, --iterations ITE :)  How many repetitions of the research with the given parameters.\n";
  std::cout << "Examples:\n";
  std::cout << "  ./project --singleFile --inputFile \"in.txt\" --outputFile \"out.txt\" -a 0 -s 1 -t 0\n";
  std::cout << "  Sort values from file \"in.txt\". Bubble sort, single linked list, integers. Save sorted values in \"out.txt\"\n";
  std::cout << "  ./project --benchmark -a 4 -p 1 -s 1 -t 4 -d 3 -l 10000 -n 50 -r \"res.txt\"\n";
  std::cout << "  Repeat following test 50 times: Linked list of 10000 random unsigned int elements initially sorted in descending order. Use quicksort with the left pivot. Results save in \"res.txt\"\n";
  std::cout << "\n";
}


int main(int argc, char **argv) {
    if (Parameters::readParameters(argc - 1, argv + 1) != 0) {
        std::cerr << "Failed to read parameters!" << std::endl;
        return 1;
    }

    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            help();
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