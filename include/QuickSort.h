#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <random>

template <typename T>
void quickSort(T* arr, int max, int min, int pivotType) {
    if (min >= max) return;

    int pivotIdx = max;
    if (pivotType == 0) {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist(min, max);
        pivotIdx = dist(gen);
    } else if (pivotType == 1) {
        pivotIdx = min;
    } else if (pivotType == 3) {
       
        pivotIdx = min + (max - min) / 2;
    }
    
    if (pivotIdx != max) {
        T tmp = arr[pivotIdx];
        arr[pivotIdx] = arr[max];
        arr[max] = tmp;
    }

    int i = min;
    int j = max - 1;
    T pivot = arr[max];
    T temp;

    while (i != j) {
        if (arr[i] > pivot) {
            temp = arr[i];
            while (i != j) {
                if (arr[j] < pivot) {
                    arr[i] = arr[j];
                    arr[j] = temp;
                    break;
                }
                j--;
            }
            continue;
        }
        i++;
    }

    if (arr[i] > pivot) {
        arr[max] = arr[i];
        arr[i] = pivot;
    } else {
        arr[max] = arr[i + 1];
        arr[i + 1] = pivot;
        i++;
    }

    if (min != i)
        quickSort(arr, i - 1, min, pivotType);
    if (max - 1 != i)
        quickSort(arr, max, i + 1, pivotType);
}

#endif