/****************************************************************
 *  Name       : unit.h                                         *
 *  Verwendung : Schnittstelle zu Praktikumsumgebung (MAPRA),   *
 *               Nullstellen eines quadratischen Polynoms       *
 *               Demonstration von Ausloeschung und Ueberlauf   *
 *  Autor      : V. Reichelt, IGPM RWTH Aachen                  *
 *  Datum      : Okt '96                                        *
 *  Aenderung  : Nov '19                                        *
 *  Sprache    : C++                                            *
 ****************************************************************/

// Mehrfaches Einbinden verhindern. Dazu wird, falls die Variable
// _UNIT_H nicht gesetzt ist, diese gesetzt und anschließend die
// folgenden Zeilen Code eingebunden. Beim nächsten Versuch die Datei
// einzubinden, ist die Variable bereits gesetzt.

#ifndef _UNIT_H
#define _UNIT_H

// Zugriff auf mathematische Funktionen ermoeglichen:

#include <cmath>

// Falls DBL_MAX noch nicht definiert ist, gibt's den Wert in cfloat.

#ifndef DBL_MAX
#include <cfloat>
#endif

// In der Praktikumsumgebung definierte Konstanten, die durch das
// C++-Schluesselwort "extern" beim Einbinden dieses Header-Files
// in "meina1.cpp" sichtbar werden:

extern const int num_examples;
extern const int infinity;

// Funktionen aus der Praktikumsumgebung, die durch Einbinden der
// Funktionskoepfe fuer das Programm "meina1.cpp" zugreifbar werden:

void getExample(int example_id, double &a, double &b, double &c);
void checkSolution(int num, bool is_complex = false, double x1 = 0, double x2 = 0);

#endif
