/***********************************************
 *  Name       : vektor.h                       *
 *  Verwendung : Header zu Vektorklasse         *
 ***********************************************/

#ifndef _VECTOR_H  // vektor.h nicht doppelt benutzen
#define _VECTOR_H

#include <iostream>

class Sparse_Matrix;  // fuer friend Matrix * Vector etc.

class Vector {
   private:
    double* vec;    // Zeiger auf Feld fuer Vektorelemente
    size_t length;  // Vektorlaenge

    static void vecError(const char str[]);  // Fehlermeldung ausgeben

   public:
    explicit Vector(size_t i = 1);  // Konstruktor mit Laengenangabe
    ~Vector();                      // Destruktor
    Vector(const Vector&);          // Kopierkonstruktor

    double& operator()(size_t);       // Standard Zugriff auf Vektorelement
    double operator()(size_t) const;  // Zugriff auf const Vector

    Vector& operator=(const Vector&);   // Zuweisung
    Vector& operator+=(const Vector&);  // Zuweisungen mit arithm. Operation
    Vector& operator-=(const Vector&);
    Vector& operator*=(double);
    Vector& operator/=(double);

    Vector& redim(size_t);  // neue Laenge festlegen
    size_t getLength() const {
        return length;
    }                        // Laenge
    double norm2() const;    // Euklidische Norm des Vektors
    double normMax() const;  // Maximum-Norm des Vektors

    friend Vector operator+(const Vector&, const Vector&);  // Addition
    friend Vector operator-(const Vector&, const Vector&);  // Subtraktion
    friend Vector operator-(const Vector&);                 // Vorzeichen

    friend double operator*(const Vector&, const Vector&);  // Skalarprodukt
    friend Vector operator*(double, const Vector&);         // Vielfache
    friend Vector operator*(const Vector&, double);
    friend Vector operator/(const Vector&, double);
    friend Vector operator/(const Vector&, const Vector&);
    friend Vector operator%(const Vector&, const Vector&);

    friend bool operator==(const Vector&, const Vector&);  // Vergleich
    friend bool operator!=(const Vector&, const Vector&);

    friend std::istream& operator>>(std::istream&, Vector&);        // Eingabe
    friend std::ostream& operator<<(std::ostream&, const Vector&);  // Ausgabe

    friend Vector operator*(const Sparse_Matrix&, const Vector&);  // Matrix-Vector-
    friend Vector operator*(const Vector&, const Sparse_Matrix&);  // Multiplikation
};

#endif
