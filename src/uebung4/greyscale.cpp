#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>

#include "greyscale.h"

using namespace std;

typedef const map<unsigned int, unsigned int> Histogram;

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
    if (0 <= i && i < getHeight() && 0 <= j && j < getWidth()) {
        return (*matrix)[i][j];
    } else if (0 <= i && i < getHeight()) {
        if (j < 0) {
            return (*matrix)[i][0];
        } else {
            return (*matrix)[i][getWidth() - 1];
        }
    } else if (0 <= j && j < getWidth()) {
        if (i < 0) {
            return (*matrix)[0][j];
        } else {
            return (*matrix)[getHeight() - 1][j];
        }
    } else {
        if (i < 0 && j < 0) {
            return (*matrix)[0][0];
        } else if (i < 0 && j >= getWidth()) {
            return (*matrix)[0][getWidth() - 1];
        } else if (i >= getHeight() && j < 0) {
            return (*matrix)[getHeight() - 1][0];
        } else {
            return (*matrix)[getHeight() - 1][getWidth() - 1];
        }
    }
}

float GreyScale::operator()(int i, int j) const {
    if (0 <= i && i < getHeight() && 0 <= j && j < getWidth()) {
        return (*matrix)[i][j];
    } else if (0 <= i && i < getHeight()) {
        if (j < 0) {
            return (*matrix)[i][0];
        } else {
            return (*matrix)[i][getWidth() - 1];
        }
    } else if (0 <= j && j < getWidth()) {
        if (i < 0) {
            return (*matrix)[0][j];
        } else {
            return (*matrix)[getHeight() - 1][j];
        }
    } else {
        if (i < 0 && j < 0) {
            return (*matrix)[0][0];
        } else if (i < 0 && j >= getWidth()) {
            return (*matrix)[0][getWidth() - 1];
        } else if (i >= getHeight() && j < 0) {
            return (*matrix)[getHeight() - 1][0];
        } else {
            return (*matrix)[getHeight() - 1][getWidth() - 1];
        }
    }
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
    return *this;
}

// Zuweisungen mit arithm. Operation
GreyScale &GreyScale::operator+=(const GreyScale &x) {
    if (x.getWidth() != getWidth() or x.getHeight() != getHeight()) {
        error("Width or height does not equal.");
    }
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            (*this)(i, j) += x(i, j);
        }
    }
    return *this;
}

GreyScale &GreyScale::operator-=(const GreyScale &x) {
    if (x.getWidth() != getWidth() or x.getHeight() != getHeight()) {
        error("Width or height does not equal.");
    }
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            (*this)(i, j) -= x(i, j);
        }
    }
    return *this;
}

inline void remove_comment(istream &s) {
    s >> ws;
    while (s.good() and (s.peek() == '#' or s.peek() == '\n')) {
        while (s.good() and s.get() != '\n') {

        }
        s >> ws;
    }
}

inline void check_good(istream &s) {
    if (not s.good()) {
        GreyScale::error("Input stream error occurred.");
    }
}

inline int get_number(istream &s) {
    string cur;
    while (isdigit(s.peek())) {
        cur += (char) s.get();
    }
    return stoi(cur);
}

void readOperatorPGM(istream &s, GreyScale &pic,string magicNumber){
    int width = get_number(s);
    s >> ws;
    if (!isdigit(s.peek())) {
        GreyScale::error("wrong format");
    }
    int height = get_number(s);
    pic.resize(height, width);
    remove_comment(s);

    auto max_val = (float) get_number(s);
    remove_comment(s);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            remove_comment(s);
            check_good(s);
            if(magicNumber=="P2"){
                pic(i, j) = ((float) get_number(s)) / max_val;
            }else if(magicNumber=="P5"){
                pic(i,j)=((float) s.get()/max_val);
                //the return value of get() is int, 
                //so hier can automatically transfer the char to corresponding ASCII code?
            }
        }
    }
    
    remove_comment(s);
    if (not s.eof()) {
        GreyScale::error("Has not reached end of file.");
    }
}

void constHuffCode(Histogram &histogram, GreyScale::Node &root) {

}

void readTransformation(GreyScale &pic) {
    
}

void readOperatorMHa(istream &s, GreyScale &pic){

}

void readOperatorMHb(istream &s, GreyScale &pic){

}

std::istream &operator>>(istream &s, GreyScale &pic) {
    string cur;
    while (s.peek() != '\n') {
        cur += (char) s.get();
    }
    remove_comment(s);

    if(cur=="P2"||cur=="P5"){
        readOperatorPGM(s,pic,cur);
    }else if (cur=="MHa"){
        readOperatorMHa(s,pic);
    }else if (cur=="MHb"){
        readOperatorMHa(s,pic);
    }else{
        GreyScale::error("No PGM or MH image");
    }

    return s;
}

void writeOperatorP2(ostream &s,const GreyScale &pic){
    s.write("P2\n", 3);

    string cur = to_string(pic.getWidth()) + " " + to_string(pic.getHeight()) + "\n";
    s.write(cur.data(), (long) cur.length());

    s.write("255\n", 4);

    for (int i = 0; i < pic.getHeight(); i++) {
        for (int j = 0; j < pic.getWidth(); j++) {
            if (pic(i, j) < 0) {
                cur = "0";
            } else if (pic(i, j) > 1) {
                cur = "255";
            } else {
                cur = to_string((int) round(pic(i, j) * 255));
            }

            if (cur.length() == 1) {//0-9, add two space before number
                s.write("  ", 2);
            } else if (cur.length() == 2) {//10-99, add one space before number
                s.write(" ", 1);
            }
            s.write(cur.data(), (long) cur.length());
            s.write(" ", 1);
        }
        s.write("\n", 1);
    }
}

void writeOperatorP5(ostream &s, const GreyScale &pic){
    s.write("P5\n", 3);
    
    string cur = to_string(pic.getWidth()) + " " + to_string(pic.getHeight()) + "\n";
    s.write(cur.data(), (long) cur.length());

    s.write("255\n", 4);

    for (int i = 0; i < pic.getHeight(); i++) {
        for (int j = 0; j < pic.getWidth(); j++) {
            unsigned char curChar;
            if(pic(i,j)<0){
                curChar=255;
            }else if(pic(i, j) > 1){
                curChar=0;
            }else{
                curChar=(int)round(pic(i, j) * 255);
            }
            s.put(curChar);//ostream& put (char c);
        }
        s.write("\n", 1);
    }
}

void calcHistogram(Histogram &histogram, const GreyScale &pic) {

}

void valueToCode(GreyScale::Node &tree, Histogram &histogram) {

}

void writeTransformation(GreyScale &pic) {

}

void writeOperatorMHa(ostream &s, const GreyScale &pic){
      
}

void writeOperatorMHb(ostream &s, const GreyScale &pic){

}

std::ostream &operator<<(ostream &s, const GreyScale &pic) {
    switch (pic.format){
        case 0://why I can't use the enum P2 here?
            writeOperatorP2(s,pic);
            break;
        case 1://
            writeOperatorP5(s,pic);
            break;
        case 2:
            writeOperatorMHa(s,pic);
            break;
        case 3:
            writeOperatorMHb(s,pic);
            break;
        default:
            break;
    }

    return s;
}


GreyScale &GreyScale::binarize(float c) {
    GreyScale *resPicPtr = new GreyScale(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            if ((*this)(i, j) < c) {
                (*resPicPtr)(i, j) = 0;
            } else {
                (*resPicPtr)(i, j) = 1;
            }
        }
    }
    return *resPicPtr;
}

GreyScale &GreyScale::clamp() {
    GreyScale *resPicPtr = new GreyScale(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            if ((*this)(i, j) < 0) {
                (*resPicPtr)(i, j) = 0;
            } else if ((*this)(i, j) > 1) {
                (*resPicPtr)(i, j) = 1;
            }
        }
    }
    return *resPicPtr;
}

GreyScale &GreyScale::contrast() {
    float min = 1;
    float max = 0;
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            float cur = (*this)(i, j);

            if (cur > max) {
                max = cur;
            }
            if (cur < min) {
                min = cur;
            }
        }
    }

    //a*min+b=0    a*max+b=1
    float a = 1.0 / (max - min);
    float b = -a * min;
    return linTrans(a, b);
}

GreyScale &GreyScale::linTrans(float a, float b) {
    GreyScale *resPicPtr = new GreyScale(getHeight(), getWidth());
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            (*resPicPtr)(i, j) = a * (*this)(i, j) + b;
        }
    }
    return *resPicPtr;
}

GreyScale &GreyScale::invert() {
    return linTrans(-1, 1);
}


GreyScale &GreyScale::convolve(const float mask[], int size = 3) {

    int midStaDif = (size - 1) / 2;

    GreyScale *resPicPtr = new GreyScale(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {

            int heiStart = i - midStaDif;
            int widStart = j - midStaDif;
            for (int k = 0; k < size; k++) {
                for (int l = 0; l < size; l++) {
                    (*resPicPtr)(i, j) += (*this)(heiStart + k, widStart + l) * mask[k * size + l];
                }
            }
        }
    }

    return *resPicPtr;
}

GreyScale &GreyScale::blur() {
    float mask[] = {0, 0.2, 0, 0.2, 0.2, 0.2, 0, 0.2, 0};

    GreyScale *resPicPtr = new GreyScale(getHeight(), getWidth());
    *resPicPtr = convolve(mask);

    return *resPicPtr;
}

GreyScale &GreyScale::kirsch() {
    float mask[] = {1, 3, 3, -1, 0, 1, -3, -3, -1};

    GreyScale *resPicPtr = new GreyScale(getHeight(), getWidth());
    *resPicPtr = convolve(mask);

    return *resPicPtr;
}

GreyScale &GreyScale::laplace() {
    float mask[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};

    GreyScale *resPicPtr = new GreyScale(getHeight(), getWidth());
    *resPicPtr = convolve(mask);

    return *resPicPtr;
}

GreyScale &GreyScale::median() {
    GreyScale *resPicPtr = new GreyScale(getHeight(), getWidth());
    float surr[9] = {0};
    int n = sizeof(surr) / sizeof(surr[0]);


    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {

            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    surr[3 * k + l] = (*this)(i - 1 + k, j - 1 + l);
                }
            }

            sort(surr, surr + n);
            (*resPicPtr)(i, j) = surr[4];
        }
    }
    return *resPicPtr;
}

GreyScale &GreyScale::sobel() {
    float DX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float DY[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    GreyScale picDX = convolve(DX);
    GreyScale picDY = convolve(DY);

    GreyScale *resPicPtr = new GreyScale(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            (*resPicPtr)(i, j) = sqrt(pow(picDX(i, j), 2) + pow(picDY(i, j), 2));
        }
    }

    return (*resPicPtr);
}

void GreyScale::error(const char str[]) {
    std::cerr << "\nFehler: " << str << '\n' << std::endl;
    std::abort();
}

void GreyScale::setFormat(int i){
    switch (i){
        case 0:
            format=P2;
            break;
        case 1:
            format=P5;
            break;
        case 2:
            format=MHa;
            break;
        case 3:
            format=MHb;
            break;
        default:
            GreyScale::error("SetNumber shoulb be in 0-3.");
            break;
    }
}
