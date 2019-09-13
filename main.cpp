#include <iostream>
#include <ctime>
#include <Windows.h>
#include <process.h>

using namespace std;

const int sizeX = 5;
const int sizeY = 6;
const int numThreads = 4;

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

//void getMem();
//void freeMem();
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

    cout << "Sum One Thread = " << sumOneThread(matrix) << endl;
    cout << "Sum Multiple Thread = " << sumMultiThread(matrix) << endl;

    //cin.ignore();
    //freeMem();
    return 0;
}
/*
void getMem()
{
matrix = new int* [sizeX];
for (int i=0; i<sizeX; i++)
matrix[i] = new int[sizeY];
}

void freeMem()
{
for (int i=0; i<sizeX; i++)
delete[] matrix[i];
delete[] matrix;
}
*/
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
    int result = 0;
    for (int i = 0; i<sizeX; i++)
        for (int j = 0; j<sizeY; j++)
            result += mat[i][j];
    return result;
}

int sumMultiThread(int mat[sizeX][sizeY])
{
    int *ptr;
    ptr = matrix[0];
    HANDLE myThread[numThreads];
    //unsigned ThreadID[numThreads];
    info infos[numThreads];
    int n = (sizeX*sizeY) / numThreads;
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