// Sequential Matrix Multiplication
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define N 500 // Matrix size

typedef vector<vector<int>> Matrix; // Short and simple way of creating of creating 2D arrays

// Function to multiply matrices sequentially
Matrix multiplySequential(const Matrix& A, const Matrix& B) {
    Matrix C(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++) { // Loop for the columns
        for (int j = 0; j < N; j++) { // Loop for the rows 
            for (int k = 0; k < N; k++) {  // Loop for multiplication and addition. 
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

int main() {
    Matrix A(N, vector<int>(N, 1));
    Matrix B(N, vector<int>(N, 1));

    auto start = high_resolution_clock::now();
    Matrix C = multiplySequential(A, B);
    auto stop = high_resolution_clock::now();

    cout << "Execution Time: " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl;
    return 0;
}
