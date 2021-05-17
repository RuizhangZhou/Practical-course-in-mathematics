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

int gsv(Sparse_Matrix &A,const Vector &b,Vector &x0,const int &k_max,double &eps){
    Sparse_Matrix originalA=A;
    if(A.getCols()<1||A.getCols()!=A.getRows()||A.getCols()!=b.getLength()||A.getCols()!=x0.getLength()){
        return -1;
    }
    size_t n=A.getCols();
    
    Vector d(n);
    for(int row=1;row<=n;row++){
        
        d(row-1)=A.get(row,row);//as d ist a str::vector, index begin from 0
        A.put(row,row,0);//A=A_L+A_R, we don't need to creat new A_L and A_R
    }

    int iterCount=0;
    Vector xk=x0;
    Vector residuenvektor;
    while(true){
        residuenvektor=b-A*xk;
        if(iterCount<k_max){
            if(residuenvektor.norm2()>=eps){
                xk=residuenvektor/d;
                iterCount++;
            }else{
                break;
            }
        }else{
            return 0;
        }
        
        
    }
    checkSolution(xk,iterCount,0);//0 for GSV
    A=originalA;
    //Beim gsv darf A modifiziert werden, muss aber beim Verlassen der Funktion wieder wie beim Eingang besetzt sein.
    return iterCount;
}

int main() {
    Sparse_Matrix A;
    Vector x0,b;
    double tol;
    int max_iter;
    for(int id=1;id<=num_examples;id++){
        getExample(id,A,x0,b,tol,max_iter); 
        if(istStrengDiagonalDominant(A)){
            int res=gsv(A,b,x0,max_iter,tol);
            if(res==0){
                cout<<max_iter<<" Iterationen erreicht"<<endl;
            }else if(res==-1){
                cout<<"Dimensionen von A,b und x0 passen nicht zusammen"<<endl;
            }
        }
         











        


    }


    return 0;
}
