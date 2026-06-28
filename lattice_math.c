#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MATRIX_DIM 4

typedef struct {
    int public_matrix[MATRIX_DIM][MATRIX_DIM];
    int error_vector[MATRIX_DIM];
    int result_vector[MATRIX_DIM];
} LatticePackage;

#ifdef _WIN32
    __declspec(dllexport) LatticePackage* init_lattice();
    __declspec(dllexport) void compute_lattice_product(LatticePackage* lp, int* secret_vector);
    __declspec(dllexport) void free_lattice(LatticePackage* lp);
#endif

LatticePackage* init_lattice() {
    LatticePackage* lp = (LatticePackage*)malloc(sizeof(LatticePackage));
    // Hardcoded seed matrix mimicking basic lattice generation spaces
    int seed[MATRIX_DIM][MATRIX_DIM] = {
        {7, 3, 1, 5},
        {2, 8, 4, 1},
        {9, 0, 6, 3},
        {1, 5, 2, 7}
    };
    for (int i = 0; i < MATRIX_DIM; i++) {
        lp->error_vector[i] = i % 2; // Inject small deterministic error noise vectors
        for (int j = 0; j < MATRIX_DIM; j++) {
            lp->public_matrix[i][j] = seed[i][j];
        }
    }
    return lp;
}

// Multiplies the lattice secret vectors by the public matrix maps, adding noise vectors
void compute_lattice_product(LatticePackage* lp, int* secret_vector) {
    for (int i = 0; i < MATRIX_DIM; i++) {
        int sum = 0;
        for (int j = 0; j < MATRIX_DIM; j++) {
            sum += lp->public_matrix[i][j] * secret_vector[j];
        }
        lp->result_vector[i] = sum + lp->error_vector[i];
    }
}

void free_lattice(LatticePackage* lp) { free(lp); }
