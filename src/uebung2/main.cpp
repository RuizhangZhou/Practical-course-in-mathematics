#include <iostream>
#include <string>
#include <cstring>
#include "unit.h"

using namespace std;


void bubblesort(size_t &length, unsigned int *&array) {

}

void selectionsort(size_t &length, unsigned int *&array) {

}

void insertionsort(size_t &length, unsigned int *&array) {

}

void heapsort(size_t &length, unsigned int *&array) {

}

void quicksort(size_t &length, unsigned int *&array) {

}

void quicksort_median3(size_t &length, unsigned int *&array) {

}

void mergesort(size_t &length, unsigned int *&array) {

}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    } else {
        if (std::strcmp(argv[1], "-b") == 0) {
            cout << "Bubble sort" << endl;
            auto length = new size_t;
            auto array = new unsigned int *;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                bubblesort(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-s") == 0) {
            cout << "Selection sort" << endl;
            auto length = new size_t;
            auto array = new unsigned int *;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                selectionsort(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-i") == 0) {
            cout << "Insertion sort" << endl;
            auto length = new size_t;
            auto array = new unsigned int *;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                insertionsort(*length, *array);
                cout << *length << endl;
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-h") == 0) {
            cout << "Heapsort" << endl;
            auto length = new size_t;
            auto array = new unsigned int *;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                heapsort(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-q") == 0) {
            cout << "Quicksort" << endl;
            auto length = new size_t;
            auto array = new unsigned int *;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                quicksort(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-q3") == 0) {
            cout << "Quicksort Median 3" << endl;
            auto length = new size_t;
            auto array = new unsigned int *;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                quicksort_median3(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-m") == 0) {
            cout << "Mergesort" << endl;
            auto length = new size_t;
            auto array = new unsigned int *;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                mergesort(*length, *array);
                checkSolution(*array);
            }
        } else {
            return 2;
        }
    }

    return 0;
}
