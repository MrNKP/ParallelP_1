#include "MatrixSumThread.h"


void MatrixSumThread::test() {
    srand((unsigned int)time(nullptr));

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
    for (int i = 0; i < sizeY; i++)
        for (int j = 0; j < sizeX; j++)
            matrix[i][j] = rand() % 10;
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
    double time = 1000 * (endTime-startTime)/freq;  // вычисление времени
    calcTime = time;
    return result;
}

// многопоточная сумма
int MatrixSumThread::sumMultiThread()
{
    __int64 startTime, endTime, freq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    int *matrixLineptr = matrix[0]; // устанавливаем указатель на начало матрицы
    HANDLE myThread[numThreads]; // массив дескрипторов тредов
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
    double time = 1000 * (endTime-startTime)/freq;
    calcTime = time;

    return result;
}

// функция-поток
unsigned __stdcall MatrixSumThread::sum(void * arg)
{
    auto *local = (threadStruct*)arg; // приведение типа аргумента к threadStruct

    for (int i = local->left; i < local->right; i++)
        local->sum += *(*local->matr + i);

    _endthreadex(0);
    return 0;
}