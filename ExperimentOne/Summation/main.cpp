#include <iostream>
#include <vector>
#include <windows.h>
#include <numeric>
#include <iomanip>
#include <cmath>

using namespace std;

// 设置测试数据规模
const int MinTestDataSize = pow(2, 1);
const int MaxTestDataSize = pow(2, 20);
const int ScaleIncreasingFactor = 2;

// 设置测试循环系数
const int CycleControlCoefficient = MaxTestDataSize * 4;

// 平凡算法
double ordinarySum(const vector<double>& numbers) {
    double sum = 0.0;
    for (const auto& num : numbers) {
        sum += num;
    }
    return sum;
}

// 多链路式算法
double MultiLinkSum(const vector<double>& numbers) {
    double sum = 0.0;
    double sum1 = 0.0;
    double sum2 = 0.0;
    int n = numbers.size();

    for (int i = 0; i + 1 < n; i += 2) {
        sum1 += numbers[i];
        sum2 += numbers[i + 1];
    }

    sum = sum1 + sum2;
    return sum;
}

// 辅助递归
void AuxiliaryRecursion(vector<double>& numbers, int n) {
    if (n == 1)
        return;
    else
    {
        for (int i = 0; i < n / 2; i++)
            numbers[i] += numbers[n - i - 1];
        n = n / 2;
        AuxiliaryRecursion(numbers, n);
    }
}

// 递归算法
double RecursiveSum(const vector<double>& numbers) {
    vector<double> numbersCopy = numbers;
    int n = numbers.size();

    AuxiliaryRecursion(numbersCopy, n);

    return numbersCopy[0];
}

// 循环算法
double CirculateSum(const vector<double>& numbers) {
    vector<double> numbersCopy = numbers;
    int n = numbers.size();

    for (int m = n; m > 1; m /= 2) // log(n)个步骤
        for (int i = 0; i < m / 2; i++)
            numbersCopy[i] = numbersCopy[i * 2] + numbersCopy[i * 2 + 1]; // 相邻元素相加连续存储到数组最前面

    return numbersCopy[0];
}

// 生成随机数
void generateRandomNumbers(int n, vector<double>& numbers) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        numbers.push_back(rand() % 100);
    }
}

// 固定数据规模测试
void FixedDataScaleTesting(int n)
{
    // 随机生成n个待求和的数
    vector<double> numbers;
    generateRandomNumbers(n, numbers);

    // 个性化循环次数
    int PersonalizedNumberOfCycles = CycleControlCoefficient / n;

    // 平凡算法
    long long headOrdinary, tailOrdinary, freqOrdinary;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqOrdinary);
    QueryPerformanceCounter((LARGE_INTEGER*)&headOrdinary);
    double resultOrdinary = 0.0;
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        resultOrdinary = ordinarySum(numbers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailOrdinary);
    double durationOrdinary = (tailOrdinary - headOrdinary) * 1e6 / freqOrdinary;

    // 多链路式算法
    long long headMultiLink, tailMultiLink, freqMultiLink;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqMultiLink);
    QueryPerformanceCounter((LARGE_INTEGER*)&headMultiLink);
    double resultMultiLink = 0.0;
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        resultMultiLink = MultiLinkSum(numbers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailMultiLink);
    double durationMultiLink = (tailMultiLink - headMultiLink) * 1e6 / freqMultiLink;

    // 递归算法
    long long headRecursive, tailRecursive, freqRecursive;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqRecursive);
    QueryPerformanceCounter((LARGE_INTEGER*)&headRecursive);
    double resultRecursive = 0.0;
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        resultRecursive = RecursiveSum(numbers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailRecursive);
    double durationRecursive = (tailRecursive - headRecursive) * 1e6 / freqRecursive;

    // 循环算法
    long long headCirculate, tailCirculate, freqCirculate;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqCirculate);
    QueryPerformanceCounter((LARGE_INTEGER*)&headCirculate);
    double resultCirculate = 0.0;
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        resultCirculate = CirculateSum(numbers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailCirculate);
    double durationCirculate = (tailCirculate - headCirculate) * 1e6 / freqCirculate;

    // 输出数据规模与循环次数
    cout << "Data size : " << n << ". " << "Number of Cycles : " << PersonalizedNumberOfCycles << ".";

    // 输出结果正确与否
    if (resultOrdinary == resultMultiLink && resultOrdinary == resultRecursive && resultOrdinary == resultCirculate)
        cout << " The calculation result is correct." << endl;

    // 输出平凡算法平均运行时间
    cout << fixed << setprecision(4) << "Ordinary algorithms : " << durationOrdinary / PersonalizedNumberOfCycles << " microseconds." << endl;

    // 输出多链算法平均运行时间
    cout << fixed << setprecision(4) << "MultiLink algorithms : " << durationMultiLink / PersonalizedNumberOfCycles << " microseconds.";
    // 输出多链算法加速比
    cout << fixed << setprecision(4) << " Acceleration ratio : " << durationOrdinary / durationMultiLink << "." << endl;

    // 输出递归算法平均运行时间
    cout << fixed << setprecision(4) << "Recursive algorithms : " << durationRecursive / PersonalizedNumberOfCycles << " microseconds.";
    // 输出递归算法加速比
    cout << fixed << setprecision(4) << " Acceleration ratio : " << durationOrdinary / durationRecursive << "." << endl;

    // 输出循环算法平均运行时间
    cout << fixed << setprecision(4) << "Circulate algorithms : " << durationCirculate / PersonalizedNumberOfCycles << " microseconds.";
    // 输出循环算法加速比
    cout << fixed << setprecision(4) << " Acceleration ratio : " << durationOrdinary / durationCirculate << "." << endl << endl;

}

int main() {

    // 对于不同数据规模进行测试
    for (int i = MinTestDataSize; i <= MaxTestDataSize; i *= ScaleIncreasingFactor)
        FixedDataScaleTesting(i);

    return 0;
}
