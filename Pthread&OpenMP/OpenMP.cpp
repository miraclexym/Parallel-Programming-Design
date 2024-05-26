#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <omp.h>

using namespace std;

// 定义文档结构
struct Document {
    string name;
    vector<string> words;
};

// 普通的串行求交算法
vector<string> serialIntersection(const vector<Document>& documents) {
    vector<string> intersection;

    if (documents.empty()) return intersection;

    // 使用第一个文档的单词作为初始交集
    intersection = documents[0].words;

    // 对每个文档进行求交
    for (size_t i = 1; i < documents.size(); ++i) {
        vector<string> currentWords = documents[i].words;
        vector<string> newIntersection;

        // 在当前文档的单词中查找与交集的重复项
        for (const string& word : currentWords) {
            if (find(intersection.begin(), intersection.end(), word) != intersection.end()) {
                newIntersection.push_back(word);
            }
        }

        // 更新交集
        intersection = newIntersection;
    }

    return intersection;
}

// 并行求交算法
vector<string> parallelIntersection(const vector<Document>& documents) {
    vector<string> intersection;

    if (documents.empty()) return intersection;

    // 使用第一个文档的单词作为初始交集
    intersection = documents[0].words;

    // 对每个文档进行求交
#pragma omp parallel for
    for (size_t i = 1; i < documents.size(); ++i) {
        vector<string> currentWords = documents[i].words;
        vector<string> newIntersection;

        // 在当前文档的单词中查找与交集的重复项
        for (const string& word : currentWords) {
            if (find(intersection.begin(), intersection.end(), word) != intersection.end()) {
                newIntersection.push_back(word);
            }
        }

        // 更新交集
#pragma omp critical
        intersection = newIntersection;
    }

    return intersection;
}

// 获取高精度计时器的时间
double getElapsedTime(LARGE_INTEGER startTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency) {
    return ((endTime.QuadPart - startTime.QuadPart) * 1000.0) / frequency.QuadPart;
}

int main() {
    // 创建示例文档和倒排索引
    vector<Document> documents = {
        {"Doc1", {"apple", "banana", "orange", "grape"}},
        {"Doc2", {"banana", "orange", "grape", "kiwi"}},
        {"Doc3", {"orange", "grape", "kiwi", "melon"}}
    };

    // 使用Windows API获取计时器频率
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    // 测量串行算法的执行时间
    LARGE_INTEGER startTimeSerial, endTimeSerial;
    QueryPerformanceCounter(&startTimeSerial);
    vector<string> intersectionSerial = serialIntersection(documents);
    QueryPerformanceCounter(&endTimeSerial);
    double elapsedTimeSerial = getElapsedTime(startTimeSerial, endTimeSerial, frequency);

    // 测量并行算法的执行时间
    LARGE_INTEGER startTimeParallel, endTimeParallel;
    QueryPerformanceCounter(&startTimeParallel);
    vector<string> intersectionParallel = parallelIntersection(documents);
    QueryPerformanceCounter(&endTimeParallel);
    double elapsedTimeParallel = getElapsedTime(startTimeParallel, endTimeParallel, frequency);

    // 输出结果和执行时间
    cout << "Serial Intersection:";
    for (const string& word : intersectionSerial) {
        cout << " " << word;
    }
    cout << endl;
    cout << "Serial Execution Time: " << elapsedTimeSerial << " ms" << endl;

    cout << "Parallel Intersection:";
    for (const string& word : intersectionParallel) {
        cout << " " << word;
    }
    cout << endl;
    cout << "Parallel Execution Time: " << elapsedTimeParallel << " ms" << endl;

    return 0;
}