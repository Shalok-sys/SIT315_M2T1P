#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define N 500 // Matrix size
#define NUM_THREADS 4 // Number of threads

typedef vector<vector<int>> Matrix;

Matrix A(N, vector<int>(N, 1));
Matrix B(N, vector<int>(N, 1));
Matrix C(N, vector<int>(N, 0));

struct ThreadData {
    int row;
};

// Thread function: Each thread computes ONE row
void* multiplyPart(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int i = data->row;  // Assigned row for this thread

    for (int j = 0; j < N; j++) {  
        C[i][j] = 0;  
        for (int k = 0; k < N; k++) {  
            C[i][j] += A[i][k] * B[k][j];
        }
    }
    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t threads[N]; // One thread per row
    ThreadData threadData[N];

    auto start_time = high_resolution_clock::now();

    // Creating threads (one per row)
    for (int i = 0; i < N; i++) {
        threadData[i].row = i; // Assign one row to each thread
        pthread_create(&threads[i], NULL, multiplyPart, (void*)&threadData[i]);
    }

    // Joining threads
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    auto stop_time = high_resolution_clock::now();

    cout << "Execution Time (Pthreads with row-wise parallelism): " 
         << duration_cast<milliseconds>(stop_time - start_time).count() 
         << " ms" << endl;

    return 0;
}
