#include "vector.h"

int main() {
    auto test1 = make_unique<Vector>(3);
    auto test2 = make_unique<Vector>(3);
    (*test1)(0) = 2;
    (*test1)(1) = 5;
    (*test2)(1) = 4;
    auto test3 = make_unique<Vector>(*test1 + *test2);
    *test2 += *test1;

    cout << (*test1) * (*test2) << endl;
    cout << test1->normMax() << endl;

    cout << *test1 << endl;
    cout << *test2 << endl;
    cout << *test3 << endl;
    return 0;
}
