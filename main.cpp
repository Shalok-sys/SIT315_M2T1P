#include <iostream>
#include <vector>
#include <chrono>
#include <pthread.h>
#include <omp.h>
#include <iomanip>

using namespace std;
using namespace std::chrono;

#define N 500         // Matrix size
#define NUM_THREADS 4 // Number of threads for pthreads

typedef vector<vector<int>> Matrix;

// Global Matrices
Matrix A(N, vector<int>(N, 1));
Matrix B(N, vector<int>(N, 1));
Matrix C(N, vector<int>(N, 0));

// Sequential Matrix Multiplication
Matrix multiplySequential(const Matrix &A, const Matrix &B)
{
    Matrix C(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// Pthreads Implementation
struct ThreadData
{
    int row;
};
void *multiplyPart(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int i = data->row;
    for (int j = 0; j < N; j++)
    {
        for (int k = 0; k < N; k++)
        {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
    pthread_exit(NULL);
    return NULL;
}
void multiplyPthreads()
{
    pthread_t threads[N];
    ThreadData threadData[N];
    for (int i = 0; i < N; i++)
    {
        threadData[i].row = i;
        pthread_create(&threads[i], NULL, multiplyPart, (void *)&threadData[i]);
    }
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

// OpenMP Implementation
Matrix multiplyOpenMP(const Matrix &A, const Matrix &B)
{
    Matrix C(N, vector<int>(N, 0));

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Assign rows to threads in a balanced way
        for (int i = thread_id; i < N; i += num_threads)
        {
            for (int j = 0; j < N; j++)
            {
                C[i][j] = 0;
                for (int k = 0; k < N; k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    return C;
}

int main()
{
    // Sequential Execution
    auto start_seq = high_resolution_clock::now();
    Matrix C_seq = multiplySequential(A, B);
    auto stop_seq = high_resolution_clock::now();
    double time_seq = duration_cast<milliseconds>(stop_seq - start_seq).count();

    // Pthreads Execution
    auto start_pth = high_resolution_clock::now();
    multiplyPthreads();
    auto stop_pth = high_resolution_clock::now();
    double time_pth = duration_cast<milliseconds>(stop_pth - start_pth).count();

    // OpenMP Execution
    double start_omp = omp_get_wtime();
    Matrix C_omp = multiplyOpenMP(A, B);
    double end_omp = omp_get_wtime();
    double time_omp = (end_omp - start_omp) * 1000;

    // Print results in table format
    cout << "\nMatrix Multiplication Performance Comparison" << endl;
    cout << "--------------------------------------------" << endl;
    cout << setw(15) << "Method" << setw(20) << "Execution Time (ms)" << endl;
    cout << "--------------------------------------------" << endl;
    cout << setw(15) << "Sequential" << setw(20) << time_seq << " ms" << endl;
    cout << setw(15) << "Pthreads" << setw(20) << time_pth << " ms" << endl;
    cout << setw(15) << "OpenMP" << setw(20) << time_omp << " ms" << endl;
    cout << "--------------------------------------------" << endl;

    return 0;
}
