/*********************************************************************
 *  Name       : student.h                                           *
 *  Verwendung : Struct zum Speichern von Studidaten,                *
 *               Sortieralgorithmen mit Templates (MaPra)            *
 *  Autor      : S. Gross, IGPM RWTH Aachen                          *
 *  Datum      : Apr '13                                             *
 *  Sprache    : C++                                                 *
 *********************************************************************/

#ifndef _STUDENT_H
#define _STUDENT_H

#include <iostream>
#include <string>

struct Student {
    std::string first_name, last_name;
    int matr_nr;
    double grade;
};

// Eingabeoperator ">>"
std::istream& operator>>(std::istream&, Student&);

// Ausgabeoperator "<<"
std::ostream& operator<<(std::ostream&, const Student&);

// Vergleichsoperator "<"
bool operator<(const Student&, const Student&);

// Vergleichsoperatoren "==" bzw. "!="
bool operator==(const Student&, const Student&);
bool operator!=(const Student&, const Student&);

#endif
