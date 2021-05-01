/********************************************************************
 *  Name       : unit.h                                             *
 *  Verwendung : Schnittstelle zu Praktikumsumgebung (MAPRA),       *
 *               Adaptive Quadratur                                 *
 *  Autor      : Karl-Heinz Brakhage, Kolja Brix, IGPM RWTH Aachen  *
 *  Datum      : April 2007                                         *
 *******************************************************************/
#ifndef _UNIT_H
#define _UNIT_H

#include <cmath>

const double Pi = 4 * std::atan(1.0);

// Externe Variablen aus der Praktikumsumgebung

extern const int num_examples;

// Die Funktion "f" stellt die Funktion zur Verfuegung, deren Integral bestimmt werden soll.

double f(double x);

// Die folgenden Funktionen sorgen fuer die Beurteilung und eventuell die grafische Ausgabe der Ergebnisse.
// Fuer eine genauere Beschreibung siehe Aufgabenblatt.

void getExample(int ex_id, double &a, double &b, double &epsilon, bool draw = true, int pause = 300);

bool checkSolution(double value);

#endif
