#ifndef BUCKETSORT_H
#define BUCKETSORT_H

#include <cmath>

template <typename T>
void insertionSort(T* arr, int size) {
    for (int i = 1; i < size; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
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
    int* bucketCapacities = new int[bucketCount];

    for (int i = 0; i < bucketCount; i++) {
        bucketCapacities[i] = 8;
        bucketSizes[i] = 0;
        buckets[i] = new T[bucketCapacities[i]];
    }

    long double range = ((long double)maxVal - (long double)minVal) / bucketCount;
    if (range == 0) range = 1;

    for (int i = 0; i < size; i++) {
        int idx = (int)(((long double)arr[i] - (long double)minVal) / range);
        if (idx >= bucketCount) idx = bucketCount - 1;
        if (idx < 0) idx = 0;

        if (bucketSizes[idx] >= bucketCapacities[idx]) {
            int newCap = bucketCapacities[idx] * 2;
            T* newBucket = new T[newCap];
            for (int k = 0; k < bucketSizes[idx]; k++) {
                newBucket[k] = buckets[idx][k];
            }
            delete[] buckets[idx];
            buckets[idx] = newBucket;
            bucketCapacities[idx] = newCap;
        }

        buckets[idx][bucketSizes[idx]] = arr[i];
        bucketSizes[idx]++;
    }

    int pos = 0;
    for (int i = 0; i < bucketCount; i++) {
        insertionSort(buckets[i], bucketSizes[i]);
        for (int j = 0; j < bucketSizes[i]; j++) {
            arr[pos++] = buckets[i][j];
        }
    }

    for (int i = 0; i < bucketCount; i++) {
        delete[] buckets[i];
    }
    delete[] buckets;
    delete[] bucketSizes;
    delete[] bucketCapacities;
}

#endif