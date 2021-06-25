#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>
#include <queue>

#include "greyscale.h"

using namespace std;

typedef vector<unsigned int> Histogram;

GreyScale::GreyScale() {
    matrix = make_unique<vector<vector<float>>>(0, vector<float>(0));
}

int GreyScale::getHeight() const {
    return (int) matrix->size();
}

int GreyScale::getWidth() const {
    return (int) (matrix->empty() ? 0 : (*matrix)[0].size());
}

GreyScale::GreyScale(int width, int height) {
    matrix = make_unique<vector<vector<float>>>(height, vector<float>(width));
}

void GreyScale::resize(int width, int height) {
    matrix = make_unique<vector<vector<float>>>(height, vector<float>(width));
}


float &GreyScale::operator()(int column, int row) {
    if (0 <= row && row < getHeight() && 0 <= column && column < getWidth()) {
        return (*matrix)[row][column];
    } else if (0 <= row && row < getHeight()) {
        if (column < 0) {
            return (*matrix)[row][0];
        } else {
            return (*matrix)[row][getWidth() - 1];
        }
    } else if (0 <= column && column < getWidth()) {
        if (row < 0) {
            return (*matrix)[0][column];
        } else {
            return (*matrix)[getHeight() - 1][column];
        }
    } else {
        if (row < 0 && column < 0) {
            return (*matrix)[0][0];
        } else if (row < 0 && column >= getWidth()) {
            return (*matrix)[0][getWidth() - 1];
        } else if (row >= getHeight() && column < 0) {
            return (*matrix)[getHeight() - 1][0];
        } else {
            return (*matrix)[getHeight() - 1][getWidth() - 1];
        }
    }
}

float GreyScale::operator()(int column, int row) const {
    if (0 <= row && row < getHeight() && 0 <= column && column < getWidth()) {
        return (*matrix)[row][column];
    } else if (0 <= row && row < getHeight()) {
        if (column < 0) {
            return (*matrix)[row][0];
        } else {
            return (*matrix)[row][getWidth() - 1];
        }
    } else if (0 <= column && column < getWidth()) {
        if (row < 0) {
            return (*matrix)[0][column];
        } else {
            return (*matrix)[getHeight() - 1][column];
        }
    } else {
        if (row < 0 && column < 0) {
            return (*matrix)[0][0];
        } else if (row < 0 && column >= getWidth()) {
            return (*matrix)[0][getWidth() - 1];
        } else if (row >= getHeight() && column < 0) {
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
                (*this)(j, i) = x(j, i);
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
                (*this)(j, i) = x(j, i);
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
            (*this)(j, i) += x(j, i);
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
            (*this)(j, i) -= x(j, i);
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

void readOperatorPGM(istream &s, GreyScale &pic, string magicNumber) {
    int width = get_number(s);
    s >> ws;
    if (!isdigit(s.peek())) {
        GreyScale::error("wrong format");
    }
    int height = get_number(s);
    pic.resize(width, height);
    remove_comment(s);

    auto max_val = (float) get_number(s);
    remove_comment(s);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            remove_comment(s);
            check_good(s);
            if (magicNumber == "P2") {
                pic(j, i) = ((float) get_number(s)) / max_val;
            } else if (magicNumber == "P5") {
                pic(j, i) = ((float) s.get() / max_val);
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

inline unsigned long read_number(istream &s, int number_of_bytes = 4) {
    unsigned int i = 0;
    unsigned char c = 0;
    for (int j = 0; j < number_of_bytes; j++) {
        i <<= 8;
        s >> c;
        i |= c;
    }
    return i;
}

void constHuffCode(const Histogram &histogram, GreyScale::Node &root) {
    auto flipped_histogram = make_unique<priority_queue<GreyScale::Node, vector<GreyScale::Node>, greater<>>>();
    for (int i = 0; i < 256; i++) {
        if (histogram[i] != 0) {
            auto node = make_unique<GreyScale::Node>();
            node->value = (byte) i;
            node->freq = histogram[i];
            flipped_histogram->push(*node);
        }
    }
    while (flipped_histogram->size() > 2) {
        auto node1 = make_unique<GreyScale::Node>(flipped_histogram->top());
        flipped_histogram->pop();
        auto node2 = make_unique<GreyScale::Node>(flipped_histogram->top());
        flipped_histogram->pop();
        auto node = make_unique<GreyScale::Node>();
        node->value = min(node1->value, node2->value);
        node->freq = node1->freq + node2->freq;
        node->p0 = move(node1);
        node->p1 = move(node2);
        flipped_histogram->push(*node);
    }
    auto node1 = make_unique<GreyScale::Node>(flipped_histogram->top());
    flipped_histogram->pop();
    auto node2 = make_unique<GreyScale::Node>(flipped_histogram->top());
    flipped_histogram->pop();
    root.value = min(node1->value, node2->value);
    root.freq = node1->freq + node2->freq;
    root.p0 = move(node1);
    root.p1 = move(node2);
}

void readTransformation(GreyScale &pic) {

}

void readOperatorMHa(istream &s, GreyScale &pic) {
    int width = read_number(s, 2);
    int height = read_number(s, 2);
    pic.resize(width, height);
    auto histogram = Histogram();
    for (int i = 0; i < 256; i++) {
        histogram[i] = read_number(s);
    }
    auto tree = make_shared<GreyScale::Node>();
    constHuffCode(histogram, *tree);
    int pos = 0;
    unsigned char current_byte;
    auto current_node = tree;
    while (s.good() and pos < height * width) {
        s >> current_byte;
        for (unsigned char i = 0; i < 8 and pos < height * width; i++) {
            tree = current_byte % 2 ? tree->p0 : tree->p1;
            current_byte >>= 1;
            if (current_node->p0 == nullptr) {
                pic(pos % width, pos / width) = ((float) current_node->value) / 255;
                current_node = tree;
                pos++;
            }
        }
    }
}

void readOperatorMHb(istream &s, GreyScale &pic) {

}

std::istream &operator>>(istream &s, GreyScale &pic) {
    char c;
    s >> c;
    if (c == 'P') {
        s >> c;
        if (c == '2') {
            readOperatorP2(s, pic);
        } else if (c == '5') {
            readOperatorP5(s, pic);
        } else {
            GreyScale::error("Wrong format");
        }
    } else if (c == 'M') {
        s >> c;
        if (c == 'H') {
            s >> c;
            if (c == 'a') {
                readOperatorMHa(s, pic);
            } else if (c == 'b') {
                readOperatorMHb(s, pic);
            } else {
                GreyScale::error("Wrong format");
            }
        } else {
            GreyScale::error("Wrong format");
        }
    } else {
        GreyScale::error("Wrong format");
    }

    return s;
}

void writeOperatorP2(ostream &s, const GreyScale &pic) {
    s.write("P2\n", 3);

    string cur = to_string(pic.getWidth()) + " " + to_string(pic.getHeight()) + "\n";
    s.write(cur.data(), (long) cur.length());

    s.write("255\n", 4);

    for (int i = 0; i < pic.getHeight(); i++) {
        for (int j = 0; j < pic.getWidth(); j++) {
            if (pic(j, i) < 0) {
                cur = "0";
            } else if (pic(j, i) > 1) {
                cur = "255";
            } else {
                cur = to_string((int) round(pic(j, i) * 255));
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

void writeOperatorP5(ostream &s, const GreyScale &pic) {
    s.write("P5\n", 3);

    string cur = to_string(pic.getWidth()) + " " + to_string(pic.getHeight()) + "\n";
    s.write(cur.data(), (long) cur.length());

    s.write("255\n", 4);

    for (int i = 0; i < pic.getHeight(); i++) {
        for (int j = 0; j < pic.getWidth(); j++) {
            unsigned char curChar;
            if (pic(j, i) < 0) {
                curChar = 255;
            } else if (pic(j, i) > 1) {
                curChar = 0;
            } else {
                curChar = (int) round(pic(j, i) * 255);
            }
            s.put(curChar);//ostream& put (char c);
        }
        s.write("\n", 1);
    }
}

void calcHistogram(Histogram &histogram, const GreyScale &pic) {

}

void valueToCode(GreyScale::Node &tree, vector<unsigned int> &histogram) {

}

void writeTransformation(GreyScale &pic) {

}

void writeOperatorMHa(ostream &s, const GreyScale &pic) {

}

void writeOperatorMHb(ostream &s, const GreyScale &pic) {

}

std::ostream &operator<<(ostream &s, const GreyScale &pic) {
    switch (pic.format) {
        case GreyScale::P2:
            writeOperatorP2(s, pic);
            break;
        case GreyScale::P5:
            writeOperatorP5(s, pic);
            break;
        case GreyScale::MHa:
            writeOperatorMHa(s, pic);
            break;
        case GreyScale::MHb:
            writeOperatorMHb(s, pic);
            break;
        default:
            break;
    }

    return s;
}


GreyScale &GreyScale::binarize(float c) {
    GreyScale *resPicPtr = new GreyScale(getWidth(), getHeight());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            if ((*this)(j, i) < c) {
                (*resPicPtr)(j, i) = 0;
            } else {
                (*resPicPtr)(j, i) = 1;
            }
        }
    }
    return *resPicPtr;
}

GreyScale &GreyScale::clamp() {
    GreyScale *resPicPtr = new GreyScale(getWidth(), getHeight());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            if ((*this)(j, i) < 0) {
                (*resPicPtr)(j, i) = 0;
            } else if ((*this)(j, i) > 1) {
                (*resPicPtr)(j, i) = 1;
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
            float cur = (*this)(j, i);

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
    GreyScale *resPicPtr = new GreyScale(getWidth(), getHeight());
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            (*resPicPtr)(j, i) = a * (*this)(j, i) + b;
        }
    }
    return *resPicPtr;
}

GreyScale &GreyScale::invert() {
    return linTrans(-1, 1);
}


GreyScale &GreyScale::convolve(const float mask[], int size = 3) {

    int midStaDif = (size - 1) / 2;

    GreyScale *resPicPtr = new GreyScale(getWidth(), getHeight());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {

            int heiStart = i - midStaDif;
            int widStart = j - midStaDif;
            for (int k = 0; k < size; k++) {
                for (int l = 0; l < size; l++) {
                    (*resPicPtr)(j, i) += (*this)(widStart + l, heiStart + k) * mask[k * size + l];
                }
            }
        }
    }

    return *resPicPtr;
}

GreyScale &GreyScale::blur() {
    float mask[] = {0, 0.2, 0, 0.2, 0.2, 0.2, 0, 0.2, 0};

    GreyScale *resPicPtr = new GreyScale(getWidth(), getHeight());
    *resPicPtr = convolve(mask);

    return *resPicPtr;
}

GreyScale &GreyScale::kirsch() {
    float mask[] = {1, 3, 3, -1, 0, 1, -3, -3, -1};

    GreyScale *resPicPtr = new GreyScale(getWidth(), getHeight());
    *resPicPtr = convolve(mask);

    return *resPicPtr;
}

GreyScale &GreyScale::laplace() {
    float mask[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};

    GreyScale *resPicPtr = new GreyScale(getWidth(), getHeight());
    *resPicPtr = convolve(mask);

    return *resPicPtr;
}

GreyScale &GreyScale::median() {
    GreyScale *resPicPtr = new GreyScale(getWidth(), getHeight());
    float surr[9] = {0};
    int n = sizeof(surr) / sizeof(surr[0]);


    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {

            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    surr[3 * k + l] = (*this)(j - 1 + l, i - 1 + k);
                }
            }

            sort(surr, surr + n);
            (*resPicPtr)(j, i) = surr[4];
        }
    }
    return *resPicPtr;
}

GreyScale &GreyScale::sobel() {
    float DX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float DY[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    GreyScale picDX = convolve(DX);
    GreyScale picDY = convolve(DY);

    GreyScale *resPicPtr = new GreyScale(getWidth(), getHeight());

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            (*resPicPtr)(j, i) = sqrt(pow(picDX(j, i), 2) + pow(picDY(j, i), 2));
        }
    }

    return (*resPicPtr);
}

void GreyScale::error(const char str[]) {
    std::cerr << "\nFehler: " << str << '\n' << std::endl;
    std::abort();
}

void GreyScale::setFormat(int i) {
    switch (i) {
        case 0:
            format = P2;
            break;
        case 1:
            format = P5;
            break;
        case 2:
            format = MHa;
            break;
        case 3:
            format = MHb;
            break;
        default:
            GreyScale::error("SetNumber shoulb be in 0-3.");
            break;
    }
}

bool GreyScale::Node::operator>(const Node &other) const {
    return freq > other.freq or (freq == other.freq and value > other.value);
}

void GreyScale::Node::print() {
    print(0);
}

string GreyScale::Node::spaces(int spaces) {
    string s;
    for (int i = 0; i < spaces; i++) {
        s += " ";
    }
    return s;
}

void GreyScale::Node::print(int level) {
    cout << spaces(level * 2) << "{" << endl;
    cout << spaces(level * 2 + 2) << to_string((int) value) << endl;
    cout << spaces(level * 2 + 2) << to_string(freq) << endl;
    if (p0 != nullptr) {
        p0->print(level + 1);
    } else {
        cout << spaces(level * 2 + 2) << "NULL" << endl;
    }
    if (p1 != nullptr) {
        p1->print(level + 1);
    } else {
        cout << spaces(level * 2 + 2) << "NULL" << endl;
    }
    cout << spaces(level * 2) << "}" << endl;
}
