#include <iostream>
#include "Parameters.h"
#include "Array.h"
#include "QuickSort.h"

int main(int argc, char **argv) {
    Parameters::readParameters(argc, argv);

    Array<int> arr(8);
    arr.fillRandom();

    std::cout << "Before quicksort: ";
    arr.print();

    quickSort(&arr[0], arr.getSize() - 1, 0);

    std::cout << "After quicksort: ";
    arr.print();

    std::cout << "Is sorted: " << (arr.isSorted() ? "yes" : "no") << std::endl;

    return 0;
}
