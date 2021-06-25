#ifndef _GRAYSCALE_H
#define _GRAYSCALE_H

#include <vector>
#include <memory>
#include <fstream>

using namespace std;

class GreyScale {

private:

    unique_ptr<vector<vector<float>>> matrix;

public:
    enum format_set{//am I right to write the enum declaration here?
        P2,
        P5,
        MHa,
        MHb
    };

    static format_set format;

    struct Node {
        byte value;
        unsigned int freq;
        shared_ptr<Node> p0, p1;

        bool operator>(const Node &other) const {
            return freq > other.freq or (freq == other.freq and value > other.value);
        }
    };

    GreyScale();

    GreyScale(int height, int width);

    GreyScale(const GreyScale &x);

    void resize(int height, int width);

    int getHeight() const;

    int getWidth() const;

    float &operator()(int, int);         // Standard Zugriff auf GrayScaleelement
    float operator()(int, int) const;   // Zugriff auf const GrayScale

    GreyScale &operator=(const GreyScale &);   // Zuweisung
    GreyScale &operator+=(const GreyScale &);   // Zuweisungen mit arithm. Operation
    GreyScale &operator-=(const GreyScale &);


    friend std::istream &operator>>(std::istream &, GreyScale &);        // Eingabe
    friend std::ostream &operator<<(std::ostream &, const GreyScale &);  // Ausgabe


    GreyScale &binarize(float c);

    GreyScale &clamp();

    GreyScale &contrast();

    GreyScale &linTrans(float a, float b);

    GreyScale &invert();


    GreyScale &convolve(const float mask[], int size);

    GreyScale &blur();

    GreyScale &kirsch();

    GreyScale &laplace();

    GreyScale &median();

    GreyScale &sobel();

    static void error(const char str[]);

    static void setFormat(int);
};


#endif