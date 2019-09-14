#include <iostream>
#include <ctime>
#include <Windows.h>
#include <process.h>

using namespace std;

const int sizeX = 5;
const int sizeY = 6;
const int numThreads = 4;

double myTime;

//int **matrix;
int matrix[sizeX][sizeY];
//int *matrixPtr[sizeX];
//int *ptr;

struct info
{
    int **matr;
    int left;
    int right;
    int sum;
};

void generate();
void output();

int sumOneThread(int mat[sizeX][sizeY]);
int sumMultiThread(int mat[sizeX][sizeY]);
unsigned __stdcall sum(void * arg);

int main() {
    srand((unsigned int)time(NULL));
    //getMem();
    generate();
    output();
    //ptr = matrix[0];
    /*
    cout << "via pointers" << endl;
    for (int i = 0; i<sizeX*sizeY; i++)
    {
        cout << *(*matrix + i) << "\t";
        //cout << **matrix << "\t";
        //matrix++;
    }
    */
    cout << "Result for matrix " << sizeX << "*" << sizeY << endl;
    cout << "Sum One Thread = " << sumOneThread(matrix) << endl;
    cout << "Time = " << myTime << " ms" << endl;
    cout << "Sum Multiple Thread = " << sumMultiThread(matrix) << endl;
    cout << "Time = " << myTime << " ms" << endl;

    //cin.ignore();
    //freeMem();
    return 0;
}

void generate()
{
    for (int i = 0; i < sizeX; i++)
    {
        //matrixPtr[i] = matrix[i];
        for (int j = 0; j < sizeY; j++)
            matrix[i][j] = rand() % 10;
    }
}

void output()
{
    cout << "Matrix all" << endl;
    for (int i = 0; i<sizeX; i++)
    {
        for (int j = 0; j<sizeY; j++)
            cout << matrix[i][j] << "\t";
        cout << endl;
    }
}

int sumOneThread(int mat[sizeX][sizeY])
{
    __int64 startTime, endTime, freq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    int result = 0;
    QueryPerformanceCounter((LARGE_INTEGER *)&startTime);
    for (int i = 0; i<sizeX; i++)
        for (int j = 0; j<sizeY; j++)
            result += mat[i][j];
    QueryPerformanceCounter((LARGE_INTEGER *)&endTime);
    double time = 1000 * (endTime-startTime)/freq;
    myTime=time;
    return result;
}

int sumMultiThread(int mat[sizeX][sizeY])
{
    __int64 startTime, endTime, freq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    int *ptr;
    ptr = matrix[0];
    HANDLE myThread[numThreads];
    //unsigned ThreadID[numThreads];
    info infos[numThreads];
    int n = (sizeX*sizeY) / numThreads;
    QueryPerformanceCounter((LARGE_INTEGER *)&startTime);
    for (int i = 0; i<numThreads; i++)
    {
        //infos[i].matr = &matrixPtr[0];
        infos[i].matr = &ptr;
        infos[i].left = n*i;
        infos[i].sum = 0;
        if (i == numThreads-1) infos[i].right = sizeX*sizeY;
        else infos[i].right = n*(i + 1);
        //cout << i << "Left = " << infos[i].left << " Right = " << infos[i].right << endl;
        myThread[i] = (HANDLE)_beginthreadex(NULL, 0, &sum, &infos[i], 0, NULL);
    }
    WaitForMultipleObjects(numThreads, myThread, TRUE, INFINITE);

    int result = 0;
    for (int i = 0; i<numThreads; i++)
    {
        result += infos[i].sum;
        CloseHandle(myThread[i]);
    }
    QueryPerformanceCounter((LARGE_INTEGER *)&endTime);
    double time = 1000 * (endTime-startTime)/freq;
    myTime=time;
    return result;
}

unsigned __stdcall sum(void * arg)
{
    info *local = (info*)arg;
    local->sum = 0;
    for (int i = local->left; i<local->right; i++)
        local->sum += *(*local->matr + i);
    _endthreadex(0);
    return 0;
}