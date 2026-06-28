#ifndef LATTICE_MATH_H
#define LATTICE_MATH_H

typedef struct LatticePackage LatticePackage;
LatticePackage* init_lattice();
void compute_lattice_product(LatticePackage* lp, int* secret_vector);
void free_lattice(LatticePackage* lp);

#endif
