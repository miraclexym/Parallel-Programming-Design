#include <iostream>
#include <vector>
#include <windows.h>
#include <numeric>
#include <iomanip>
#include <cmath>

using namespace std;

// ���ò������ݹ�ģ
const int MinTestDataSize = pow(2, 1);
const int MaxTestDataSize = pow(2, 20);
const int ScaleIncreasingFactor = 2;

// ���ò���ѭ��ϵ��
const int CycleControlCoefficient = MaxTestDataSize * 4;

// ƽ���㷨
double ordinarySum(const vector<double>& numbers) {
    double sum = 0.0;
    for (const auto& num : numbers) {
        sum += num;
    }
    return sum;
}

// ����·ʽ�㷨
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

// �����ݹ�
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

// �ݹ��㷨
double RecursiveSum(const vector<double>& numbers) {
    vector<double> numbersCopy = numbers;
    int n = numbers.size();

    AuxiliaryRecursion(numbersCopy, n);

    return numbersCopy[0];
}

// ѭ���㷨
double CirculateSum(const vector<double>& numbers) {
    vector<double> numbersCopy = numbers;
    int n = numbers.size();

    for (int m = n; m > 1; m /= 2) // log(n)������
        for (int i = 0; i < m / 2; i++)
            numbersCopy[i] = numbersCopy[i * 2] + numbersCopy[i * 2 + 1]; // ����Ԫ����������洢��������ǰ��

    return numbersCopy[0];
}

// ���������
void generateRandomNumbers(int n, vector<double>& numbers) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        numbers.push_back(rand() % 100);
    }
}

// �̶����ݹ�ģ����
void FixedDataScaleTesting(int n)
{
    // �������n������͵���
    vector<double> numbers;
    generateRandomNumbers(n, numbers);

    // ���Ի�ѭ������
    int PersonalizedNumberOfCycles = CycleControlCoefficient / n;

    // ƽ���㷨
    long long headOrdinary, tailOrdinary, freqOrdinary;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqOrdinary);
    QueryPerformanceCounter((LARGE_INTEGER*)&headOrdinary);
    double resultOrdinary = 0.0;
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        resultOrdinary = ordinarySum(numbers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailOrdinary);
    double durationOrdinary = (tailOrdinary - headOrdinary) * 1e6 / freqOrdinary;

    // ����·ʽ�㷨
    long long headMultiLink, tailMultiLink, freqMultiLink;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqMultiLink);
    QueryPerformanceCounter((LARGE_INTEGER*)&headMultiLink);
    double resultMultiLink = 0.0;
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        resultMultiLink = MultiLinkSum(numbers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailMultiLink);
    double durationMultiLink = (tailMultiLink - headMultiLink) * 1e6 / freqMultiLink;

    // �ݹ��㷨
    long long headRecursive, tailRecursive, freqRecursive;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqRecursive);
    QueryPerformanceCounter((LARGE_INTEGER*)&headRecursive);
    double resultRecursive = 0.0;
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        resultRecursive = RecursiveSum(numbers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailRecursive);
    double durationRecursive = (tailRecursive - headRecursive) * 1e6 / freqRecursive;

    // ѭ���㷨
    long long headCirculate, tailCirculate, freqCirculate;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqCirculate);
    QueryPerformanceCounter((LARGE_INTEGER*)&headCirculate);
    double resultCirculate = 0.0;
    for (int i = 0; i < PersonalizedNumberOfCycles; i++)
        resultCirculate = CirculateSum(numbers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tailCirculate);
    double durationCirculate = (tailCirculate - headCirculate) * 1e6 / freqCirculate;

    // ������ݹ�ģ��ѭ������
    cout << "Data size : " << n << ". " << "Number of Cycles : " << PersonalizedNumberOfCycles << ".";

    // ��������ȷ���
    if (resultOrdinary == resultMultiLink && resultOrdinary == resultRecursive && resultOrdinary == resultCirculate)
        cout << " The calculation result is correct." << endl;

    // ���ƽ���㷨ƽ������ʱ��
    cout << fixed << setprecision(4) << "Ordinary algorithms : " << durationOrdinary / PersonalizedNumberOfCycles << " microseconds." << endl;

    // ��������㷨ƽ������ʱ��
    cout << fixed << setprecision(4) << "MultiLink algorithms : " << durationMultiLink / PersonalizedNumberOfCycles << " microseconds.";
    // ��������㷨���ٱ�
    cout << fixed << setprecision(4) << " Acceleration ratio : " << durationOrdinary / durationMultiLink << "." << endl;

    // ����ݹ��㷨ƽ������ʱ��
    cout << fixed << setprecision(4) << "Recursive algorithms : " << durationRecursive / PersonalizedNumberOfCycles << " microseconds.";
    // ����ݹ��㷨���ٱ�
    cout << fixed << setprecision(4) << " Acceleration ratio : " << durationOrdinary / durationRecursive << "." << endl;

    // ���ѭ���㷨ƽ������ʱ��
    cout << fixed << setprecision(4) << "Circulate algorithms : " << durationCirculate / PersonalizedNumberOfCycles << " microseconds.";
    // ���ѭ���㷨���ٱ�
    cout << fixed << setprecision(4) << " Acceleration ratio : " << durationOrdinary / durationCirculate << "." << endl << endl;

}

int main() {

    // ���ڲ�ͬ���ݹ�ģ���в���
    for (int i = MinTestDataSize; i <= MaxTestDataSize; i *= ScaleIncreasingFactor)
        FixedDataScaleTesting(i);

    return 0;
}
