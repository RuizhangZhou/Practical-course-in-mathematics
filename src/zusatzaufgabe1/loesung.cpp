#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include "unit.h"

using namespace std;

double detCenterIntegral(double a, double b, double fab2) {
    return (b-a) * fab2;
}

double detTrapezeIntegral(double a, double b, double fa, double fb) {
    return (b-a) * 1.0/2.0 * (fa + fb);
}

double detSimpsonIntegral(double a, double b, double fa, double fb, double fab2) {
    return (b-a) * (1.0/6.0 * fa + 4.0/6.0 * fab2 + 1.0/6.0 * fb);
}

double detIntegralEqui(double a, double b, double epsilon, int &equiCounter,int intervalCounter) {
    double h=(b-a)/intervalCounter;
    double sum=0.5*(f(a)+f(b));
    //equiCounter+=2;//hier f(a) and f(b) calculate f 2 times
    for(int j=1;j<=intervalCounter-1;j++){
        sum+=f(a+j*h);
        //equiCounter++;//every loop calculate f 1 time
    }
    sum*=h;
    return sum;
}

double detIntegralAdap(double a, double b, double fa, double fb, double epsilon, int &adapCounter, int &intervalCounter,double &minDistant) {
    double im, it;
    double fab2 = f((a+b)/2.0);
    adapCounter += 3;
    im = detCenterIntegral(a, b, fab2);
    it = detTrapezeIntegral(a, b, fa, fb);
    if (abs(im-it) <= epsilon) {
        return detSimpsonIntegral(a, b, fa, fb, fab2);
    } else {
        if((b-a)/2.0<minDistant){//I use your recursion to calculate the minimale Schrittweite, which is required for adaptive Algorithmus here,
        //so that I don't need to calculate all the functionsvalue and compare with epsilon again to get this Schrittweite in detIntegralEqui
            intervalCounter*=2;
            minDistant/=2;
        }
        return detIntegralAdap(a, (a+b)/2.0, fa, fab2, epsilon/2.0, adapCounter,intervalCounter,minDistant) 
        + detIntegralAdap((a+b)/2, b, fab2, fb, epsilon/2.0, adapCounter, intervalCounter, minDistant);
    }
}


int main(int argc, char *argv[]) {
    double a, b, epsilon;
    int ex_id;
    double integral;
    int equiCounter = 0, adapCounter = 0,intervalCounter=1;

    cout << "Welches Beispiel soll gerechnet werden?" << endl;
    cin >> ex_id;
    while(cin.fail() || ex_id < 1 || ex_id > num_examples) {
        cout << "Falsche Eingabe. Bitte natürliche Zahl zwischen 1 und " << num_examples << " eingeben." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin >> ex_id;
    }

    getExample(ex_id, a, b, epsilon);
    double minDistant=b-a;
    double fa = f(a), fb = f(b);

    integral = detIntegralAdap(a, b, fa, fb, epsilon, adapCounter, intervalCounter,minDistant);
    checkSolution(integral);
    
    integral = detIntegralEqui(a, b, epsilon, equiCounter,intervalCounter);
    equiCounter=intervalCounter+1;////actually the function f calculates exactly intervalCounter times
    checkSolution(integral);

    cout << "Bei der aequidistanten Unterteilung wurden " << equiCounter - adapCounter << " Funktionsauswertungen mehr benoetigt." << endl;
}