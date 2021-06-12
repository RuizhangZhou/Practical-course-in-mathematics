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
    while (s.peek() == '#' || s.peek() == '\n') {
        while (s.get() != '\n') {

        }
        s >> ws;
    }
}

inline int get_number(istream &s) {
    string cur;
    while (isdigit(s.peek())) {
        cur += (char) s.get();
    }
    return stoi(cur);
}

std::istream &operator>>(istream &s, GreyScale &pic) {
    string cur;
    while (s.peek() != '\n') {
        cur += (char) s.get();
    }

    if (cur != "P2") {
        GreyScale::error("No PGM image");
    }

    cout << cur << endl;

    remove_comment(s);

    int height = get_number(s);
    s >> ws;
    if (!isdigit(s.peek())) {
        GreyScale::error("wrong format");
    }
    int width = get_number(s);
    pic.resize(height, width);

    remove_comment(s);
    auto max_val = (float) get_number(s);
    remove_comment(s);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++){
            remove_comment(s);
            pic(i, j) = ((float) get_number(s)) / max_val;
        }
    }
    return s;
}

std::ostream &operator<<(ostream &s, const GreyScale &pic) {
    s.write("P2\n", 3);

    string cur = to_string(pic.getHeight()) + " " + to_string(pic.getWidth()) + "\n";
    s.write(cur.data(), (long) cur.length());

    s.write("255\n", 4);

    for (int i = 0; i < pic.getHeight(); i++) {
        for (int j = 0; j < pic.getWidth(); j++) {
            cur = to_string((int) trunc(pic(i, j) * 255));
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
    return s;
}


GreyScale &GreyScale::binarize(float c) {
    auto resPicPtr = make_unique<GreyScale>(getHeight(), getWidth());

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
    auto resPicPtr = make_unique<GreyScale>(getHeight(), getWidth());

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
            if (cur == 0) {
                min = 0;
            } else if (cur == 1) {
                max = 1;
            }

            if (min == 0 && max == 1) {
                break;
            }

            if (cur > max) {
                max = cur;
            }
            if (cur < min) {
                min = cur;
            }
        }
        if (min == 0 && max == 1) {
            break;
        }
    }

    //a*min+b=0    a*max+b=1
    float a = 1 / (max - min);
    float b = -a * min;
    return linTrans(a, b);
}

GreyScale &GreyScale::linTrans(float a, float b) {
    auto resPicPtr = make_unique<GreyScale>(getHeight(), getWidth());
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
    /*if (sqrt(sizeof(mask)/sizeof(*mask)) != size || size % 2 != 1) {
        error("Die Anzahl der Einträge der Maske muss der Quadratzahl einer ungeraden Zahl entsprechen.");
    }*/

    int midStaDif = (size - 1) / 2;

    auto resPicPtr = make_unique<GreyScale>(getHeight(), getWidth());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {

            int heiStart = i - midStaDif;
            int widStart = j - midStaDif;
            for (int k = 0; k < size; k++) {
                for (int l = 0; l < size; l++) {
                    (*resPicPtr)(i, j) += (*this)(heiStart + k, widStart + l) * mask[k + l];
                }
            }

        }
    }

    return *resPicPtr;
}

GreyScale &GreyScale::blur() {
    float mask[] = {0, 0.2, 0, 0.2, 0.2, 0.2, 0, 0.2, 0};

    auto resPicPtr = make_unique<GreyScale>(getHeight(), getWidth());
    *resPicPtr = convolve(mask);

    return *resPicPtr;
}

GreyScale &GreyScale::kirsch() {
    float mask[] = {1, 3, 3, -1, 0, 1, -3, -3, -1};

    auto resPicPtr = make_unique<GreyScale>(getHeight(), getWidth());
    *resPicPtr = convolve(mask);

    return *resPicPtr;
}

GreyScale &GreyScale::laplace() {
    float mask[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};

    auto resPicPtr = make_unique<GreyScale>(getHeight(), getWidth());
    *resPicPtr = convolve(mask);

    return *resPicPtr;
}

GreyScale &GreyScale::median() {
    auto resPicPtr = make_unique<GreyScale>(getHeight(), getWidth());
    double surr[9] = {0};
    int n = sizeof(surr) / sizeof(surr[0]);

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {

            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    surr[k + l] = (*this)(i - 1 + k, j - 1 + l);
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

    auto resPicPtr = make_unique<GreyScale>(getHeight(), getWidth());

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
