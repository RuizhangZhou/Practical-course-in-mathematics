#include "student.h"
#include "unit.h"
#include <fstream>
#include <vector>
#include <memory>
#include <cstring>

using namespace std;

template<typename T>
void read(ifstream &ifs, vector<T> &array) {
    auto t = make_unique<T>();
    while (ifs >> *t) {
        array.push_back(*t);
    }
}

template<typename T>
void print(ostream &ofs, const vector<T> &array) {
    for (const T &element : array) {
        ofs << element << endl;
    }
}

template<typename T>
void swap(vector<T> &array, size_t i, size_t j) {
    auto tmp = make_unique<T>(array[i]);
    array[i] = array[j];
    array[j] = *tmp;
}

template<typename T>
void bubblesort(vector<T> &array) {
    for (size_t i = array.size() - 1; i > 0; i--) {
        for (size_t j = 0; j < i; j++) {
            if (array[j + 1] < array[j]) {
                swap(array, j, j + 1);
            }
        }
    }
}

template<typename T>
void selectionsort(vector<T> &array) {
    for (size_t i = 0; i < array.size() - 1; i++) {
        size_t minPosition = i;
        for (size_t j = i + 1; j < array.size(); j++) {
            if (array[j] < array[minPosition]) {
                minPosition = j;
            }
        }
        swap(array, i, minPosition);
    }
}

template<typename T>
void merge(vector<T> &array1, vector<T> &array2, vector<T> &array) {
    size_t i = 0, j = 0, k = 0;
    while (i < array1.size() && j < array2.size()) {
        if (array1[i] < array2[j]) {
            array[k++] = array1[i++];
        } else {
            array[k++] = array2[j++];
        }
    }
    while (i < array1.size()) {
        array[k++] = array1[i++];
    }
    while (j < array2.size()) {
        array[k++] = array2[j++];
    }
}

template<typename T>
void mergesort(vector<T> &array) {
    if (array.size() <= 1) {
        return;
    }
    size_t l1, l2;
    l1 = (array.size() + 1) / 2;
    l2 = array.size() - l1;
    auto array1 = make_unique<vector<T>>(l1);
    auto array2 = make_unique<vector<T>>(l2);
    for (size_t i = 0; i < l1; i++) {
        (*array1)[i] = array[i];
    }
    for (size_t i = l1; i < array.size(); i++) {
        (*array2)[i - l1] = array[i];
    }
    mergesort(*array1);
    mergesort(*array2);
    merge(*array1, *array2, array);
}

enum Algorithm {
        BUBBLESORT,
        SELECTIONSORT,
        MERGESORT,
        NONE,
    };

int main(int argc, char* argv[]) {
    Algorithm algo = NONE;
    if (argc != 2) {
        cout << "Mit welchem Verfahren soll sortiert werden?" << endl;
        string algoString;
        cin >> algoString;
        if (algoString == "B") {
            algo = BUBBLESORT;
        } else if (algoString == "S") {
            algo = SELECTIONSORT;
        } else if (algoString == "M") {
            algo = MERGESORT;
        }
    } else {
        if (std::strcmp(argv[1], "-b") == 0) {
            algo = BUBBLESORT;
        } else if (std::strcmp(argv[1], "-s") == 0) {
            algo = SELECTIONSORT;
        } else if (std::strcmp(argv[1], "-m") == 0) {
            algo = MERGESORT;
        }
    }

    auto if_stud = make_unique<ifstream>("studenten.txt");
    auto studierende = make_unique<vector<Student>>();
    read(*if_stud, *studierende);

    auto if_double = make_unique<ifstream>("doubles.txt");
    auto doubles = make_unique<vector<double>>();
    read(*if_double, *doubles);

    auto if_string = make_unique<ifstream>("strings.txt");
    auto strings = make_unique<vector<string>>();
    read(*if_string, *strings);

    if (algo == BUBBLESORT) {
        bubblesort(*studierende);
        bubblesort(*doubles);
        bubblesort(*strings);
    } else if (algo == SELECTIONSORT) {
        selectionsort(*studierende);
        selectionsort(*doubles);
        selectionsort(*strings);
    } else if (algo == MERGESORT) {
        mergesort(*studierende);
        mergesort(*doubles);
        mergesort(*strings);
    }

    print(cout, *studierende);
    print(cout, *doubles);
    print(cout, *strings);

    checkSolution(*studierende);
    checkSolution(*doubles);
    checkSolution(*strings);

    return 0;
}