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
