#include <cmath>
#include <iostream>
#include <algorithm>

#include "greyscale.h"

using namespace std;


GreyScale::GreyScale() {
    matrix = make_unique<vector<vector<float>>>(0, vector<float>(0));
}

int GreyScale::getHeight() const {
    return (int) matrix->size();
}

int GreyScale::getWidth() const {
    return (int) (matrix->empty() ? 0 : (*matrix)[0].size());
}

GreyScale::GreyScale(int height, int width) {
    matrix = make_unique<vector<vector<float>>>(height, vector<float>(width));
}

void GreyScale::resize(int height, int width) {
    matrix = make_unique<vector<vector<float>>>(height, vector<float>(width));
}

float &GreyScale::operator()(int i, int j) {
    return (*matrix)[i][j];
}

float GreyScale::operator()(int i, int j) const {
    return (*matrix)[i][j];
}

GreyScale::GreyScale(const GreyScale &x) {
    if (x.getWidth() == 0) {
        matrix = make_unique<vector<vector<float>>>(0, vector<float>(0));
    } else {
        matrix = make_unique<vector<vector<float>>>(x.getHeight(), vector<float>(x.getWidth()));
        for (int i = 0; i < getHeight(); i++) {
            for (int j = 0; j < getWidth(); j++) {
                (*this)(i, j) = x(i, j);
            }
        }
    }
}

// Zuweisung
GreyScale &GreyScale::operator=(const GreyScale &x) {

}

// Zuweisungen mit arithm. Operation
GreyScale &GreyScale::operator+=(const GreyScale &x) {

}

GreyScale &GreyScale::operator-=(const GreyScale &x) {

}

GreyScale& GreyScale::operator>>(ifstream s){//Eingabe
    string cur;
    while(s.get()!=' '){//P2
        cur+=s.get();
    }
    s>>ws;

    if(s.peek()=='#'){
        while(s.get()!=' '){//Kommentarzeile
            cur+=s.get();
        }
        s>>ws;
    }

    cur="";//height  
    while(s.get()!=' '){
        cur+=s.get();
    }
    int height=stoi(cur);
    s>>ws;
    cur="";//width
    while(s.get()!=' '){
        cur+=s.get();
    }
    int width=stoi(cur);
    this->resize(height, width);
    s>>ws;

    while(s.get()!=' '){//Graystufen:255
        cur+=s.get();
    }
    s>>ws;

    for(int i=0;i<height;i++){//the pixels
        for(int j=0;j<width;j++){
            cur="";
            while(s.get()!=' '){
                cur+=s.get();
            }
            (*this)(i,j)=stoi(cur)/256;
            s>>ws;
        }
    }
}

GreyScale& GreyScale::operator<<(ofstream s){//Ausgabe
    s.write("P2\n",3);

    string cur=to_string(getHeight())+" "+to_string(getWidth())+"\n";
    s.write(cur.data(),cur.length());

    s.write("255\n",4);
    
    int count=1;
    for(int i=0;i<getHeight();i++){
        for(int j=0;j<getWidth();j++){
            cur=to_string(trunc((*this)(i,j)*256));
            if(cur.length()==1){
                s.write("  ",2);
            }else if(cur.length()==2){
                s.write(" ",1);
            }
            s.write(cur.data(),cur.length());
            s.write(" ",1);

            if(count==16){//every row 16 numbers
                s.write("\n",1);
                count=0;
            }
            count++;
        }
    }
}


GreyScale &GreyScale::binarize(float c) {
    GreyScale resPic(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            if ((*this)(i, j) < c) {
                resPic(i, j) = 0;
            } else {
                resPic(i, j) = 1;
            }
        }
    }
    return resPic;
}

GreyScale &GreyScale::clamp() {
    GreyScale resPic(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            if ((*this)(i, j) < 0) {
                resPic(i, j) = 0;
            } else if ((*this)(i, j) > 1) {
                resPic(i, j) = 1;
            }
        }
    }
    return resPic;
}

GreyScale &GreyScale::contrast() {
    int length = getHeight() * getWidth();
    int values[length];
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            values[i * getWidth() + j] = (*this)(i, j);
        }
    }
    sort(values, values + length);//using the sort() function from <algorithm>
    int min = values[0];
    int max = values[length - 1];
    //a*min+b=0    a*max+b=1
    float a = 1 / (max - min);
    float b = -a * min;
    return linTrans(a, b);
}

GreyScale &GreyScale::linTrans(float a, float b) {
    GreyScale resPic(getHeight(), getWidth());
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            resPic(i, j) = a * (*this)(i, j) + b;
        }
    }
    return resPic;
}

GreyScale &GreyScale::invert() {
    return linTrans(-1, 1);
}


GreyScale &GreyScale::convolve(const float mask[], int size = 3) {
    if (sqrt(sizeof(mask)) != size || size % 2 != 1) {
        error("Die Anzahl der Einträge der Maske muss der Quadratzahl einer ungeraden Zahl entsprechen.");
    }

    int midStaDif = (size - 1) / 2;

    GreyScale resPic(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {

            int heiStart = i - midStaDif;
            int widStart = j - midStaDif;
            for (int k = 0; k < size; k++) {
                for (int l = 0; l < size; l++) {
                    resPic(i, j) += (*this)(heiStart + k, widStart + l) * mask[k + l];
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
                    surr[k + l] = (*this)(i - 1 + k, j - 1 + l);
                }
            }
            sort(surr, n);

            resPic(i, j) = surr[4];
        }
    }
}

GreyScale &GreyScale::sobel() {
    float DX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float DY[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    GreyScale picDX = convolve(DX);
    GreyScale picDY = convolve(DY);

    GreyScale resPic(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            resPic(i, j) = sqrt(pow(picDX(i, j), 2) + pow(picDY(i, j), 2));
        }
    }

    return resPic;
}

void GreyScale::error(const char str[]) {
    std::cerr << "\nFehler: " << str << '\n' << std::endl;
    std::abort();
}
