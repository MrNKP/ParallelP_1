#ifndef PARALLELP_1_MATRIXSUMTHREAD_H
#define PARALLELP_1_MATRIXSUMTHREAD_H

#include <iostream>
#include <random>
#include <ctime>
#include <Windows.h>
#include <process.h>

using namespace std;

class MatrixSumThread
{
    struct threadStruct
    {
        int **matr{};
        int left{};  // левая граница в матрице
        int right{}; // правая граница
        int sum = 0;
    };

    static const int sizeY = 1000;
    static const int sizeX = 1000;
    static const int numThreads = 4;

    uint64_t calcTime;

    int matrix[sizeY][sizeX];

public:

    void test();

    void fillRandMatrix();
    void printMatrix();
    int sumOneThread();
    int sumMultiThread();
    static unsigned __stdcall sum(void * arg);
};



#endif //PARALLELP_1_MATRIXSUMTHREAD_H
