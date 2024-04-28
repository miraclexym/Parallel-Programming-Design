#include <iostream>
#include <vector>
#include <algorithm>
#include <immintrin.h>
#include <windows.h>

using namespace std;

// �����㷨
vector<int> bitwise_and_serial(const vector<vector<int>>& vectors) {
    vector<int> result(vectors[0].size(), 1);

    for (const auto& vec : vectors) {
        for (size_t i = 0; i < vec.size(); ++i) {
            result[i] &= vec[i];
        }
    }

    return result;
}

// �����㷨
vector<int> bitwise_and_parallel(const vector<vector<int>>& vectors) {
    vector<int> result(vectors[0].size(), 1);
    int simdSize = 256 / (sizeof(int) * 8); // SIMD�Ĵ�����С

    for (size_t i = 0; i < vectors.size(); ++i) {
        for (size_t j = 0; j < vectors[i].size(); j += simdSize) {
            // ����������������
            __m256i vec_result = _mm256_loadu_si256((__m256i*) & result[j]);
            __m256i vec_input = _mm256_loadu_si256((__m256i*) & vectors[i][j]);

            // ִ��λ������
            vec_result = _mm256_and_si256(vec_result, vec_input);

            // ������洢�����������
            _mm256_storeu_si256((__m256i*) & result[j], vec_result);
        }
    }

    return result;
}

// ����ʵ��
void ConductExperiment(int N, int L)
{
    // �������N��λ����
    vector<vector<int>> vectors(N, vector<int>(L));
    for (auto& vec : vectors) {
        for (auto& bit : vec) {
            bit = rand() % 2; // ����0��1
        }
    }

    // �����㷨
    long long head, tail, freq;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);

    vector<int> serial_result = bitwise_and_serial(vectors);

    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double duration_serial = (tail - head) * 1e6 / freq;

    // �����㷨
    QueryPerformanceCounter((LARGE_INTEGER*)&head);

    vector<int> parallel_result = bitwise_and_parallel(vectors);

    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double duration_parallel = (tail - head) * 1e6 / freq;

    // ��������ʱ��
    // 
    //cout << "Serial result:" << endl;
    //for (const auto& bit : serial_result) {
    //    cout << bit;
    //}
    //cout << "Parallel result:" << endl;
    //for (const auto& bit : parallel_result) {
    //    cout << bit;
    //}
    cout << "Problem scale: " << L << ", Serial duration: "
        << duration_serial << " microseconds" << ", Parallel duration: " << duration_parallel << " microseconds" << endl;
}

int main() {
    const int N = 10; // λ��������

    // ���������ģ��10^3��ʼ��ʮ��������һֱ��10^6
    for (int exp = 3; exp <= 7; ++exp) {
        int L = pow(10, exp); // ÿ��λ�����ĳ���
        ConductExperiment(N, L);
    }
    return 0;
}