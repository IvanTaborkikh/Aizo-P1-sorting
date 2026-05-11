#ifndef BUCKETSORT_H
#define BUCKETSORT_H

#include <cmath>


// used to sort elements inside each bucket
template <typename T>
void insertionSort(T* arr, int size) {
    for (int i = 1; i < size; i++) {
        T klucz = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > klucz) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = klucz;
    }
}

template <typename T>
void bucketSort(T* arr, int size) {
    if (size <= 1) return;

    T minVal = arr[0];
    T maxVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    int bucketCount = (int)std::sqrt((double)size);
    if (bucketCount < 1) bucketCount = 1;

    T** buckets = new T*[bucketCount];
    int* bucketSizes = new int[bucketCount];


    for (int i = 0; i < bucketCount; i++) {
        bucketSizes[i] = 0;
        buckets[i] = new T[size];
    }

    long double range = ((long double)maxVal - (long double)minVal) / bucketCount;
    if (range == 0) range = 1;

    // put each element into the right bucket
    for (int i = 0; i < size; i++) {
        int idx = (int)(((long double)arr[i] - (long double)minVal) / range);
        if (idx >= bucketCount) idx = bucketCount - 1;
        if (idx < 0) idx = 0;

        buckets[idx][bucketSizes[idx]] = arr[i];
        bucketSizes[idx]++;
    }

    int pozycja = 0;
    for (int i = 0; i < bucketCount; i++) {
        // std::cout << "bucket " << i << " size: " << bucketSizes[i] << std::endl;
        insertionSort(buckets[i], bucketSizes[i]);
        for (int j = 0; j < bucketSizes[i]; j++) {
            arr[pozycja++] = buckets[i][j];
        }

    }

    for (int i = 0; i < bucketCount; i++) {
        delete[] buckets[i];
    }
    delete[] buckets;
    delete[] bucketSizes;
    
}

#endif