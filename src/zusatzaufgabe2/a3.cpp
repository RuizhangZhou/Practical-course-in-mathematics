#include "a3.h"

#include <cmath>
#include <iostream>

//------------------------------------------------------------------------------
// bitte mit Leben fuellen:
//------------------------------------------------------------------------------

// Probedivision

lint trialDivision(lint n, const Sieve &sieve) {
    for (lint i = 2; i < (lint) sqrt((double) n) + 1; i++) {
        if (sieve[i] and n % i == 0) {
            return i;
        }
    }
    return 1;
}

// Fermat

lint fermatTest(lint n, lint a) {
    return fastpow(a,n-1,n);
}

// Miller-Rabin

lint millerRabinTest(lint n, lint a, lint d, lint r) {
    return 0;
}

// Hilfsfunktionen

lint fastpow(lint nBase, lint nExp, lint nMod) {
    return 0;
}

lint gcd(lint a, lint b) {
    return 0;
}

void getDS(lint n, lint &d, lint &s) {
    return;
}

//------------------------------------------------------------------------------
// Hauptprogramm
//------------------------------------------------------------------------------

int main() {
    Sieve sieve;

    sieve[0] = false;
    sieve[1] = false;

    for (lint i = 2; i < (lint) sqrt((double) Sieve::length()) + 1; i++) {
        for (lint j = 2 * i; j < Sieve::length(); j += i) {
            sieve[j] = false;
        }
    }

    checkSieve(sieve);

    for (unsigned int i = 1; i <= num_examples; i++) {
        lint test = getCandidate(DIVISION, i);
        checkSolution(DIVISION, i, trialDivision(test, sieve));
    }

    // PrimTest

    // Fermat-Test
    for(int i=1;i<=num_examples;i++){
        lint n=getCandidate(FERMAT,i);
        int y=1;
        
        for (lint a=1;a<maxPrimeNumber;a++){
            if(!sieve[a]||n%a==0) continue;
            y=fermatTest(n,a);
            checkSolution(FERMAT,i,y,a);
            if(y!=1){
                break;
            }
        }

        if(y==1){
            checkSolution(FERMAT,i,1,0);
        }else{
            checkSolution(FERMAT,i,0,0);
        }
    }



    // Miller-Rabin-Test

    return 0;
}
