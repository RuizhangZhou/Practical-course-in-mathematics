#include "unit.h"
#include "vector.h"
#include "sparse_matrix.h"
#include "stdlib.h"


using namespace std;

bool istStrengDiagonalDominant(Sparse_Matrix &A){
    size_t n=A.getCols();

    for(int row=1;row<=n;row++){
        double sumOfRow=0;
        for(int col=1;col<=n;col++){
            if(row!=col){
                sumOfRow+=abs(A.get(row,col));
            }
        }
        if(abs(A.get(row,row))<=sumOfRow){
            return false;
        }
    }
    return true;
}

void gesamtschrittverfahren(Sparse_Matrix &A,Vector &x0,Vector &b,double tol,int &max_iter){
    size_t n=A.getCols();

    //Sparse_Matrix A_L(n,n), A_R(n,n);
    Vector d(n);
    for(int row=1;row<=n;row++){
        // for(int col=1;col<row;col++){
        //     A_L.put(row,col,A.get(row,col));
        //     A_R.put(col,row,A.get(col,row));
        // }
        d(row-1)=A.get(row,row);//as d ist a str::vector, index begin from 0
        A.put(row,row,0);//A=A_L+A_R, we don't need to creat new A_L and A_R
    }

    int iterCount=0;
    Vector xk=x0;
    Vector residuenvektor;
    while(true){
        residuenvektor=b-A*xk;
        if(iterCount<max_iter && residuenvektor.norm2()<tol){
            xk=residuenvektor/d;
            iterCount++;
        }else{
            break;
        }
    }
    checkSolution(xk,iterCount,0);//0 for GSV
}

int main() {
    Sparse_Matrix A;
    Vector x0,b;
    double tol;
    int max_iter;
    for(int id=1;id<=num_examples;id++){
        getExample(id,A,x0,b,tol,max_iter);

        if(istStrengDiagonalDominant(A)){
            gesamtschrittverfahren(A,x0,b,tol,max_iter);
        }












        


    }


    return 0;
}
