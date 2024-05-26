#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <pthread.h>
#include <windows.h>

using namespace std;

// 定义倒排列表中的文档编号类型
typedef int DocID;

// 定义倒排列表类型
typedef vector<DocID> PostingList;

// 定义倒排索引类型，用unordered_map来存储每个词项对应的倒排列表
typedef unordered_map<string, PostingList> InvertedIndex;

// 定义参数结构体，用于传递给线程函数
struct ThreadArgs {
    InvertedIndex* index1;
    InvertedIndex* index2;
    InvertedIndex result;
};

// 串行算法：求取两个倒排列表的交集
PostingList intersect(const PostingList& list1, const PostingList& list2) {
    PostingList result;
    auto it1 = list1.begin();
    auto it2 = list2.begin();

    // 使用两个指针同时遍历两个有序列表，找到共同的文档编号
    while (it1 != list1.end() && it2 != list2.end()) {
        if (*it1 == *it2) {
            result.push_back(*it1);
            ++it1;
            ++it2;
        }
        else if (*it1 < *it2) {
            ++it1;
        }
        else {
            ++it2;
        }
    }

    return result;
}

// 并行算法：线程函数，求取两个倒排索引的交集
void* parallelIntersect(void* args) {
    ThreadArgs* tArgs = (ThreadArgs*)args;
    InvertedIndex* index1 = tArgs->index1;
    InvertedIndex* index2 = tArgs->index2;
    InvertedIndex& result = tArgs->result;

    // 遍历第一个倒排索引的所有词项
    for (const auto& entry : *index1) {
        const string& term = entry.first;
        const PostingList& list1 = entry.second;

        // 如果第二个倒排索引也包含该词项，则求取交集
        if (index2->count(term)) {
            const PostingList& list2 = (*index2)[term];
            PostingList intersection = intersect(list1, list2);

            // 将交集结果存入最终的倒排索引中
            result[term] = intersection;
        }
    }

    pthread_exit(NULL);
}

// 计算两个倒排索引的交集
InvertedIndex parallelIndexIntersection(const InvertedIndex& index1, const InvertedIndex& index2, int numThreads) {
    pthread_t threads[numThreads];
    ThreadArgs threadArgs[numThreads];
    InvertedIndex result;

    // 按照线程数将词项划分成多个子集
    int idx = 0;
    for (auto it = index1.begin(); it != index1.end(); ++it) {
        if (idx == numThreads) idx = 0;
        threadArgs[idx].index1 = &index1;
        threadArgs[idx].index2 = &index2;
        pthread_create(&threads[idx], NULL, parallelIntersect, (void*)&threadArgs[idx]);
        ++idx;
    }

    // 等待所有线程执行完毕
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
        // 合并各个线程的结果
        for (const auto& entry : threadArgs[i].result) {
            result[entry.first] = entry.second;
        }
    }

    return result;
}

// 计时函数：获取当前系统时间（单位：毫秒）
long long getCurrentTime() {
    LARGE_INTEGER frequency, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&currentTime);
    return (1000LL * currentTime.QuadPart) / frequency.QuadPart;
}

int main() {
    // 构建倒排索引1
    InvertedIndex index1 = {
        {"apple", {1, 3, 5}},
        {"banana", {2, 3, 4, 5}},
        {"orange", {1, 2, 5}}
    };

    // 构建倒排索引2
    InvertedIndex index2 = {
        {"apple", {1, 3, 4}},
        {"banana", {2, 3, 5}},
        {"grape", {1, 2, 4}}
    };

    // 串行算法求交
    long long startTime = getCurrentTime();
    InvertedIndex serialResult;
    for (const auto& entry : index1) {
        const string& term = entry.first;
        if (index2.count(term)) {
            const PostingList& list1 = entry.second;
            const PostingList& list2 = index2.at(term);
            PostingList intersection = intersect(list1, list2);
            serialResult[term] = intersection;
        }
    }
    long long endTime = getCurrentTime();
    cout << "Serial algorithm time: " << (endTime - startTime) << " ms" << endl;

    // 并行算法求交
    int numThreads = 3; // 设置线程数
    startTime = getCurrentTime();
    InvertedIndex parallelResult = parallelIndexIntersection(index1, index2, numThreads);
    endTime = getCurrentTime();
    cout << "Parallel algorithm time: " << (endTime - startTime) << " ms" << endl;

    return 0;
}