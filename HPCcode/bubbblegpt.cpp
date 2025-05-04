#include <iostream>
#include <omp.h>
using namespace std;

// Sequential Bubble Sort
void sequentialBubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
//statics for equal load in tread
// Parallel Bubble Sort using Odd-Even Transposition
void parallelBubbleSort(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            #pragma omp parallel for schedule(static)
          //  #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        } else {
             #pragma omp parallel for schedule(static)
           // #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

// Print array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    const int n = 25000;
    int arr1[n], arr2[n];
   
    // Seed and fill the arrays with random values
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        int num = rand() % 100000;
        arr1[i] = num;
        arr2[i] = num;
    }
  //  printArray(arr1,n);

    double start, end;

    // Sequential Sort
    start = omp_get_wtime();
    sequentialBubbleSort(arr1, n);
    end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << (end - start) * 1e6 << " microseconds\n";

    // Parallel Sort
    start = omp_get_wtime();
    parallelBubbleSort(arr2, n);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time:   " << (end - start) * 1e6 << " microseconds\n";

    return 0;
}