#include "pch.h"
#include "SortLogic.h"
#include <thread>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>

// Global mutex for thread-safe UI updates
std::mutex g_array_mutex;

// --- Helper: Generate Random Data ---
std::vector<int> GenerateRandomArray(int size) {
    std::vector<int> arr(size);
    std::iota(arr.begin(), arr.end(), 1);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(arr.begin(), arr.end(), g);
    return arr;
}

// --- Core Merge Logic ---
void Merge(SortData& data, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = data.arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = data.arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    // Standard visual check: 150 to 2000 is for seeing bars move
    bool is_visual = (data.arr.size() >= 150 && data.arr.size() < 2000);

    while (i < n1 && j < n2) {
        {
            std::lock_guard<std::mutex> lock(g_array_mutex);
            if (L[i] <= R[j]) { data.arr[k] = L[i]; i++; }
            else { data.arr[k] = R[j]; j++; }

            if (data.pCanvasWnd) {
                data.active_index1 = k;
                data.pCanvasWnd->PostMessage(WM_REDRAW_CANVAS);
            }
        }
        if (is_visual) std::this_thread::sleep_for(std::chrono::microseconds(500));
        k++;
    }

    while (i < n1) {
        { std::lock_guard<std::mutex> lock(g_array_mutex); data.arr[k] = L[i]; }
        if (is_visual) std::this_thread::sleep_for(std::chrono::microseconds(500));
        i++; k++;
    }

    while (j < n2) {
        { std::lock_guard<std::mutex> lock(g_array_mutex); data.arr[k] = R[j]; }
        if (is_visual) std::this_thread::sleep_for(std::chrono::microseconds(500));
        j++; k++;
    }
}

// --- Sequential Sort (Uses 1 Thread) ---
void SequentialMergeSort(SortData& data, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    SequentialMergeSort(data, l, m);
    SequentialMergeSort(data, m + 1, r);
    Merge(data, l, m, r);
}

// --- Parallel Sort (Uses 2 Threads) ---
void ParallelMergeSort(SortData& data, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;

    // At the very first call, split into two threads
    if (l == 0 && r == (int)data.arr.size() - 1) {
        // Create one worker thread for the left half
        std::thread t1(SequentialMergeSort, std::ref(data), l, m);

        // Main thread handles the right half
        SequentialMergeSort(data, m + 1, r);

        t1.join(); // Wait for the worker to finish
        Merge(data, l, m, r);
    }
    else {
        // Standard recursive calls
        SequentialMergeSort(data, l, m);
        SequentialMergeSort(data, m + 1, r);
        Merge(data, l, m, r);
    }
}