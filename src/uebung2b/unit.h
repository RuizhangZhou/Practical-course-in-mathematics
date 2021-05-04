/*********************************************************************
 *  Name       : unit.h                                              *
 *  Verwendung : Umgebung zu Praktikumsaufgabe (MaPra),              *
 *               Sortieralgorithmen mit Templates                    *
 *  Autor      : S. Gross, IGPM RWTH Aachen                          *
 *  Datum      : Apr '13                                             *
 *  Sprache    : C++                                                 *
 *********************************************************************/

#ifndef _UNIT_H
#define _UNIT_H

#include <iostream>
#include <string>
#include <vector>

#include "student.h"

// Funktionen zur Ueberpruefung der Ergebnisse
// geben zurueck, ob ein Fehler entdeckt wurde
bool checkSolution(const std::vector<double>& array);
bool checkSolution(const std::vector<std::string>& array);
bool checkSolution(const std::vector<Student>& array);

#endif
