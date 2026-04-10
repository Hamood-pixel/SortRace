#pragma once

#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <algorithm>
#include <random>
#include <numeric>

// --- Threading and Synchronization ---
extern std::mutex g_array_mutex;

// Custom Windows messages for cross-thread communication
#define WM_REDRAW_CANVAS (WM_USER + 100)
#define WM_RACE_FINISHED (WM_USER + 200)

// --- Timer Utility ---
class HighPrecisionTimer {
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    void Start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    // Returns duration in milliseconds (ms)
    long long Stop() {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    }
};

// --- Data Structure and Utilities ---

struct SortData {
    std::vector<int> arr;
    CWnd* pCanvasWnd = nullptr; // Used to determine if visualization/sleep should occur
    int active_index1 = -1;
    int active_index2 = -1;
};

std::vector<int> GenerateRandomArray(int size);

// --- Merge Sort Functions ---
const int THREAD_CUTOFF = 10000;

void Merge(SortData& data, int l, int m, int r);
void SequentialMergeSort(SortData& data, int l, int r);
void ParallelMergeSort(SortData& data, int l, int r);