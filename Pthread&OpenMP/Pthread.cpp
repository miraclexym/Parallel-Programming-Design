#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <pthread.h>
#include <windows.h>

using namespace std;

// ���嵹���б��е��ĵ��������
typedef int DocID;

// ���嵹���б�����
typedef vector<DocID> PostingList;

// ���嵹���������ͣ���unordered_map���洢ÿ�������Ӧ�ĵ����б�
typedef unordered_map<string, PostingList> InvertedIndex;

// ��������ṹ�壬���ڴ��ݸ��̺߳���
struct ThreadArgs {
    InvertedIndex* index1;
    InvertedIndex* index2;
    InvertedIndex result;
};

// �����㷨����ȡ���������б�Ľ���
PostingList intersect(const PostingList& list1, const PostingList& list2) {
    PostingList result;
    auto it1 = list1.begin();
    auto it2 = list2.begin();

    // ʹ������ָ��ͬʱ�������������б��ҵ���ͬ���ĵ����
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

// �����㷨���̺߳�������ȡ�������������Ľ���
void* parallelIntersect(void* args) {
    ThreadArgs* tArgs = (ThreadArgs*)args;
    InvertedIndex* index1 = tArgs->index1;
    InvertedIndex* index2 = tArgs->index2;
    InvertedIndex& result = tArgs->result;

    // ������һ���������������д���
    for (const auto& entry : *index1) {
        const string& term = entry.first;
        const PostingList& list1 = entry.second;

        // ����ڶ�����������Ҳ�����ô������ȡ����
        if (index2->count(term)) {
            const PostingList& list2 = (*index2)[term];
            PostingList intersection = intersect(list1, list2);

            // ����������������յĵ���������
            result[term] = intersection;
        }
    }

    pthread_exit(NULL);
}

// �����������������Ľ���
InvertedIndex parallelIndexIntersection(const InvertedIndex& index1, const InvertedIndex& index2, int numThreads) {
    pthread_t threads[numThreads];
    ThreadArgs threadArgs[numThreads];
    InvertedIndex result;

    // �����߳���������ֳɶ���Ӽ�
    int idx = 0;
    for (auto it = index1.begin(); it != index1.end(); ++it) {
        if (idx == numThreads) idx = 0;
        threadArgs[idx].index1 = &index1;
        threadArgs[idx].index2 = &index2;
        pthread_create(&threads[idx], NULL, parallelIntersect, (void*)&threadArgs[idx]);
        ++idx;
    }

    // �ȴ������߳�ִ�����
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
        // �ϲ������̵߳Ľ��
        for (const auto& entry : threadArgs[i].result) {
            result[entry.first] = entry.second;
        }
    }

    return result;
}

// ��ʱ��������ȡ��ǰϵͳʱ�䣨��λ�����룩
long long getCurrentTime() {
    LARGE_INTEGER frequency, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&currentTime);
    return (1000LL * currentTime.QuadPart) / frequency.QuadPart;
}

int main() {
    // ������������1
    InvertedIndex index1 = {
        {"apple", {1, 3, 5}},
        {"banana", {2, 3, 4, 5}},
        {"orange", {1, 2, 5}}
    };

    // ������������2
    InvertedIndex index2 = {
        {"apple", {1, 3, 4}},
        {"banana", {2, 3, 5}},
        {"grape", {1, 2, 4}}
    };

    // �����㷨��
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

    // �����㷨��
    int numThreads = 3; // �����߳���
    startTime = getCurrentTime();
    InvertedIndex parallelResult = parallelIndexIntersection(index1, index2, numThreads);
    endTime = getCurrentTime();
    cout << "Parallel algorithm time: " << (endTime - startTime) << " ms" << endl;

    return 0;
}