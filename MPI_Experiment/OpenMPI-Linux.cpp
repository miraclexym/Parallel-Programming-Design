#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <mpi.h>

using namespace std;

// 生成随机整数集合
void generateRandomSet(vector<int>& set, int size) {
    set.clear();
    for (int i = 0; i < size; ++i) {
        set.push_back(rand() % 1000); // 假设元素取值范围为0到999
    }
    sort(set.begin(), set.end()); // 对集合进行排序
}

// 串行算法求交集
vector<int> serialIntersection(const vector<vector<int>>& sets) {
    vector<int> intersection = sets[0];
    for (size_t i = 1; i < sets.size(); ++i) {
        vector<int> temp;
        set_intersection(intersection.begin(), intersection.end(),
            sets[i].begin(), sets[i].end(),
            back_inserter(temp));
        intersection = temp;
    }
    return intersection;
}

// 并行算法求交集
vector<int> parallelIntersection(const vector<vector<int>>& sets, int world_rank, int world_size) {
    vector<int> local_intersection = sets[world_rank];
    vector<int> global_intersection;
    int num_sets = sets.size();

    // 各进程广播本地集合
    for (int i = 0; i < world_size; ++i) {
        MPI_Bcast(&local_intersection[0], local_intersection.size(), MPI_INT, i, MPI_COMM_WORLD);

        // 求并集
        vector<int> temp;
        set_intersection(local_intersection.begin(), local_intersection.end(),
            global_intersection.begin(), global_intersection.end(),
            back_inserter(temp));
        global_intersection = temp;
    }

    return global_intersection;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // 设置随机种子
    srand(time(NULL) + world_rank);

    // 定义问题规模和节点数
    vector<int> problem_sizes = { 1000, 5000, 10000 };
    vector<int> num_nodes = { 1, 2, 4 };

    // 计算并行算法的执行时间和加速比
    double parallel_start, parallel_end, parallel_time;
    double serial_time;
    vector<double> speedup;

    // 循环遍历问题规模和节点数
    for (int size : problem_sizes) {
        for (int nodes : num_nodes) {
            // 仅当节点数小于等于问题规模时才进行计算
            if (nodes <= world_size) {
                vector<vector<int>> sets(nodes);
                // 生成随机整数集合
                for (int i = 0; i < nodes; ++i) {
                    generateRandomSet(sets[i], size);
                }

                // 串行算法求交集
                MPI_Barrier(MPI_COMM_WORLD);
                if (world_rank == 0) {
                    serial_time = MPI_Wtime();
                }
                vector<int> serial_result = serialIntersection(sets);
                MPI_Barrier(MPI_COMM_WORLD);
                if (world_rank == 0) {
                    serial_time = MPI_Wtime() - serial_time;
                }

                // 并行算法求交集
                MPI_Barrier(MPI_COMM_WORLD);
                if (world_rank == 0) {
                    parallel_start = MPI_Wtime();
                }
                vector<int> parallel_result = parallelIntersection(sets, world_rank, world_size);
                MPI_Barrier(MPI_COMM_WORLD);
                if (world_rank == 0) {
                    parallel_end = MPI_Wtime();
                }
                parallel_time = parallel_end - parallel_start;

                // 计算加速比
                double s = serial_time;
                double p = parallel_time;
                double spd = s / p;
                speedup.push_back(spd);

                // 输出结果
                if (world_rank == 0) {
                    cout << "Problem Size: " << size << ", Nodes: " << nodes << endl;
                    cout << "Serial Time: " << serial_time << " seconds" << endl;
                    cout << "Parallel Time: " << parallel_time << " seconds" << endl;
                    cout << "Speedup: " << spd << endl;
                    cout << endl;
                }
            }
        }
    }

    MPI_Finalize();
    return 0;
}