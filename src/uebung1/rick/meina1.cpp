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
#include <iostream>
using namespace std;
#include "unit.h" 

// ===== Hauptprogramm =====


int main() {
    // Hier kommt nun Ihr Programm. Viel Erfolg!
    double a,b,c;
    for (int i = 1; i <= num_examples; i++)
    {
        getExample(i,a,b,c);
        cout<<i<<" "<<a<<" "<<b<<" "<<c<<endl;
        if(a==0){
            if(b==0){
                if(c==0){
                    checkSolution(infinity);
                }else{
                    checkSolution(0);
                }
            }else{
                checkSolution(1,false,-c/b);
            }
        }else{
            double p,q,x1,x2,real,imagin,discriminant;
            p=b/a;q=c/a;
            if(abs(p)<2*sqrt(DBL_MAX)){
                discriminant=p*p/4-q;
                if(discriminant==0){
                    checkSolution(1,false,-p/2);
                }else if(c==0){
                    checkSolution(2,false,-p,0);
                }else if(discriminant>0){
                    if(p>=0){
                        x1=-p/2-sqrt(discriminant);
                    }else{
                        x1=-p/2+sqrt(discriminant);
                    }
                    x2=q/x1; 
                    checkSolution(2,false,x1,x2);
                }else{
                    real=-p/2;imagin=sqrt(-discriminant); 
                    checkSolution(2,true,real,imagin);
                    
                }
            }else{
                discriminant=1/4-q/p/p;
                cout<<discriminant<<endl;
                if(discriminant==0){
                    checkSolution(1,false,-p/2);
                }else if(c==0){
                    checkSolution(2,false,-p,0);
                }else if (discriminant>0){
                    if(p>=0){
                        x1=-p/2-abs(p)*sqrt(discriminant);
                    }else{
                        x1=-p/2+abs(p)*sqrt(discriminant);
                    }
                    x2=q/x1; 
                    checkSolution(2,false,x1,x2);
                }else{
                    real=-p/2;imagin=abs(p)*sqrt(-discriminant);
                    checkSolution(2,true,real,imagin);
                }
            }
            
        }
        
    }
    return 0;
}

