#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef vector<vector<int>> Matrix;

int current_size; // Global variable for matrix size
Matrix A, B, C;

struct ThreadData
{
    int start_row, end_row;
};

// Thread function: Each thread computes a range of rows
void *multiplyPart(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start_row; i < data->end_row; i++)
    {
        for (int j = 0; j < current_size; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < current_size; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
    return NULL;
}

// Function to evaluate performance for different thread values and sizes
void evaluatePerformance()
{
    int sizes[] = {200, 400, 600};
    int threads_arr[] = {1, 2, 4, 8};

    cout << "\nPerformance Evaluation" << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "Threads" << "\t" << "Matrix Size" << "\t" << "Execution Time (ms)" << endl;
    cout << "------------------------------------------------------" << endl;

    for (int s : sizes)
    {
        current_size = s;
        A = Matrix(s, vector<int>(s, 1));
        B = Matrix(s, vector<int>(s, 1));
        C = Matrix(s, vector<int>(s, 0));

        for (int t : threads_arr)
        {
            pthread_t threads[t];
            ThreadData threadData[t];

            int rows_per_thread = s / t;

            auto start_time = high_resolution_clock::now();

            for (int i = 0; i < t; i++)
            {
                threadData[i].start_row = i * rows_per_thread;
                threadData[i].end_row = (i == t - 1) ? s : (i + 1) * rows_per_thread;
                pthread_create(&threads[i], NULL, multiplyPart, (void *)&threadData[i]);
            }

            for (int i = 0; i < t; i++)
            {
                pthread_join(threads[i], NULL);
            }

            auto stop_time = high_resolution_clock::now();
            double exec_time = duration_cast<milliseconds>(stop_time - start_time).count();

            cout << t << "\t" << s << "\t" << exec_time << " ms" << endl;
        }
    }
    cout << "------------------------------------------------------" << endl;
}

int main()
{
    evaluatePerformance();
    return 0;
}
