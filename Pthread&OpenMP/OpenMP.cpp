#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <omp.h>

using namespace std;

// �����ĵ��ṹ
struct Document {
    string name;
    vector<string> words;
};

// ��ͨ�Ĵ������㷨
vector<string> serialIntersection(const vector<Document>& documents) {
    vector<string> intersection;

    if (documents.empty()) return intersection;

    // ʹ�õ�һ���ĵ��ĵ�����Ϊ��ʼ����
    intersection = documents[0].words;

    // ��ÿ���ĵ�������
    for (size_t i = 1; i < documents.size(); ++i) {
        vector<string> currentWords = documents[i].words;
        vector<string> newIntersection;

        // �ڵ�ǰ�ĵ��ĵ����в����뽻�����ظ���
        for (const string& word : currentWords) {
            if (find(intersection.begin(), intersection.end(), word) != intersection.end()) {
                newIntersection.push_back(word);
            }
        }

        // ���½���
        intersection = newIntersection;
    }

    return intersection;
}

// �������㷨
vector<string> parallelIntersection(const vector<Document>& documents) {
    vector<string> intersection;

    if (documents.empty()) return intersection;

    // ʹ�õ�һ���ĵ��ĵ�����Ϊ��ʼ����
    intersection = documents[0].words;

    // ��ÿ���ĵ�������
#pragma omp parallel for
    for (size_t i = 1; i < documents.size(); ++i) {
        vector<string> currentWords = documents[i].words;
        vector<string> newIntersection;

        // �ڵ�ǰ�ĵ��ĵ����в����뽻�����ظ���
        for (const string& word : currentWords) {
            if (find(intersection.begin(), intersection.end(), word) != intersection.end()) {
                newIntersection.push_back(word);
            }
        }

        // ���½���
#pragma omp critical
        intersection = newIntersection;
    }

    return intersection;
}

// ��ȡ�߾��ȼ�ʱ����ʱ��
double getElapsedTime(LARGE_INTEGER startTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency) {
    return ((endTime.QuadPart - startTime.QuadPart) * 1000.0) / frequency.QuadPart;
}

int main() {
    // ����ʾ���ĵ��͵�������
    vector<Document> documents = {
        {"Doc1", {"apple", "banana", "orange", "grape"}},
        {"Doc2", {"banana", "orange", "grape", "kiwi"}},
        {"Doc3", {"orange", "grape", "kiwi", "melon"}}
    };

    // ʹ��Windows API��ȡ��ʱ��Ƶ��
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    // ���������㷨��ִ��ʱ��
    LARGE_INTEGER startTimeSerial, endTimeSerial;
    QueryPerformanceCounter(&startTimeSerial);
    vector<string> intersectionSerial = serialIntersection(documents);
    QueryPerformanceCounter(&endTimeSerial);
    double elapsedTimeSerial = getElapsedTime(startTimeSerial, endTimeSerial, frequency);

    // ���������㷨��ִ��ʱ��
    LARGE_INTEGER startTimeParallel, endTimeParallel;
    QueryPerformanceCounter(&startTimeParallel);
    vector<string> intersectionParallel = parallelIntersection(documents);
    QueryPerformanceCounter(&endTimeParallel);
    double elapsedTimeParallel = getElapsedTime(startTimeParallel, endTimeParallel, frequency);

    // ��������ִ��ʱ��
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