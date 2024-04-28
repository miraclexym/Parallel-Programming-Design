#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <windows.h>

using namespace std;

// 设置测试数据规模
const int MinTestDataSize = 3e2;
const int MaxTestDataSize = 6e3;
const int ScaleIncreasingFactor = MinTestDataSize;

// 设置测试循环系数
const int CycleControlCoefficient = 1e5;

// 逐列访问元素的平凡算法
void ordinaryMatrixVectorInnerProduct(const vector<vector<double>>& theMatrix, const vector<double>& theVector, vector<double>& theResult) {
    int n = theMatrix.size();
    theResult.resize(n, 0.0);
    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            theResult[j] += theMatrix[i][j] * theVector[i];
}

// cache优化算法
void optimizedMatrixVectorInnerProduct(const vector<vector<double>>& theMatrix, const vector<double>& theVector, vector<double>& theResult) {
    int n = theMatrix.size();
    theResult.resize(n, 0.0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            theResult[j] += theMatrix[i][j] * theVector[i];
}

// 生成随机矩阵和向量
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

// 判断向量是否相等
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

// 固定数据规模测试
void FixedDataScaleTesting(int n)
{
    // 随机生成矩阵与向量
    vector<vector<double>> theMatrix;
    vector<double> theVector;
    generateRandomMatrixAndVector(n, theMatrix, theVector);

    // 个性化循环次数
    int PersonalizedNumberOfCycles;
    if (n == 10)
        PersonalizedNumberOfCycles = 1e6;
    else if (n == MaxTestDataSize)
        PersonalizedNumberOfCycles = 5;
    else
    {
        PersonalizedNumberOfCycles = CycleControlCoefficient / n;
    }

    // 逐列访问元素的平凡算法
    vector<double> theOrdinaryResult;
    long long headOrdinary, tailOrdinary, freqOrdinary;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqOrdinary);
    QueryPerformanceCounter((LARGE_INTEGER*)&headOrdinary);
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        ordinaryMatrixVectorInnerProduct(theMatrix, theVector, theOrdinaryResult);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailOrdinary);
    double duration_ordinary = (tailOrdinary - headOrdinary) * 1e6 / freqOrdinary;

    // cache优化算法
    vector<double> theOptimizedResult;
    long long headOptimized, tailOptimized, freqOptimized;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqOptimized);
    QueryPerformanceCounter((LARGE_INTEGER*)&headOptimized);
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        optimizedMatrixVectorInnerProduct(theMatrix, theVector, theOptimizedResult);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailOptimized);
    double duration_optimized = (tailOptimized - headOptimized) * 1e6 / freqOptimized;

    // 输出正确与否
    bool areEqual = JudgingWhetherVectorsAreEqual(theOrdinaryResult, theOptimizedResult);
    if (areEqual) {
        cout << "Correct. ";
    }
    else {
        cout << "Incorrect. ";
    }

    // 输出数据规模
    cout << "\nSize : " << n << ". ";

    // 输出循环次数
    // cout << "Cycles : " << PersonalizedNumberOfCycles << ".";

    // 输出平凡算法平均运行时间
    cout << fixed << setprecision(4) << "\nOrdinary : " << duration_ordinary / PersonalizedNumberOfCycles << " us." << endl;

    // 输出优化算法平均运行时间
    cout << fixed << setprecision(4) << "Optimized : " << duration_optimized / PersonalizedNumberOfCycles << " us." << endl;

    // 输出加速比
    cout << fixed << setprecision(4) << "Ratio : " << duration_ordinary / duration_optimized << "." << endl << endl;
}

int main() {

    // 对于不同数据规模进行测试
    for (int i = MinTestDataSize; i <= MaxTestDataSize; i += ScaleIncreasingFactor)
        FixedDataScaleTesting(i);

    return 0;
}
