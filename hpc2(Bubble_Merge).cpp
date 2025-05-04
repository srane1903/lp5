#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace chrono;

// Sequential Bubble Sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
}

// Parallel Bubble Sort
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for
        for (int j = (i % 2); j < n-1; j += 2) {
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }
    }
}

// Merge two parts
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;
    int i = l, j = m+1;
    while (i <= m && j <= r) {
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }
    while (i <= m) temp.push_back(arr[i++]);
    while (j <= r) temp.push_back(arr[j++]);

    for (int k = l; k <= r; k++)
        arr[k] = temp[k-l];
}

// Sequential Merge Sort
void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l+r)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l+r)/2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m+1, r);
        }
        merge(arr, l, m, r);
    }
}

// Print array
void printArray(vector<int> arr) {
    for (int x : arr)
        cout << x << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    cout << "\nOriginal Array:\n";
    printArray(arr);

    vector<int> temp;

    // Sequential Bubble Sort
    temp = arr;
    auto start = high_resolution_clock::now();
    bubbleSort(temp);
    auto end = high_resolution_clock::now();
       cout << "\nSequential Bubble Sort:\n";
    printArray(temp);
    duration<double> time = end - start;
    cout << "Time: " << time.count() << " seconds\n";

    // Parallel Bubble Sort
    temp = arr;
    start = high_resolution_clock::now();
    parallelBubbleSort(temp);
    end = high_resolution_clock::now();
    cout << "\nParallel Bubble Sort:\n";
    printArray(temp);
    time = end - start;
    cout << "Time: " << time.count() << " seconds\n";

    // Sequential Merge Sort
    temp = arr;
    start = high_resolution_clock::now();
    mergeSort(temp, 0, n-1);
    end = high_resolution_clock::now();
    cout << "\nSequential Merge Sort:\n";
    printArray(temp);
    time = end - start;
    cout << "Time: " << time.count() << " seconds\n";

    // Parallel Merge Sort
    temp = arr;
    start = high_resolution_clock::now();
    parallelMergeSort(temp, 0, n-1);
    end = high_resolution_clock::now();
    cout << "\nParallel Merge Sort:\n";
    printArray(temp);
    time = end - start;
    cout << "Time: " << time.count() << " seconds\n";

    return 0;
}

