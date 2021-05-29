#include <math.h>
#include <iostream>

using namespace std;

GreyScale &GreyScale::convolve(const float mask[], int size=3) {
    if (sqrt(sizeof(mask))) != size || size % 2 != 1) {
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

    GreyScale resPic(getHeight(), getWdith());
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