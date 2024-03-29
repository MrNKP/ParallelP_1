#include "MatrixSumThread.h"


void MatrixSumThread::test() {

    fillRandMatrix();
    printMatrix();

    cout << "\nSum 1 thread = " << sumOneThread() << endl;
    cout << "Time = " << calcTime << " ms" << endl;
    cout << "\nSum " << numThreads <<" threads = " << sumMultiThread() << endl;
    cout << "Time = " << calcTime << " ms" << endl;
}


// заполняет матрицу рандомными значениями
void MatrixSumThread::fillRandMatrix()
{
    mt19937 gen(time(nullptr)); // новый генератор СЧ c++11
    uniform_int_distribution<> uid(0, 9);

    for (int i = 0; i < sizeY; i++)
        for (int j = 0; j < sizeX; j++)
            matrix[i][j] = uid(gen);
}

// вывести матрицу в консоль
void MatrixSumThread::printMatrix()
{
    if (sizeX > 25 || sizeY > 25)
    {
        cout << "matrix too large for print" << endl;
        return;
    }

    cout << "Matrix [" << sizeX << " x " << sizeY << "]: " << endl;
    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
            cout << matrix[i][j] << "\t";
        cout << endl;
    }
}

// однопоточная сумма
int MatrixSumThread::sumOneThread()
{
    __int64 startTime, endTime, freq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    int result = 0;
    QueryPerformanceCounter((LARGE_INTEGER *)&startTime);
    for (int i = 0; i < sizeY; i++) // вычисление суммы
        for (int j = 0; j < sizeX; j++)
            result += matrix[i][j];

    QueryPerformanceCounter((LARGE_INTEGER *)&endTime);
    uint64_t time = 1000 * (double)(endTime-startTime)/(double)freq;  // вычисление времени
    calcTime = time;
    return result;
}

// многопоточная сумма
int MatrixSumThread::sumMultiThread()
{
    __int64 startTime, endTime, freq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    int *matrixLineptr = matrix[0]; // устанавливаем указатель на начало матрицы
    HANDLE myThread[numThreads]; // массив дескрипторов потока
    threadStruct tstruct[numThreads]; // создаем структуры по кол-ву тредов
    int dataBlockSize = (sizeY * sizeX) / numThreads; // размер блока данных для 1 треда

    QueryPerformanceCounter((LARGE_INTEGER *)&startTime);
    // Запускаем каждый тред
    for (int i = 0; i < numThreads; i++)
    {
        // заполняем структуру для передачи в тред
        tstruct[i].matr = &matrixLineptr;
        tstruct[i].left = dataBlockSize * i;
        tstruct[i].right = (i == numThreads - 1) ? sizeY * sizeX : dataBlockSize * (i + 1);

        // запуск i-го треда
        myThread[i] = (HANDLE)_beginthreadex(
                nullptr,
                0,
                &MatrixSumThread::sum,
                &tstruct[i],
                0,
                nullptr
        );
    }
    WaitForMultipleObjects(numThreads, myThread, TRUE, INFINITE); // ждем остановки всех тредов

    int result = 0;
    // суммируем результаты и закрываем дескрипторы
    for (int i = 0; i < numThreads; i++)
    {
        result += tstruct[i].sum;
        CloseHandle(myThread[i]);
    }

    QueryPerformanceCounter((LARGE_INTEGER *)&endTime);
    double time = 1000 * (double)(endTime-startTime)/(double)freq;
    calcTime = time;

    return result;
}

// функция, которую выполняет поток
unsigned __stdcall MatrixSumThread::sum(void * arg)
{
    auto *local = (threadStruct*)arg; // приведение типа аргумента к threadStruct

    for (int i = local->left; i < local->right; i++)
        local->sum += *(*local->matr + i);

    _endthreadex(0); // завершение потока
    return 0;
}