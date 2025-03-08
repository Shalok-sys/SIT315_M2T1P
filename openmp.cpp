#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

const int SIZE = 500; // Adjust matrix size as needed

typedef vector<vector<int>> Matrix;

// Function to initialize a matrix with random values
void initializeMatrix(Matrix &mat)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            mat[i][j] = rand() % 10; // Random values between 0 and 9
        }
    }
}

// Parallel matrix multiplication using OpenMP
Matrix multiplyMatrices(const Matrix &A, const Matrix &B)
{
    Matrix C(SIZE, vector<int>(SIZE, 0));

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Assign rows to threads in a balanced way
        for (int i = thread_id; i < SIZE; i += num_threads)
        {
            for (int j = 0; j < SIZE; j++)
            {
                C[i][j] = 0;
                for (int k = 0; k < SIZE; k++)
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
    Matrix A(SIZE, vector<int>(SIZE));
    Matrix B(SIZE, vector<int>(SIZE));

    initializeMatrix(A);
    initializeMatrix(B);

    double start_time = omp_get_wtime();
    Matrix result = multiplyMatrices(A, B);
    double end_time = omp_get_wtime();

    cout << "Time taken for parallel matrix multiplication: " << (end_time - start_time) << " seconds." << endl;
    return 0;
}
