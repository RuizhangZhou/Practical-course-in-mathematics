#include <math.h>
#include <iostream>

#include "grayscale.h"
using namespace std;




GreyScale::GreyScale(){
    height=0;
    width=0;
}

GreyScale::GreyScale(int height, int width){
    this->height=height;
    this->width=width;
}

void GreyScale :: resize( int height, int width){
    this->height=height;
    this->width=width;
}




void  GreyScale::put(int i, int j, int x){
    pixels.put(i,j,x);
}

int  GreyScale::get(int i, int j) const{
    return pixels.get(i,j);
}

int& GreyScale::operator () (int i,int j){

}

int GreyScale::operator () (int i,int j) const{

}
// Zuweisung
GreyScale& GreyScale::operator =  (const GreyScale &x){

}  

// Zuweisungen mit arithm. Operation
GreyScale& GreyScale::operator += (const GreyScale &x){

}   

GreyScale& GreyScale::operator -= (const GreyScale &x){

}



GreyScale &GreyScale::binarize(float c){
    GreyScale resPic(getHeight(),getWidth());

    for(int i=0;i<getHeight();i++){
        for(int j=0;j<getWidth();j++){
            if(get(i,j)<c){
                resPic.put(i,j,0);
            }else{
                resPic.put(i,j,1);
            }
        }
    }
    return resPic;
}

GreyScale &GreyScale::clamp(){
    GreyScale resPic(getHeight(),getWidth());

    for(int i=0;i<getHeight();i++){
        for(int j=0;j<getWidth();j++){
            if(get(i,j)<0){
                resPic.put(i,j,0);
            }else if(get(i,j)>1){
                resPic.put(i,j,1);
            }
        }
    }
    return resPic;
}

GreyScale &GreyScale::contrast(){
    int length=getHeight()*getWidth();
    int values[length];
    for(int i =0;i<getHeight();i++){
        for(int j=0;j<getWidth();j++){
            values[i*getWidth()+j]=get(i,j);
        }
    }
    sort(values,length);
    int min=values[0];
    int max=values[length-1];
    //a*min+b=0    a*max+b=1
    float a=1/(max-min);
    float b=-a*min;
    return linTrans(a,b);
}

GreyScale &GreyScale::linTrans(float a , float b){
    GreyScale resPic(getHeight(),getWidth());
    for(int i =0;i<getHeight();i++){
        for(int j=0;j<getWidth();j++){
            resPic.put(i,j,a*get(i,j)+b);
        }
    }
    return resPic;
}

GreyScale &GreyScale::invert(){
    return linTrans(-1,1);
}



GreyScale &GreyScale::convolve(const float mask[], int size=3) {
    if ( sqrt(sizeof(mask)) != size || size % 2 != 1 ) {
        error("Die Anzahl der Einträge der Maske muss der Quadratzahl einer ungeraden Zahl entsprechen.");
    }
    
    int midStaDif = (size - 1)/2;

    GreyScale resPic(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            
            int heiStart = i - midStaDif;
            int widStart = j - midStaDif;
            for (int k = 0; k < size; k++) {
                for (int l = 0; l < size; l++) {
                    resPic(i,j) += (*this)(heiSTart + k, widStart + l) * mask[k+l];
                }
            }
        
        }
    }

    return resPic;
}

GreyScale &GreyScale::blur() {
    float mask[] = {0, 0.2, 0, 0.2, 0.2, 0.2, 0, 0.2, 0};

    GreyScale resPic(getHeight(), getWidth());
    resPic = convolve(mask);

    return resPic;
}

GreyScale &GreyScale::kirsch() {
    float mask[] = {1, 3, 3, -1, 0, 1, -3, -3, -1};

    GreyScale resPic(getHeight(), getWidth());
    resPic = convolve(mask);

    return resPic;
}

GreyScale &GreyScale::laplace() {
    float mask[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};

    GreyScale resPic(getHeight(), getWidth());
    resPic = convolve(mask);

    return resPic;
}

GreyScale &GreyScale::median() {
    GreyScale resPic(getHeight(), getWidth());
    double surr[9] = {0};
    int n = sizeof(surr) / sizeof(surr[0]);
    
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {

            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    surr[k+l] = (*this)(i - 1 + k, j - 1 + l);
                }
            }
            sort(surr, n);

            resPic(i,j) = surr[4];
        }
    }
}

GreyScale &GreyScale::sobel() {
    float DX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float DY[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    picDX = convolve(DX);
    picDY = convolve(DY);

    GreyScale resPic(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            resPic(i,j) = sqrt(pow(picDX(i,j),2) + pow(picDY(i,j),2));
        }
    }

    return resPic;
}

void GreyScale::error(const char str[]) {
    std::cerr << "\nFehler: " << str << '\n' << std::endl;
    std::abort();
}