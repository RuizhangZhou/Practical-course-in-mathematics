#include "greyscale.h"
#include "unit.h"
#include <iostream>

using namespace std;

int main() {
    std::ifstream s("bilder/dom.pgm");

    GreyScale pic;

    s >> pic;
    s.close();

    std::ofstream o("results/out.pgm");

    o << pic;
    o.close();
}