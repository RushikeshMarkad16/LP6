#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Bubble Sort algorithm
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;

        #pragma omp parallel for shared(arr, swapped)
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) {
            // If no swap occurred, the array is already sorted
            break;
        }
    }
}

// Merge two sorted subarrays
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> leftArr(n1), rightArr(n2);

    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }

    for (int j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// Merge Sort algorithm
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergeSort(arr, left, mid);
            }

            #pragma omp section
            {
                mergeSort(arr, mid + 1, right);
            }
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    // Example usage
    vector<int> arr = {5, 1, 4, 2, 8, 9, 3, 6, 7};

    cout << "Original array: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    // Parallel Bubble Sort
    bubbleSort(arr);

    cout << "Sorted array (Bubble Sort): ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    // Generate a new array for Merge Sort
    vector<int> arrMerge = {5, 1, 4, 2, 8, 9, 3, 6, 7};

    cout << "Original array: ";
    for (int num : arrMerge) {
        cout << num << " ";
    }
    cout << endl;

    // Parallel Merge Sort
    mergeSort(arrMerge, 0, arrMerge.size() - 1);

    cout << "Sorted array (Merge Sort): ";
    for (int num : arrMerge) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
