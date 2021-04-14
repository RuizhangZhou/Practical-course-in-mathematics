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
#include <cmath>
#include <iostream>
using namespace std;

// ===== Hauptprogramm =====

struct polynom {
        double a, b, c;
        int num;
        bool is_complex;
        double x1, x2;
};

void determineZeros(polynom &pol) {
    
    if(pol.a == 0) {
        pol.is_complex = false;
        if(pol.b == 0) {
            if(pol.c == 0) {
                pol.num = infinity;
                return;
            } else {
                pol.num = 0;
                return;
            }
        } else {
            pol.num = 1;
            pol.x1 = -(pol.c/pol.b);
            return;
        }
    } else {  

        double p = pol.b/pol.a;
        double q = pol.c/pol.a;
        double squareroot;
        if(abs(p)/2 <= sqrt(DBL_MAX)) {
            if(pow(p/2,2)-q >= 0) {
                pol.is_complex = false;
                squareroot = sqrt(pow(p/2,2)-q);
            } else {
                pol.is_complex = true;
                pol.num = 2;
                pol.x1 = -(p/2);
                pol.x2 = sqrt(-(pow(p/2,2)-q));
                return;
            }
        } else {
            pol.is_complex = false;
            squareroot = abs(p) * sqrt(0.25-(q/p)/p);
        }
        
        if(p > 0) {
            pol.x2 = -(p/2) - squareroot;
            pol.x1 = q/pol.x2;
        } else if (p <0) {
            pol.x1 = -(p/2) + squareroot;
            pol.x2 = q/pol.x1;
        } else {
            pol.x1 = squareroot;
            pol.x2 = -squareroot;
        } 

        if (pol.x1 == pol.x2) {
            pol.num = 1;
            return;
        } else {
            pol.num = 2;
            return;
        }
    }
}

int main() {
    // Hier kommt nun Ihr Programm. Viel Erfolg!
    polynom pol;
    for(int i = 1; i <= num_examples; i++) {
        getExample(i, pol.a, pol.b, pol.c);
        determineZeros(pol);
        
        if(pol.num == infinity) {
            checkSolution(infinity);
        } else if(pol.num == 0) {
            checkSolution(0);
        } else if(pol.num == 1) {
            checkSolution(1, false, pol.x1);
        } else if(pol.num == 2) {
            if(!pol.is_complex) {
                checkSolution(2, false, pol.x1, pol.x2);
            } else {
                checkSolution(2, true, pol.x1, pol.x2);
            }
        }
    }
        
    return 0;
}