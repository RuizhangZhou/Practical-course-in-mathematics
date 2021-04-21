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
    if (length > 1) {
        for (size_t i = 1; i < length; i++) {
            size_t j = i;
            while (j > 0 and array[j - 1] > array[j]) {
                swap(array, j - 1, j);
                j--;
            }
        }
    }
}

void heapsort(size_t &length, unsigned int *&array) {

}

void quicksort_recursive_call(size_t &length, unsigned int *&array, size_t left, size_t right) {
    size_t old_left = left;
    size_t pivot = right;
    right--;
    while (left < right) {
        while (left < pivot and array[left] < array[pivot]) {
            left++;
        }
        while (old_left < right and array[right] >= array[pivot]) {
            right--;
        }
        if (left < right) {
            swap(array, left, right);
        }
    }
    if (array[left] > array[pivot]) {
        swap(array, left, pivot);
    }
    if (left - old_left > 1) {
        quicksort_recursive_call(length, array, old_left, left - 1);
    }
    if (pivot - left > 1) {
        quicksort_recursive_call(length, array, left + 1, pivot);
    }
}

void quicksort(size_t &length, unsigned int *&array) {
    quicksort_recursive_call(length, array, 0, length - 1);
}

void quicksort_median_3_recursive_call(size_t &length, unsigned int *&array, size_t left, size_t right) {
    size_t mid = left + (right - left) / 2;
    if ((array[left] <= array[mid] and array[mid] < array[right]) or (array[right] < array[mid] and array[mid] <= array[left])) {
        swap(array, mid, right);
    } else if ((array[mid] <= array[left] and array[left] < array[right]) or (array[right] < array[left] and array[left] <= array[mid])) {
        swap(array, left, right);
    }

    size_t old_left = left;
    size_t pivot = right;
    right--;
    while (left < right) {
        while (left < pivot and array[left] < array[pivot]) {
            left++;
        }
        while (old_left < right and array[right] >= array[pivot]) {
            right--;
        }
        if (left < right) {
            swap(array, left, right);
        }
    }
    if (array[left] > array[pivot]) {
        swap(array, left, pivot);
    }
    if (left - old_left > 1) {
        quicksort_median_3_recursive_call(length, array, old_left, left - 1);
    }
    if (pivot - left > 1) {
        quicksort_median_3_recursive_call(length, array, left + 1, pivot);
    }
}

void quicksort_median3(size_t &length, unsigned int *&array) {
    quicksort_median_3_recursive_call(length, array, 0, length - 1);
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
