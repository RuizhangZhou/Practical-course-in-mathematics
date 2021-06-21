#include "a3.h"

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

//------------------------------------------------------------------------------
// bitte mit Leben fuellen:
//------------------------------------------------------------------------------

// Hilfsfunktionen

void detBinaryRepresentation(lint nExp, vector<int> &x) {
    if (nExp == 0) {
        x.push_back(0);
    } else {
        while (nExp != 0) {
            x.push_back(nExp % 2);
            nExp = nExp / 2;
        }
    }
}

lint fastpow(lint nBase, lint nExp, lint nMod) {
    lint aPowx = 1;
    vector<int> x;
    detBinaryRepresentation(nExp, x);
    lint aPowTwoPowi = nBase % nMod;
    if (x[0] == 1) {
        aPowx = aPowTwoPowi;
    }

    for (int i = 1; i < (int) x.size(); i++) {
        aPowTwoPowi = lint(powl(aPowTwoPowi, 2)) % nMod;
        if (x[i] == 1) {
            aPowx = (aPowx * aPowTwoPowi) % nMod;
        }
    }

    return aPowx;
}

void getDS(lint n, lint &d, lint &s) {
    lint r = 0;
    lint twoPowr = (lint) powl(2, r);

    while ((n - 1) % twoPowr == 0) {
        s = r;
        r++;
        twoPowr = twoPowr * 2;
    }

    lint twoPows = twoPowr / 2;
    d = (n - 1) / twoPows;

    return;
}

// Probedivision

lint trialDivision(lint n, const Sieve &sieve) {
    for (lint i = 2; i < (lint) sqrt((double) n) + 1; i++) {
        if (sieve[i] && n % i == 0) {
            return i;
        }
    }
    return 1;
}

// Fermat

lint fermatTest(lint n, lint a) {
    return fastpow(a, n - 1, n);
}

// Miller-Rabin

lint millerRabinTest(lint n, lint a, lint d, lint r) {
    return fastpow(a, lint(powl(2, r)) * d, n);
}



//------------------------------------------------------------------------------
// Hauptprogramm
//------------------------------------------------------------------------------

int main() {
    Sieve sieve;

    sieve[0] = false;
    sieve[1] = false;

    for (lint i = 2; i < (lint) sqrt((double) Sieve::length()) + 1; i++) {
        if (!sieve[i]) continue;
        for (lint j = 2 * i; j < Sieve::length(); j += i) {
            sieve[j] = false;
        }
    }

    checkSieve(sieve);

    // PrimTest

    for (unsigned int i = 1; i <= num_examples; i++) {
        lint test = getCandidate(DIVISION, i);
        checkSolution(DIVISION, i, trialDivision(test, sieve));
    }


    // Fermat-Test

    for (int i = 1; i <= (int) num_examples; i++) {
        lint n = getCandidate(FERMAT, i);
        int y = 1;

        for (lint a = 1; a < maxPrimeNumber; a++) {
            if (!sieve[a] || n % a == 0) continue;
            y = fermatTest(n, a);
            checkSolution(FERMAT, i, y, a);
            if (y != 1) {
                break;
            }
        }

        if (y == 1) {
            checkSolution(FERMAT, i, 1, 0);
        } else {
            checkSolution(FERMAT, i, 0, 0);
        }
    }

    // Miller-Rabin-Test

    for (int i = 1; i <= (int) num_examples; i++) {
        lint n = getCandidate(MILLER_RABIN, i);
        lint f = 1;
        lint d;
        lint s;
        lint y;

        for (lint a = 1; a < maxPrimeNumber; a++) {
            if (f == 0) {
                break;
            }
            if (sieve[a] && n % a != 0) {
                getDS(n, d, s);
                if (fastpow(a, d, n) != 1) {
                    for (lint r = 0; r < s; r++) {
                        y = millerRabinTest(n, a, d, r);
                        checkSolution(MILLER_RABIN, i, y, a, r);
                        if (y == n - 1) {
                            break;
                        }
                        if (r == s - 1) {
                            f = 0;
                        }
                    }
                }
            }
        }

        checkSolution(MILLER_RABIN, i, f);
    }

    return 0;
}
