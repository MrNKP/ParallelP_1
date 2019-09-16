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
    // структура передаваема в поток с помощью аргумента
    struct threadStruct
    {
        int **matr{};
        int left{};  // левая граница в матрице
        int right{}; // правая граница
        int sum = 0;
    };

    // размер матрицы
    static const int sizeY = 10000;
    static const int sizeX = 10000;
    static const int numThreads = 4; // кол-во потоков

    uint64_t calcTime; // время выполнения

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
