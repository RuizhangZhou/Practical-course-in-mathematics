/**************************************************
 *  Name       : sparse_matrix.h                  *
 *  Verwendung : Header zu sparser Matrixklasse   *
 *  Autoren    : V. Reichelt, T. Barsch, K. Urban *
 *               IGPM RWTH Aachen                 *
 *  Anpassung:   K. Brix, F. Gruber,              *
 *  Anpassung:   H. Sass, P. Brandner, S. Groß     *
 *  Datum      : Nov '96 - Apr '19                *
 **************************************************/

#ifndef _SPARSE_MATRIX_H  // Sparse_Matrix.h nicht doppelt benutzen
#define _SPARSE_MATRIX_H

#include <iostream>
#include <unordered_map>

class Vector;

using coord_t = std::pair<size_t, size_t>;  // Datentyp des Schlüssels

struct coord_hash {  // Hashfunktion für Schlüsseldatentyp
    std::size_t operator()(coord_t key) const {
        std::hash<size_t> size_t_hash;
        return size_t_hash(key.first) + size_t_hash(key.second);
    }
};

using hashmap_t = std::unordered_map<coord_t, double, coord_hash>;  // Hashmap

class Sparse_Matrix {
   private:
    size_t rows, cols;  // Matrixdimension
    hashmap_t mat;      // Hashmap fuer Matrixelemente

    static void matError(const char str[]);  // Fehlermeldung ausgeben

   public:
    explicit Sparse_Matrix(size_t r = 1, size_t c = 1);  // Konstruktor
    Sparse_Matrix(const Sparse_Matrix&);                 // Kopierkonstruktor

    void put(size_t i, size_t j, double x);   // Matrixelement schreiben
    double operator()(size_t, size_t) const;  // Matrixelement lesen
    double get(size_t i, size_t j) const;

    Sparse_Matrix& operator=(const Sparse_Matrix&);   // Zuweisung
    Sparse_Matrix& operator+=(const Sparse_Matrix&);  // Zuweisungen mit arithm. Operation
    Sparse_Matrix& operator-=(const Sparse_Matrix&);
    Sparse_Matrix& operator*=(const Sparse_Matrix&);
    Sparse_Matrix& operator*=(double);
    Sparse_Matrix& operator/=(double);

    Sparse_Matrix& redim(size_t, size_t);  // neue Dimensionen festlegen
    size_t getRows() const {
        return rows;
    }  // Zeilen
    size_t getCols() const {
        return cols;
    }  // Spalten

    friend Sparse_Matrix operator+(const Sparse_Matrix&, const Sparse_Matrix&);  // Addition
    friend Sparse_Matrix operator-(const Sparse_Matrix&, const Sparse_Matrix&);  // Subtraktion
    friend Sparse_Matrix operator-(const Sparse_Matrix&);                        // Vorzeichen

    friend Sparse_Matrix operator*(double, const Sparse_Matrix&);  // Vielfache
    friend Sparse_Matrix operator*(const Sparse_Matrix&, double);
    friend Sparse_Matrix operator/(const Sparse_Matrix&, double);

    friend bool operator==(const Sparse_Matrix&, const Sparse_Matrix&);  // Vergleich
    friend bool operator!=(const Sparse_Matrix&, const Sparse_Matrix&);

    friend std::istream& operator>>(std::istream&, Sparse_Matrix&);        // Eingabe
    friend std::ostream& operator<<(std::ostream&, const Sparse_Matrix&);  // Ausgabe

    friend Vector operator*(const Sparse_Matrix&, const Vector&);  // Sparse_Matrix-Vector-
    friend Vector operator*(const Vector&, const Sparse_Matrix&);  // Multiplikation
};

#endif
