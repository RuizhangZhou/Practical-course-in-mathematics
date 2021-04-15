/***********************************************************
 *  Name       : meina1.cpp                                *
 *  Verwendung : Loesung der ersten Praktikumsaufgabe,     *
 *               Nullstellen eines quadratischen Polynoms  *
 *  Autor      :                                           *
 *  Datum      :                                           *
 *  Sprache    : C++                                       *
 ***********************************************************/

// Einbinden der Praktikums-Umgebung. Falls Sie die Ein-/Ausgabe zuerst
// nicht ueber die Praktikumsumgebung machen wollen, sollten Sie auch noch
// #include <iostream> einbinden.

#include "unit.h"
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// ===== Hauptprogramm =====

const double SQRT_DBL_MAX = sqrt(DBL_MAX);

struct Polynomial {
    double a;
    double b;
    double c;

public:
    Polynomial(double a, double b, double c) : a(a), b(b), c(c) {}
};

struct Imaginary {
    double re;
    double im;
};

struct Roots {
    int count = 0;
    vector<Imaginary> roots;
};

unique_ptr<Roots> calculate_roots(const Polynomial &poly) {
    auto roots = make_unique<Roots>();
    if (poly.a == 0 and poly.b == 0) {
        if (poly.c == 0) {
            roots->count = infinity;
        }
    } else if (poly.a == 0) {
        roots->count = 1;
        roots->roots.push_back({(-poly.c) / poly.b, 0});
    } else {
        double p = poly.b / poly.a;
        double q = poly.c / poly.a;

        auto calculate_sqrt_pq = [](double p, double q) -> Imaginary {
            if (abs(p / 2) > SQRT_DBL_MAX) {
                double value = 0.25 - (q / p) / p;
                if (value >= 0) {
                    return {abs(p) * sqrt(value), 0};
                } else {
                    return {0, abs(p) * sqrt(-value)};
                }
            } else {
                double value = pow(p / 2, 2) - q;
                if (value >= 0) {
                    return {sqrt(value), 0};
                } else {
                    return {0, sqrt(-value)};
                }
            }
        };

        Imaginary root = calculate_sqrt_pq(p, q);
        if (root.im != 0) {
            roots->count = 2;
            roots->roots.push_back({-p / 2, root.im});
            roots->roots.push_back({-p / 2, -root.im});
        } else {
            if (root.re == 0) {
                roots->count = 1;
                roots->roots.push_back({-p / 2, 0});
            } else {
                double x1, x2;
                if (p > 0) {
                    x2 = (-p / 2) - root.re;
                    x1 = q / x2;
                } else {
                    x1 = (-p / 2) + root.re;
                    x2 = q / x1;
                }
                roots->count = 2;
                roots->roots.push_back({x1, 0});
                roots->roots.push_back({x2, 0});
            }
        }
    }
    return roots;
}

int main() {
    // Hier kommt nun Ihr Programm. Viel Erfolg!
    cout << "Hallo Welt" << endl;

    cout << "Test" << endl;
    {
        auto polynomial = make_unique<Polynomial>(5, -4, 3);
        auto roots = calculate_roots(*polynomial);

        cout << roots->count << endl;
        for (Imaginary imaginary : roots->roots) {
            cout << imaginary.re;
            cout << " ";
            cout << imaginary.im << endl;
        }
    }

    cout << endl;
    cout << "Test der Beispiele der unit.o" << endl;
    for (int i = 1; i <= num_examples; i++) {
        auto a = make_unique<double>();
        auto b = make_unique<double>();
        auto c = make_unique<double>();
        getExample(i, *a, *b, *c);
        Polynomial polynomial = {*a, *b, *c};
        auto roots = calculate_roots(polynomial);

        if (roots->count == infinity){
            checkSolution(infinity);
        } else if (roots->count == 0) {
            checkSolution(0);
        } else if (roots->count == 1) {
            checkSolution(1, false, roots->roots[0].re);
        } else {
            if (roots->roots[0].im != 0) {
                checkSolution(2, true, roots->roots[0].re, abs(roots->roots[0].im));
            } else {
                if (abs(roots->roots[0].re) >= abs(roots->roots[1].re)) {
                    checkSolution(2, false, roots->roots[0].re, roots->roots[1].re);
                } else {
                    checkSolution(2, false, roots->roots[1].re, roots->roots[0].re);
                }
            }
        }
    }

    return 0;
}
