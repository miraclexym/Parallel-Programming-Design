#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <windows.h>

using namespace std;

// ���ò������ݹ�ģ
const int MinTestDataSize = 3e2;
const int MaxTestDataSize = 6e3;
const int ScaleIncreasingFactor = MinTestDataSize;

// ���ò���ѭ��ϵ��
const int CycleControlCoefficient = 1e5;

// ���з���Ԫ�ص�ƽ���㷨
void ordinaryMatrixVectorInnerProduct(const vector<vector<double>>& theMatrix, const vector<double>& theVector, vector<double>& theResult) {
    int n = theMatrix.size();
    theResult.resize(n, 0.0);
    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            theResult[j] += theMatrix[i][j] * theVector[i];
}

// cache�Ż��㷨
void optimizedMatrixVectorInnerProduct(const vector<vector<double>>& theMatrix, const vector<double>& theVector, vector<double>& theResult) {
    int n = theMatrix.size();
    theResult.resize(n, 0.0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            theResult[j] += theMatrix[i][j] * theVector[i];
}

// ����������������
void generateRandomMatrixAndVector(int n, vector<vector<double>>& theMatrix, vector<double>& theVector) {
    srand(time(0));

    for (int i = 0; i < n; i++) {
        vector <double> row;
        for (int j = 0; j < n; j++) {
            row.push_back(rand() % 100);
        }
        theMatrix.push_back(row);
        theVector.push_back(rand() % 100);
    }
}

// �ж������Ƿ����
bool JudgingWhetherVectorsAreEqual(const vector<double>& vec1, const vector<double>& vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    }

    for (int i = 0; i < vec1.size(); i++) {
        if (vec1[i] != vec2[i]) {
            return false;
        }
    }

    return true;
}

// �̶����ݹ�ģ����
void FixedDataScaleTesting(int n)
{
    // ������ɾ���������
    vector<vector<double>> theMatrix;
    vector<double> theVector;
    generateRandomMatrixAndVector(n, theMatrix, theVector);

    // ���Ի�ѭ������
    int PersonalizedNumberOfCycles;
    if (n == 10)
        PersonalizedNumberOfCycles = 1e6;
    else if (n == MaxTestDataSize)
        PersonalizedNumberOfCycles = 5;
    else
    {
        PersonalizedNumberOfCycles = CycleControlCoefficient / n;
    }

    // ���з���Ԫ�ص�ƽ���㷨
    vector<double> theOrdinaryResult;
    long long headOrdinary, tailOrdinary, freqOrdinary;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqOrdinary);
    QueryPerformanceCounter((LARGE_INTEGER*)&headOrdinary);
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        ordinaryMatrixVectorInnerProduct(theMatrix, theVector, theOrdinaryResult);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailOrdinary);
    double duration_ordinary = (tailOrdinary - headOrdinary) * 1e6 / freqOrdinary;

    // cache�Ż��㷨
    vector<double> theOptimizedResult;
    long long headOptimized, tailOptimized, freqOptimized;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqOptimized);
    QueryPerformanceCounter((LARGE_INTEGER*)&headOptimized);
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        optimizedMatrixVectorInnerProduct(theMatrix, theVector, theOptimizedResult);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailOptimized);
    double duration_optimized = (tailOptimized - headOptimized) * 1e6 / freqOptimized;

    // �����ȷ���
    bool areEqual = JudgingWhetherVectorsAreEqual(theOrdinaryResult, theOptimizedResult);
    if (areEqual) {
        cout << "Correct. ";
    }
    else {
        cout << "Incorrect. ";
    }

    // ������ݹ�ģ
    cout << "\nSize : " << n << ". ";

    // ���ѭ������
    // cout << "Cycles : " << PersonalizedNumberOfCycles << ".";

    // ���ƽ���㷨ƽ������ʱ��
    cout << fixed << setprecision(4) << "\nOrdinary : " << duration_ordinary / PersonalizedNumberOfCycles << " us." << endl;

    // ����Ż��㷨ƽ������ʱ��
    cout << fixed << setprecision(4) << "Optimized : " << duration_optimized / PersonalizedNumberOfCycles << " us." << endl;

    // ������ٱ�
    cout << fixed << setprecision(4) << "Ratio : " << duration_ordinary / duration_optimized << "." << endl << endl;
}

int main() {

    // ���ڲ�ͬ���ݹ�ģ���в���
    for (int i = MinTestDataSize; i <= MaxTestDataSize; i += ScaleIncreasingFactor)
        FixedDataScaleTesting(i);

    return 0;
}
