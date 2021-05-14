/************************************************
 *  Name       : vektor.cpp                     *
 *  Verwendung : Vektorklasse                   *
 *  Autor      :                                *
 *  Datum      :                                *
 ************************************************/

#include "vector.h"

#include <cmath>
#include <cstdlib>
#include <iomanip>

// =======================
//      Konstruktoren
// =======================

// ----- Konstruktor -----

Vector::Vector(size_t l) {
#ifndef NDEBUG
    if (l <= 0) vecError("Nur Vektoren mit positiver Laenge!");
#endif

    length = l;

    vec = new double[l];
    if (vec == NULL) vecError("Nicht genuegend Speicher!");

    for (size_t i = 0; i < l; i++) (*this)(i) = 0;
}

// ----- Destruktor -----

Vector::~Vector() {
    // ***** Hier fehlt was *****
}

// ----- Kopierkonstruktor -----

Vector::Vector(const Vector &x) {
    length = x.length;

    vec = new double[length];
    if (vec == NULL) vecError("Nicht genuegend Speicher!");

    for (size_t i = 0; i < length; i++) (*this)(i) = x(i);
}

// ===========================================
//      Vektorelement schreiben und lesen
// ===========================================

// ----- Standardzugriff auf Vektorelemente -----

double &Vector::operator()(size_t i) {
#ifndef NDEBUG
    if (i >= length) vecError("Ungueltiger Index!");
#endif

    return vec[i];
}

// ----- Lesezugriff auf Elemente eines const Vektors -----

double Vector::operator()(size_t i) const {
#ifndef NDEBUG
    if (i >= length) vecError("Ungueltiger Index!");
#endif

    return vec[i];
}

// =====================
//      Zuweisungen
// =====================

// ----- Zuweisungsoperator "=" ----

Vector &Vector::operator=(const Vector &x) {
    if (length != x.length) {
        delete[] vec;
        length = x.getLength();
        vec = new (std::nothrow) double[length];
        if (vec == NULL) vecError("Nicht genuegend Speicher!");
    }

    for (size_t i = 0; i < length; i++) (*this)(i) = x(i);

    return *this;
}

// ----- Zuweisungsoperator mit Addition "+=" ----

Vector &Vector::operator+=(const Vector &x) {
    // ***** Hier fehlt was *****
}

// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Vector &Vector::operator-=(const Vector &x) {
    // ***** Hier fehlt was *****
}

// ----- Zuweisungsoperator mit Multiplikation "*=" ----

Vector &Vector::operator*=(double c) {
    // ***** Hier fehlt was *****
}

// ----- Zuweisungsoperator mit Divsion "/=" ----

Vector &Vector::operator/=(double c) {
    // ***** Hier fehlt was *****
}

// ==============================
//      Vektorlaenge aendern
// ==============================

// ----- Vektorlaenge aendern -----

Vector &Vector::redim(size_t l) {
    // ***** Hier fehlt was *****
}

// ======================
//      Vektornormen
// ======================

// ----- Euklidische Norm -----

double Vector::norm2() const {
    // ***** Hier fehlt was *****
}

// ----- Maximum-Norm -----

double Vector::normMax() const {
    // ***** Hier fehlt was *****
}

// ==================================
//      arithmetische Operatoren
// ==================================

// ----- Addition "+" -----

Vector operator+(const Vector &x, const Vector &y) {
    // ***** Hier fehlt was *****
}

// ----- Subtraktion "-" -----

Vector operator-(const Vector &x, const Vector &y) {
    // ***** Hier fehlt was *****
}

// ----- Vorzeichen wechseln "-" -----

Vector operator-(const Vector &x) {
    // ***** Hier fehlt was *****
}

// ----- Skalarprodukt "*" -----

double operator*(const Vector &x, const Vector &y) {
    // ***** Hier fehlt was *****
}

// ----- Multiplikation Skalar*Vector "*" -----

Vector operator*(double c, const Vector &x) {
    // ***** Hier fehlt was *****
}

// ----- Multiplikation Vector*Skalar "*" -----

Vector operator*(const Vector &x, double c) {
    // ***** Hier fehlt was *****
}

// ----- Division Vector/Skalar "/" -----

Vector operator/(const Vector &x, double c) {
    // ***** Hier fehlt was *****
}

// ----- Division Vector/Vector "/"  <-->  D^(-1)*x -----

Vector operator/(const Vector &x, const Vector &d) {
    // ***** Hier fehlt was *****
}

// ----- Vector*.Vector "%"  <--> komponentenweise Multiplikation -----

Vector operator%(const Vector &x, const Vector &d) {
    // ***** Hier fehlt was *****
}

// ==============================
//      Vergleichsoperatoren
// ==============================

// ----- Test auf Gleichheit "==" -----

bool operator==(const Vector &x, const Vector &y) {
    if (x.length != y.length) return false;

    for (size_t i = 0; i < x.length; i++)
        if (x(i) != y(i)) return false;

    return true;
}

// ----- Test auf Ungleichheit "!=" -----

bool operator!=(const Vector &x, const Vector &y) {
    // ***** Hier fehlt was *****
}

// ==========================
//      Ein- und Ausgabe
// ==========================

// ----- Ausgabe "<<" -----

std::ostream &operator<<(std::ostream &s, const Vector &x) {
    s << std::setiosflags(std::ios::right);
    for (size_t i = 0; i < x.length; i++) s << "\n(" << std::setw(4) << i << ") " << x(i);

    return s << std::endl;
}

// ----- Eingabe ">>" -----

std::istream &operator>>(std::istream &s, Vector &x) {
    std::cout << std::setiosflags(std::ios::right);
    for (size_t i = 0; i < x.length; i++) {
        std::cout << "\n(" << std::setw(4) << i << ") ";
        s >> x(i);
    }
    return s;
}

// ==========================
//      Fehlerbehandlung
// ==========================

// ----- Ausgabe der Fehlermeldung "str" -----

void Vector::vecError(const char str[]) {
    std::cerr << "\nVektorfehler: " << str << '\n' << std::endl;
    std::abort();
}
