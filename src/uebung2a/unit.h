/*********************************************************************
 *  Name       : unit.h                                              *
 *  Verwendung : Umgebung zu Praktikumsaufgabe (MAPRA),              *
 *               Sortieralgorithmen                                  *
 *  Autor      : K. Brix, IGPM RWTH Aachen                           *
 *  Datum      : Mar '08                                             *
 *  Sprache    : C++                                                 *
 *********************************************************************/

#ifndef __unit_h_
#define __unit_h_

#include <iostream>

// Konstanten

extern const unsigned int num_examples;

// Funktionen

void getExample(unsigned int example_id, size_t &length, unsigned int *&array);
bool checkSolution(unsigned int *array);
void swap(unsigned int *array, size_t i, size_t j);

#endif
