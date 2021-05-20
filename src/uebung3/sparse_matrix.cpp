#include "sparse_matrix.h"
#include "vector.h"

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>


// =======================
//      Konstruktoren
// =======================

// ----- Konstruktor -----

Sparse_Matrix::Sparse_Matrix(size_t r, size_t c) {
#ifndef NDEBUG
    if (r <= 0 || c <= 0) matError("Die Anzahl der Zeilen und Spalten der Matrix muss positiv sein!");
#endif

    rows = r;
    cols = c;

    hashmap_t mat;
}

// ----- Kopierkonstruktor -----

Sparse_Matrix::Sparse_Matrix(const Sparse_Matrix &M) {
    rows = M.getRows();
    cols = M.getCols();

    for (std::pair<coord_t, double> entry : M.mat) {
       mat[entry.first] = entry.second;
    }
}

// ===========================================
//      Vektorelement schreiben und lesen
// ===========================================

// ----- Schreibzugriff auf Matrixelemente -----

void Sparse_Matrix::put(size_t i, size_t j, double x) {
    coord_t ind(i, j);

    if (x != 0) {
        mat[ind] = x;
    } else {
        mat.erase(ind);
    }

}

// ----- Lesezugriff auf Elemente einer const Matrix -----

double Sparse_Matrix::operator()(size_t i, size_t j) const {
#ifndef NDEBUG
    if (i >= rows || j >= cols) {
        matError("Ungueltige Indizes!");
    }
#endif
    coord_t ind(i, j);
    if (mat.find(ind) == mat.end()) {
        return 0;
    } else {
        return mat.at(ind);
    }
}

double Sparse_Matrix::get(size_t i, size_t j) const {
    return (*this)(i,j);
}


// =====================
//      Zuweisungen
// =====================

// ----- Zuweisungsoperator "=" ----

Sparse_Matrix &Sparse_Matrix::operator=(const Sparse_Matrix &M) {
    (*this).redim(M.getRows(), M.getCols());

    for (std::pair<coord_t, double> entry : M.mat) {
        mat[entry.first] = entry.second;
    }

    return *this;
}

// ----- Zuweisungsoperator mit Addition "+=" ----

Sparse_Matrix &Sparse_Matrix::operator+=(const Sparse_Matrix &M) {

#ifndef NDEBUG
    if (M.getRows() != rows || M.getCols() != cols) {
        matError("Addition nur zwischen Matrizen mit gleicher Zeilen- und Splatenzahl moeglich!");
    }
#endif
    double sum;
    
    for (std::pair<coord_t, double> entry : M.mat) {
        sum = (*this)(entry.first.first, entry.first.second) + M.mat.at(entry.first);
        
        if (sum == 0) {
            mat.erase(entry.first);
        } else {
            mat[entry.first] = sum;
        }

    }

    return *this;
}

// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Sparse_Matrix &Sparse_Matrix::operator-=(const Sparse_Matrix &M) {

#ifndef NDEBUG
    if (M.getRows() != rows || M.getCols() != cols) {
        matError("Subtraktion nur zwischen Matrizen mit gleicher Zeilen- und Spaltenzahl moeglich!");
    }
#endif
    double dif;
    
    for (std::pair<coord_t, double> entry : M.mat) {
        dif = (*this)(entry.first.first, entry.first.second) - M.mat.at(entry.first);
        
        if (dif == 0) {
            mat.erase(entry.first);
        } else {
            mat[entry.first] = dif;
        }

    }

    return *this;
}

// ----- Matrixmultiplikation mit Zuweisungsoperator "*=" ----

Sparse_Matrix &Sparse_Matrix::operator*=(const Sparse_Matrix &M) {

    // implementation not necessary
    return *this;
}

// ----- Multiplikation Matrix Skalar mit Zuweisungsoperator "*=" ----

Sparse_Matrix &Sparse_Matrix::operator*=(double c) {
    if (c == 0) {
        mat.clear();
        return *this;
    }

    for (std::pair<coord_t, double> entry : mat) {
        mat[entry.first] *= c;
    }

    return *this;
}

// ----- Division Matrix Skalar mit Zuweisungsoperator "/=" ----

Sparse_Matrix &Sparse_Matrix::operator/=(double c) {
#ifndef NDEBUG
    if (c == 0) {
        matError("Es darf nicht durch 0 geteilt werden!");
    }
#endif

    for (std::pair<coord_t, double> entry : mat) {
        mat[entry.first] /= c;
    }

    return *this;
}

// ==================================
//        Redimensionierung
// ==================================

// ----- Redimensionierung ----

Sparse_Matrix &Sparse_Matrix::redim(size_t r, size_t c) {
    rows = r;
    cols = c;

    mat.clear();

    return *this;
}

// ==================================
//      arithmetische Operatoren
// ==================================

// ----- Addition "+" -----

Sparse_Matrix operator+(const Sparse_Matrix &A, const Sparse_Matrix &B) {
    Sparse_Matrix C(A);
    C += B;
    return C;
}


// ----- Subtraktion "-" -----

Sparse_Matrix operator-(const Sparse_Matrix &A, const Sparse_Matrix &B) {
    Sparse_Matrix C(A);
    C -= B;
    return C;
}


// ----- Vorzeichen wechseln "-" ----

Sparse_Matrix operator-(const Sparse_Matrix &A) {
    Sparse_Matrix C(A);
    C *= -1;
    return C;
}

// ----- Multiplikation Skalar*Matrix "*" -----

Sparse_Matrix operator*(double c, const Sparse_Matrix &A) {
    Sparse_Matrix C(A);
    C *= c;
    return C;
}

// ----- Multiplikation Matrix*Skalar "*" -----

Sparse_Matrix operator*(const Sparse_Matrix &A, double c){
    Sparse_Matrix C(A);
    C *= c;
    return C;
}

// ----- Division Matrix/Skalar "/" -----

Sparse_Matrix operator/(const Sparse_Matrix& A, double c) {
    Sparse_Matrix C(A);
    C /= c;
    return C;
}

// ----- Multiplikation Matrix*Vektor "*" -----

Vector operator*(const Sparse_Matrix& A, const Vector& v) {
#ifndef NDEBUG
    if (A.getCols() != v.getLength()) {
        Sparse_Matrix::matError("Für die Multiplikation Matrix*Vektor muss die Spaltenzahl der Matrix der Länge des Vektors entsprechen!");
    }
#endif
    
    Vector w(A.getRows());
    
    for (std::pair<coord_t, double> entry : A.mat) {
        w(entry.first.first) += entry.second * v(entry.first.second);
    }

    return w;
}

// ----- Multiplikation Vektor*Matrix "*" -----

Vector operator*(const Vector& v, const Sparse_Matrix& A) {
#ifndef NDEBUG
    if (A.getRows() != v.getLength()) {
        Sparse_Matrix::matError("Für die Multiplikation Vektor*Matrix muss die Zeilenzahl der Matrix der Länge des Vektors entsprechen!");
    }
#endif
    
    Vector w(A.getCols());

    for (std::pair<coord_t, double> entry : A.mat) {
        w(entry.first.second) += entry.second * v(entry.first.first);
    }

    return w;
}

// ==============================
//      Vergleichsoperatoren
// ==============================

// ----- Test auf Gleichheit "==" -----

bool operator==(const Sparse_Matrix &A, const Sparse_Matrix &B) {
    if (A.getRows() != B.getRows() || A.getCols() != B.getCols() || A.mat.size() != B.mat.size()) {
        return false;
    }

    for (std::pair<coord_t, double> entry : A.mat) {
        if (entry.second != B(entry.first.first, entry.first.second)) return false;
    }
    

    return true;
}

bool operator!=(const Sparse_Matrix &A, const Sparse_Matrix &B) {
    return !(A == B);
}

// ==========================
//      Ein- und Ausgabe
// ==========================

// ----- Ausgabe "<<" -----

std::ostream& operator<<(std::ostream &s, const Sparse_Matrix &A) {
    s << std::setiosflags(std::ios::right);
    
    for (size_t i = 0; i < A.getRows(); i++) {
        s << A(i,0);
        for (size_t j = 1; j < A.getCols(); j++) {
            s << "  " << A(i,j);
        }
        s << "\n";
    }

    return s << std::endl;
}

// ----- Eingabe ">>" -----

std::istream& operator>>(std::istream &s, Sparse_Matrix &A) {
    double value;
    std::cout << std::setiosflags(std::ios::right);

    for (size_t i = 0; i < A.getRows(); i++) {
        for (size_t j = 0; j < A.getCols(); j++) {
            std::cout << "\n(" << i << "," << j << ")  ";
            s >> value;
            A.put(i,j,value);
        }
    }

    return s;
}

// ==========================
//      Fehlerbehandlung
// ==========================

// ----- Ausgabe der Fehlermeldung "str" -----

void Sparse_Matrix::matError(const char str[]) {
    std::cerr << "\nMatrixfehler: " << str << '\n' << std::endl;
    std::abort();
}