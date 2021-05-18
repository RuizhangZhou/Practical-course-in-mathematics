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
#include <memory>
#include <array>

// =======================
//      Konstruktoren
// =======================

// ----- Konstruktor -----

Vector::Vector(size_t l) {
#ifndef NDEBUG
    if (l <= 0) vecError("Nur Vektoren mit positiver Laenge!");
#endif

    vec = make_unique<vector<double>>(l);
    length = vec->size();
    if (vec == nullptr) vecError("Nicht genuegend Speicher!");

    for (size_t i = 0; i < l; i++) (*this)(i) = 0;
}

// ----- Destruktor -----

Vector::~Vector() {
    // ***** Hier fehlt was *****
}

// ----- Kopierkonstruktor -----

Vector::Vector(const Vector &x) {
    vec = make_unique<vector<double>>(x.vec->size());
    if (vec == nullptr) vecError("Nicht genuegend Speicher!");
    length = vec->size();
    for (size_t i = 0; i < x.vec->size(); i++) (*this)(i) = x(i);
}

// ===========================================
//      Vektorelement schreiben und lesen
// ===========================================

// ----- Standardzugriff auf Vektorelemente -----

double &Vector::operator()(size_t i) {
#ifndef NDEBUG
    if (i >= vec->size()) vecError("Ungueltiger Index!");
#endif

    return (*vec)[i];
}

// ----- Lesezugriff auf Elemente eines const Vektors -----

double Vector::operator()(size_t i) const {
#ifndef NDEBUG
    if (i >= vec->size()) vecError("Ungueltiger Index!");
#endif

    return (*vec)[i];
}

// =====================
//      Zuweisungen
// =====================

// ----- Zuweisungsoperator "=" ----

Vector &Vector::operator=(const Vector &x) {
    if (vec->size() != x.vec->size()) {
        vec = make_unique<vector<double>>(x.vec->size());
        if (vec == nullptr) vecError("Nicht genuegend Speicher!");
    }
    length = vec->size();
    for (size_t i = 0; i < vec->size(); i++) (*this)(i) = x(i);

    return *this;
}

// ----- Zuweisungsoperator mit Addition "+=" ----

Vector &Vector::operator+=(const Vector &x) {
    if (vec->size() != x.vec->size()) {
        vecError("Ungleiche Länge");
    }
    for (size_t i = 0; i < vec->size(); i++) {
        (*this)(i) += x(i);
    }
    return *this;
}

// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Vector &Vector::operator-=(const Vector &x) {
    if (vec->size() != x.vec->size()) {
        vecError("Ungleiche Länge");
    }
    for (size_t i = 0; i < vec->size(); i++) {
        (*this)(i) -= x(i);
    }
    return *this;
}

// ----- Zuweisungsoperator mit Multiplikation "*=" ----

Vector &Vector::operator*=(double c) {
    for (size_t i = 0; i < vec->size(); i++) {
        (*this)(i) *= c;
    }
    return *this;
}

// ----- Zuweisungsoperator mit Divsion "/=" ----

Vector &Vector::operator/=(double c) {
    for (size_t i = 0; i < vec->size(); i++) {
        (*this)(i) /= c;
    }
    return *this;
}

// ==============================
//      Vektorlaenge aendern
// ==============================

// ----- Vektorlaenge aendern -----

Vector &Vector::redim(size_t l) {
    vec = make_unique<vector<double>>(l);
    length = vec->size();
    for (size_t i = 0; i < l; i++) (*this)(i) = 0;
    return (*this);
}

// ======================
//      Vektornormen
// ======================

// ----- Euklidische Norm -----

double Vector::norm2() const {
    double norm = 0;
    for (const double i : *vec) {
        norm += i * i;
    }
    return sqrt(norm);
}

// ----- Maximum-Norm -----

double Vector::normMax() const {
    double norm = abs((*vec)[0]);
    for (const double i : *vec) {
        norm = max(norm, abs(i));
    }
    return norm;
}

// ==================================
//      arithmetische Operatoren
// ==================================

// ----- Addition "+" -----

Vector operator+(const Vector &x, const Vector &y) {
    auto vec = make_unique<Vector>(x.vec->size());
    if (x.vec->size() != y.vec->size()) {

    }
    for (size_t i = 0; i < x.vec->size(); i++) {
        (*vec)(i) = x(i) + y(i);
    }
    return *vec;
}

// ----- Subtraktion "-" -----

Vector operator-(const Vector &x, const Vector &y) {
    auto vec = make_unique<Vector>(x.vec->size());
    if (x.vec->size() != y.vec->size()) {

    }
    for (size_t i = 0; i < x.vec->size(); i++) {
        (*vec)(i) = x(i) - y(i);
    }
    return *vec;
}

// ----- Vorzeichen wechseln "-" -----

Vector operator-(const Vector &x) {
    auto vec = make_unique<Vector>(x.vec->size());
    for (size_t i = 0; i < x.vec->size(); i++) {
        (*vec)(i) = -x(i);
    }
    return *vec;
}

// ----- Skalarprodukt "*" -----

double operator*(const Vector &x, const Vector &y) {
    double scalar = 0;
    if (x.vec->size() != y.vec->size()) {
        Vector::vecError("Ungleiche Länge");
    }
    for (size_t i = 0; i < x.vec->size(); i++) {
        scalar += x(i) * y(i);
    }
    return scalar;
}

// ----- Multiplikation Skalar*Vector "*" -----

Vector operator*(double c, const Vector &x) {
    auto vec = make_unique<Vector>(x.vec->size());
    for (size_t i = 0; i < x.vec->size(); i++) {
        (*vec)(i) = x(i) * c;
    }
    return *vec;
}

// ----- Multiplikation Vector*Skalar "*" -----

Vector operator*(const Vector &x, double c) {
    return c * x;
}

// ----- Division Vector/Skalar "/" -----

Vector operator/(const Vector &x, double c) {
    auto vec = make_unique<Vector>(x.vec->size());
    for (size_t i = 0; i < x.vec->size(); i++) {
        (*vec)(i) = x(i) / c;
    }
    return *vec;
}

// ----- Division Vector/Vector "/"  <-->  D^(-1)*x -----

Vector operator/(const Vector &x, const Vector &d) {
    // ***** Hier fehlt was *****
    // erste Zeile von Seite 6
    //as D is a Diagonalmatrix so we can devide every element of x by every element on the diagonal
    auto vec = make_unique<Vector>(x.vec->size());
    for (size_t i = 0; i < x.vec->size(); i++) {
        (*vec)(i) = x(i) / d(i);
    }
    return *vec;
}

// ----- Vector*.Vector "%"  <--> komponentenweise Multiplikation -----

Vector operator%(const Vector &x, const Vector &d) {
    auto vec = make_unique<Vector>(x.vec->size());
    if (x.vec->size() != d.vec->size()) {

    }
    for (size_t i = 0; i < x.vec->size(); i++) {
        (*vec)(i) = x(i) * d(i);
    }
    return *vec;
}

// ==============================
//      Vergleichsoperatoren
// ==============================

// ----- Test auf Gleichheit "==" -----

bool operator==(const Vector &x, const Vector &y) {
    if (x.vec->size() != y.vec->size()) return false;

    for (size_t i = 0; i < x.vec->size(); i++) {
        if (x(i) != y(i)) return false;
    }

    return true;
}

// ----- Test auf Ungleichheit "!=" -----

bool operator!=(const Vector &x, const Vector &y) {
    if (x.vec->size() != y.vec->size()) return true;

    for (size_t i = 0; i < x.vec->size(); i++) {
        if (x(i) != y(i)) return true;
    }

    return false;
}

// ==========================
//      Ein- und Ausgabe
// ==========================

// ----- Ausgabe "<<" -----

std::ostream &operator<<(std::ostream &s, const Vector &x) {
    s << std::setiosflags(std::ios::right);
    for (size_t i = 0; i < x.vec->size(); i++) s << "\n(" << std::setw(4) << i << ") " << x(i);

    return s << std::endl;
}

// ----- Eingabe ">>" -----

std::istream &operator>>(std::istream &s, Vector &x) {
    std::cout << std::setiosflags(std::ios::right);
    for (size_t i = 0; i < x.vec->size(); i++) {
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
