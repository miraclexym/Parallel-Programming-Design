#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <mpi.h>

using namespace std;

// ���������������
void generateRandomSet(vector<int>& set, int size) {
    set.clear();
    for (int i = 0; i < size; ++i) {
        set.push_back(rand() % 1000); // ����Ԫ��ȡֵ��ΧΪ0��999
    }
    sort(set.begin(), set.end()); // �Լ��Ͻ�������
}

// �����㷨�󽻼�
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

// �����㷨�󽻼�
vector<int> parallelIntersection(const vector<vector<int>>& sets, int world_rank, int world_size) {
    vector<int> local_intersection = sets[world_rank];
    vector<int> global_intersection;
    int num_sets = sets.size();

    // �����̹㲥���ؼ���
    for (int i = 0; i < world_size; ++i) {
        MPI_Bcast(&local_intersection[0], local_intersection.size(), MPI_INT, i, MPI_COMM_WORLD);

        // �󲢼�
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

    // �����������
    srand(time(NULL) + world_rank);

    // ���������ģ�ͽڵ���
    vector<int> problem_sizes = { 1000, 5000, 10000 };
    vector<int> num_nodes = { 1, 2, 4 };

    // ���㲢���㷨��ִ��ʱ��ͼ��ٱ�
    double parallel_start, parallel_end, parallel_time;
    double serial_time;
    vector<double> speedup;

    // ѭ�����������ģ�ͽڵ���
    for (int size : problem_sizes) {
        for (int nodes : num_nodes) {
            // �����ڵ���С�ڵ��������ģʱ�Ž��м���
            if (nodes <= world_size) {
                vector<vector<int>> sets(nodes);
                // ���������������
                for (int i = 0; i < nodes; ++i) {
                    generateRandomSet(sets[i], size);
                }

                // �����㷨�󽻼�
                MPI_Barrier(MPI_COMM_WORLD);
                if (world_rank == 0) {
                    serial_time = MPI_Wtime();
                }
                vector<int> serial_result = serialIntersection(sets);
                MPI_Barrier(MPI_COMM_WORLD);
                if (world_rank == 0) {
                    serial_time = MPI_Wtime() - serial_time;
                }

                // �����㷨�󽻼�
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

                // ������ٱ�
                double s = serial_time;
                double p = parallel_time;
                double spd = s / p;
                speedup.push_back(spd);

                // ������
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