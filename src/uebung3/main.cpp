#include "unit.h"
#include "vector.h"
#include "sparse_matrix.h"
#include "stdlib.h"


using namespace std;

bool istStrengDiagonalDominant(Sparse_Matrix &A){
    size_t n=A.getCols();

    for(size_t row=0;row<=n-1;row++){
        double sumOfRow=0;
        for(size_t col=0;col<=n-1;col++){
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
    if(A.getCols()<1||A.getCols()!=A.getRows()||A.getCols()!=b.getLength()||A.getCols()!=x0.getLength()){
        return -1;
    }

    size_t n=A.getCols();
    Vector d= Vector(n);//use a vector to record all the elements of diagonal
    Sparse_Matrix originalA=Sparse_Matrix(A);//Kopierkonstuktor
    //but according to the last paragraph in paper, we can't use implement by create extra matrix or vector?
    
    for(size_t row=0;row<=n-1;row++){
        
        d(row)=A.get(row,row);//as d is a str::vector, index begin from 0
        A.put(row,row,0);//set the dianoal to 0
        //not A=A_L+A_R, so that we don't need to creat new A_L and A_R
    }

    int iterCount=0;
    Vector xk=x0;
    Vector zwischenspeichert;
    while(true){
        zwischenspeichert=b-A*xk;//b-(AL+AR)*xk
        if(iterCount>k_max){
            return 0;
        }else{
            Vector Residuenvektor=zwischenspeichert;
            for(size_t i=0;i<=n-1;i++){
                Residuenvektor(i)-=d(i)*xk(i);
            }
            if(Residuenvektor.norm2()>=eps){//here we need to use the zwischenspeichert
            
            //Residuenvektor rk=(b-(AL+AR)*xk)-D*xk=zwischenspeichert-D*xk
                xk=zwischenspeichert/d;
                iterCount++;
            }else{
                break;
            }
        }   
    }
    checkSolution(xk,iterCount,0);//0 for GSV
    A=originalA;
    //Beim gsv darf A modifiziert werden, muss aber beim Verlassen der Funktion wieder wie beim Eingang besetzt sein.
    return iterCount;//here we resume that itercount can't be 0, 
    //namely x0 from getExample isn't already the answer 
}

bool istSymPosDefinit(Sparse_Matrix &A){
    //how to determind if a matrix is sysposdefinit?
    size_t n=A.getCols();
    //determine if it's symmetrisch
    for(size_t row=0;row<=n-1;row++){
        for(size_t col=row+1;col<=n-1;col++){
            if(A.get(row,col)!=A.get(col,row)){
                return false;
            }
        }
    }

    //determine if it's positiv symmetrisch
    






    return true;
}

int cg(const Sparse_Matrix &A,const Vector &b, Vector &x0, const int k_max, double &eps){
    if(A.getCols()<1||A.getCols()!=A.getRows()||A.getCols()!=b.getLength()||A.getCols()!=x0.getLength()){
        return -1;
    }
    size_t n=b.getLength();
    Vector rk=Vector(b-A*x0);
    Vector dk=Vector(rk);
    int iterCount=0;
    Vector xk=Vector(x0);

    double rknorm2=rk.norm2();
    Vector Adk=Vector(n);
    while(true){
        if(iterCount>k_max){
            return 0;
        }else{
            
            if(rknorm2>=eps){//is the residuum hier still with norm2 
            //or should we use Energienorm hier?

                Adk=A*dk;
                double ak=pow(rknorm2,2)/(Adk*dk);//Adk*dk is a skalarprodukt
                xk+=ak*dk;
                rk-=ak*Adk;
                double rk1norm2=rk.norm2();
                double bk=pow(rk1norm2/rknorm2,2);
                rknorm2=rk1norm2;
                dk=rk+bk*dk;


                iterCount++;
            }else{
                break;
            }
        }
        
        
    }
    checkSolution(xk,iterCount,1);
    return iterCount;
}

int main() {
    Sparse_Matrix A;
    Vector x0,b;
    double eps;
    int k_max;
    for(int id=1;id<=num_examples;id++){
        getExample(id,A,x0,b,eps,k_max); 
        
        if(istStrengDiagonalDominant(A)){
            int res=gsv(A,b,x0,k_max,eps);
            if(res==0){
                cout<<"gsv:"<<k_max<<" Iterationen erreicht"<<endl;
            }else if(res==-1){
                cout<<"gsv:Dimensionen von A,b und x0 passen nicht zusammen"<<endl;
            }
        }else{
            cout<<"Beispiel "<<id<<" ist nicht strengdiagonal dominant, also kann nicht mit GSV Verfahren benutzen."<<endl;
        }


        getExample(id,A,x0,b,eps,k_max); 
        if(istSymPosDefinit(A)){
            int res=cg(A,b, x0, k_max, eps);
            if(res==0){
                cout<<"CG:"<<k_max<<" Iterationen erreicht"<<endl;
            }else if(res==-1){
                cout<<"CG:Dimensionen von A,b und x0 passen nicht zusammen"<<endl;
            }
        }else{
            cout<<"Beispiel "<<id<<" ist nicht symmetrisch positiv definit, also kann nicht mit CG Verfahren benutzen."<<endl;
        }

    }
    

    return 0;
}
