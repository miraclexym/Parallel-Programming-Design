#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <mpi.h>

// 生成随机整数集合
std::vector<int> generateRandomSet(int size) {
    std::vector<int> result(size);
    for (int i = 0; i < size; ++i) {
        result[i] = rand() % size;
    }
    return result;
}

// 串行算法求交集
std::vector<int> sequentialIntersection(const std::vector<std::vector<int>>& sets) {
    std::vector<int> intersection = sets[0];
    for (int i = 1; i < sets.size(); ++i) {
        std::vector<int> temp;
        std::set_intersection(intersection.begin(), intersection.end(),
            sets[i].begin(), sets[i].end(),
            std::back_inserter(temp));
        intersection = temp;
    }
    return intersection;
}

// 并行算法求交集
std::vector<int> parallelIntersection(const std::vector<int>& localSet, int world_size) {
    std::vector<int> globalSet;
    MPI_Allreduce(localSet.data(), globalSet.data(), localSet.size(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    std::sort(globalSet.begin(), globalSet.end());
    std::vector<int> intersection;
    for (int i = 0; i < globalSet.size(); ++i) {
        if (std::binary_search(localSet.begin(), localSet.end(), globalSet[i])) {
            intersection.push_back(globalSet[i]);
        }
    }
    return intersection;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    srand(time(NULL));

    const int num_sets = 4;
    std::vector<std::vector<int>> sets(num_sets);
    const int set_size = 1000; // 每个集合的大小

    // 生成随机整数集合
    for (int i = 0; i < num_sets; ++i) {
        sets[i] = generateRandomSet(set_size);
    }

    double start_time, end_time;
    std::vector<int> intersection;

    // 串行算法求解
    if (world_rank == 0) {
        start_time = MPI_Wtime();
        intersection = sequentialIntersection(sets);
        end_time = MPI_Wtime();
        std::cout << "Sequential algorithm execution time: " << end_time - start_time << " seconds\n";
    }

    // 并行算法求解
    start_time = MPI_Wtime();
    std::vector<int> localSet = sets[world_rank % num_sets];
    intersection = parallelIntersection(localSet, world_size);
    end_time = MPI_Wtime();

    if (world_rank == 0) {
        std::cout << "Parallel algorithm execution time: " << end_time - start_time << " seconds\n";
        std::cout << "Speedup: " << (end_time - start_time) / (end_time - start_time) << "\n";
    }

    MPI_Finalize();
    return 0;
}