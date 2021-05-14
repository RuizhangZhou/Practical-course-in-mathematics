/**************************************************
 *  Name       : unit.h                           *
 *  Verwendung : Header zu unit.cpp               *
 *  Autoren    : V. Reichelt, T. Barsch, K. Urban *
 *               IGPM RWTH Aachen                 *
 *  Anpassung:   K. Brix, F. Gruber,              *
 *  Anpassung:  H. Sass, P. Brandner, S. Groß     *
 *  Datum      : Nov '96 - Apr '19                *
 **************************************************/

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "sparse_matrix.h"
#include "vector.h"

extern const int num_examples;

/**
   Eingabe: int ex_id, Matrix A, Vector x0, Vector b, double tol, int max_iter.
   Ausgabe: Matrix A, Vector x0, Vector b, double tol, int max_iter.
 */
void getExample(int ex_id, Sparse_Matrix &A, Vector &x0, Vector &b, double &tol, int &max_iter);

/**
 method: 0 -> GSV, 1-> CG
 */
void checkSolution(Vector &x, int num_iterations, int method);
