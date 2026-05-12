#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <random>

template <typename T>
void quickSort(T* arr, int max, int min, int pivotType) {
    if (min >= max) return;

    // using stack instead of recursion, was getting segfault on large sorted arrays
    int stackSize = max - min + 1;
    int* lo = new int[stackSize];
    int* hi = new int[stackSize];
    int top = 0;
    lo[top] = min;
    hi[top] = max;
    top++;

    std::mt19937 gen(std::random_device{}());

    while (top > 0) {
        top--;
        int curMin = lo[top];
        int curMax = hi[top];

        if (curMin >= curMax) continue;
        
        // default pivot is rightmost element
        int pivotIdx = curMax;
        if (pivotType == 0) {
            std::uniform_int_distribution<int> dist(curMin, curMax);
            pivotIdx = dist(gen);
        } else if (pivotType == 1) {
            pivotIdx = curMin;
        } else if (pivotType == 3) {
            pivotIdx = curMin + (curMax - curMin) / 2;
        }


        if (pivotIdx != curMax) {
            T tmp = arr[pivotIdx];
            arr[pivotIdx] = arr[curMax];
            arr[curMax] = tmp;
        }

        int i = curMin;
        int j = curMax - 1;
        T pivot = arr[curMax];
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
            arr[curMax] = arr[i];
            arr[i] = pivot;
        } else {
            arr[curMax] = arr[i + 1];
            arr[i + 1] = pivot;
            i++;
        }

        // push left and right parts to stack
        if (curMin < i - 1) {
            lo[top] = curMin;
            hi[top] = i - 1;
            top++;
        }
        if (i + 1 < curMax) {
            lo[top] = i + 1;
            hi[top] = curMax;
            top++;
        }
    }


    delete[] lo;
    delete[] hi;
}

#endif
