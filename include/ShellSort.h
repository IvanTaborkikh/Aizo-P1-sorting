#ifndef SHELLSORT_H
#define SHELLSORT_H

template <typename T>
void shellSort(T* arr, int size, int formula) {
    if (formula == 0) {
        for (int gap = size / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < size; i++) {
                T temp = arr[i];
                int j = i;
                while (j >= gap && arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                arr[j] = temp;
            }
        }
    } else {
        int gap = 1;
        while (gap < size / 3) {
            gap = 3 * gap + 1;
        }
        while (gap > 0) {
            for (int i = gap; i < size; i++) {
                T temp = arr[i];
                int j = i;
                while (j >= gap && arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                arr[j] = temp;
            }
            gap = (gap - 1) / 3;
        }
    }
}

#endif