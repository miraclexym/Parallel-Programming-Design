#include <iostream>
#include <vector>
#include <algorithm>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <Windows.h>

using namespace std;

// CUDA kernel to compute intersection of two sorted lists
__global__ void intersectKernel(int *d_list1, int size1, int *d_list2, int size2, int *d_result) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int i = 0, j = 0;
    int count = 0;

    while (i < size1 && j < size2) {
        if (d_list1[i] == d_list2[j]) {
            d_result[count++] = d_list1[i];
            i++;
            j++;
        } else if (d_list1[i] < d_list2[j]) {
            i++;
        } else {
            j++;
        }
    }
}

// Function to compute intersection of k lists (parallel using CUDA)
vector<int> intersectAllParallel(const vector<vector<int>>& postings) {
    if (postings.empty()) return {};

    // Allocate memory on host and device
    vector<int> result(postings[0]);
    int *d_result;
    cudaMalloc((void **)&d_result, result.size() * sizeof(int));
    
    for (int i = 1; i < postings.size(); ++i) {
        // Prepare data for CUDA
        vector<int> current_posting = postings[i];
        int *d_list1, *d_list2;
        cudaMalloc((void **)&d_list1, result.size() * sizeof(int));
        cudaMalloc((void **)&d_list2, current_posting.size() * sizeof(int));
        cudaMemcpy(d_list1, result.data(), result.size() * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_list2, current_posting.data(), current_posting.size() * sizeof(int), cudaMemcpyHostToDevice);

        // Launch CUDA kernel
        intersectKernel<<<1, 1>>>(d_list1, result.size(), d_list2, current_posting.size(), d_result);
        cudaDeviceSynchronize();

        // Copy result back to host
        cudaMemcpy(result.data(), d_result, result.size() * sizeof(int), cudaMemcpyDeviceToHost);

        // Free device memory
        cudaFree(d_list1);
        cudaFree(d_list2);
    }

    // Free device memory
    cudaFree(d_result);

    return result;
}

int main() {
    // Example usage
    vector<vector<int>> postings = {{1, 3, 5, 7}, {2, 3, 5}, {3, 4, 5, 7}};

    // Measure execution time using Windows API
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    vector<int> intersection = intersectAllParallel(postings);

    QueryPerformanceCounter(&end);
    double elapsed_time = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    // Output results
    cout << "Intersection result: ";
    for (int num : intersection) {
        cout << num << " ";
    }
    cout << endl;
    cout << "Parallel execution time: " << elapsed_time << " milliseconds" << endl;

    return 0;
}