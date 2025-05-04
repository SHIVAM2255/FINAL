#include <iostream>
#include <omp.h>
#include <cstdlib>
using namespace std;

void merge(int arr[], int low, int mid, int high) {
	int n1 = mid - low + 1;
	int n2 = high - mid;

	int* left = new int[n1];
	int* right = new int[n2];

	for (int i = 0; i < n1; i++) left[i] = arr[low + i];
	for (int j = 0; j < n2; j++) right[j] = arr[mid + 1 + j];

	int i = 0, j = 0, k = low;
	while (i < n1 && j < n2) {
		if (left[i] <= right[j]) arr[k++] = left[i++];
		else arr[k++] = right[j++];
	}

	while (i < n1) arr[k++] = left[i++];
	while (j < n2) arr[k++] = right[j++];

	delete[] left;
	delete[] right;
}

// Threshold to switch to sequential sorting
const int THRESHOLD = 10000;
// If the subarray is smaller than this, we don't use parallelism Thr
void parallelMergeSort(int arr[], int low, int high, int depth = 0) {
	if (low < high) {
		int mid = (low + high) / 2;

		if ((high - low) < THRESHOLD || depth > 4) {
			// Switch to sequential for small arrays or deep recursion			
			parallelMergeSort(arr, low, mid, depth + 1);
			parallelMergeSort(arr, mid + 1, high, depth + 1);
		} else {
			#pragma omp parallel sections
			{
				#pragma omp section
				parallelMergeSort(arr, low, mid, depth + 1);
				#pragma omp section
				parallelMergeSort(arr, mid + 1, high, depth + 1);
			}
		}
		merge(arr, low, mid, high);
	}
}

void mergeSort(int arr[], int low, int high) {
	if (low < high) {
		int mid = (low + high) / 2;
		mergeSort(arr, low, mid);
		mergeSort(arr, mid + 1, high);
		merge(arr, low, mid, high);
	}
}

int main() {
	int n = 907800;
	int* arr1 = new int[n];
	int* arr2 = new int[n];

	for (int i = 0, j = n; i < n; i++, j--) {
		arr1[i] = j;
		arr2[i] = j;
	}

	double start_time, end_time;

	// Sequential Sort
	start_time = omp_get_wtime();
	mergeSort(arr1, 0, n - 1);
	end_time = omp_get_wtime();
	cout << "Time taken by sequential algorithm: " << end_time - start_time << " seconds\n";

	// Parallel Sort
	start_time = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp single
		parallelMergeSort(arr2, 0, n - 1);
	}
	end_time = omp_get_wtime();
	cout << "Time taken by parallel algorithm: " << end_time - start_time << " seconds\n";

	delete[] arr1;
	delete[] arr2;
	return 0;
}
