#ifndef BUCKETSORT_H
#define BUCKETSORT_H

#include <cmath>
#include <string>


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

// sort only strings
void bucketSortString(std::string* arr, int size) {
    if (size <= 1) return;

    int ileBucketow = 128;

    std::string** buckets = new std::string*[ileBucketow];
    int* rozmiary = new int[ileBucketow];
    int* pojemnosci = new int[ileBucketow];

    for (int i = 0; i < ileBucketow; i++) {
        rozmiary[i] = 0;
        pojemnosci[i] = 4;
        buckets[i] = new std::string[4];
    }

    for (int i = 0; i < size; i++) {
        int idx = 0;
        if (!arr[i].empty()) {
            idx = (unsigned char)arr[i][0];
        }
        if (idx >= ileBucketow) idx = ileBucketow - 1;

        // resize bucket if full
        if (rozmiary[idx] >= pojemnosci[idx]) {
            int nowaPojemnosc = pojemnosci[idx] * 2;
            std::string* nowyBucket = new std::string[nowaPojemnosc];
            for (int j = 0; j < rozmiary[idx]; j++) {
                nowyBucket[j] = buckets[idx][j];
            }
            delete[] buckets[idx];
            buckets[idx] = nowyBucket;
            pojemnosci[idx] = nowaPojemnosc;
        }

        buckets[idx][rozmiary[idx]] = arr[i];
        rozmiary[idx]++;
    }

    int pozycja = 0;
    for (int i = 0; i < ileBucketow; i++) {
        if (rozmiary[i] == 0) continue;
        insertionSort(buckets[i], rozmiary[i]);
        for (int j = 0; j < rozmiary[i]; j++) {
            arr[pozycja] = buckets[i][j];
            pozycja++;
        }
    }

    for (int i = 0; i < ileBucketow; i++) {
        delete[] buckets[i];
    }
    delete[] buckets;
    delete[] rozmiary;
    delete[] pojemnosci;
}

 // sort everything except strings
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
