#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define N 500         // Matrix size
#define NUM_THREADS 4 // Number of threads

typedef vector<vector<int>> Matrix;

Matrix A(N, vector<int>(N, 1));
Matrix B(N, vector<int>(N, 1));
Matrix C(N, vector<int>(N, 0));

struct ThreadData
{
    int start_row, end_row;
};

// Thread function: Each thread computes multiple rows
void *multiplyPart(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start_row; i < data->end_row; i++)
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
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS]; // Limited threads
    ThreadData threadData[NUM_THREADS];

    auto start_time = high_resolution_clock::now();

    // Distribute rows among threads
    int rows_per_thread = N / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadData[i].start_row = i * rows_per_thread;
        threadData[i].end_row = (i == NUM_THREADS - 1) ? N : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, multiplyPart, (void *)&threadData[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    auto stop_time = high_resolution_clock::now();

    cout << "Execution Time (Pthreads with optimized row-wise parallelism): "
         << duration_cast<milliseconds>(stop_time - start_time).count()
         << " ms" << endl;

    return 0;
}
