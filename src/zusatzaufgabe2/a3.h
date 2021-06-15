#ifndef _A3_H
#define _A3_H

#include <iostream>

using lint = unsigned long int;

/*----------------------------------------------------------------------------------------

TSieve enthaelt einen langen Vektor von bool und dient hauptsaechlich dazu, die Operatoren
zum Lesen (operator[] const) und Schreiben (operator[]) mit einem Index-Test auszustatten.

Am Besten wie folgt benutzen:

          using Sieve = TSieve(unsigned int, 20000);
                               (Indextyp)  (Laenge)

Auf die Laenge kann man mit

          Sieve::length()

oder
          Sieve sieve;  sieve.length()

zugreifen.

----------------------------------------------------------------------------------------*/

template<class LINT, LINT LEN>
class TSieve {
    enum { len = LEN };

    bool elems[len];

   public:
    TSieve() {
        for (LINT i = 0; i < len; ++i) {
            elems[i] = true;
        }
    }

    bool operator[](LINT n) const {
        if (n >= len) {
            std::cerr << "Falscher Index (" << n << ") in TSieve::op[]" << std::endl;
            exit(-1);
        }
        return elems[n];
    }

    bool& operator[](LINT n) {
        if (n >= len) {
            std::cerr << "Falscher Index (" << n << ") in TSieve::op[]" << std::endl;
            exit(-1);
        }
        return elems[n];
    }

    friend bool operator==(const TSieve& s1, const TSieve& s2) {
        LINT i;
        for (i = 0; (i < TSieve::length()) && (s1[i] == s2[i]); ++i)
            ;
        return (i == TSieve::length());
    }

    static LINT length() {
        return len;
    }
};

using Sieve = TSieve<lint, 100000>;

void checkSieve(const Sieve& sieve);

enum Method { DIVISION, FERMAT, MILLER_RABIN };

extern unsigned int maxPrimeNumber;
extern unsigned int num_examples;
lint getCandidate(Method method, unsigned int ex);
void checkSolution(Method method, unsigned int ex, const lint f, const lint basis = 0, const lint r = 0);

#endif
