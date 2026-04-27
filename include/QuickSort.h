#ifndef QUICKSORT_H
#define QUICKSORT_H

void quickSort(int* arr, int max, int min) {
    if (min >= max) return;
    int i, j, pivot, temp;

    i = min;
    j = max - 1;
    pivot = arr[max];

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
        quickSort(arr, i - 1, min);
    if (max - 1 != i)
        quickSort(arr, max, i + 1);
}

#endif
