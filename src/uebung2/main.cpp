#include <iostream>
#include <string>
#include <cstring>
#include "unit.h"

using namespace std;


void bubblesort(size_t &length, unsigned int *&array) {
    for (size_t i = length - 1; i > 0; i--) {
        for (size_t j = 0; j < i; j++) {
            if (array[j] > array[j + 1]) {
                swap(array, j, j + 1);
            }
        }
    }
}

void selectionsort(size_t &length, unsigned int *&array) {
    for (size_t i = 0; i < length - 1; i++) {
        size_t minPosition = i;
        for (size_t j = i + 1; j < length; j++) {
            if (array[j] < array[minPosition]) {
                minPosition = j;
            }
        }
        swap(array, i, minPosition);
    }
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

void heapify(size_t &length, unsigned int *&array, size_t i) {
    size_t maxPosition = i;

    if (2 * i + 1 < length && array[2 * i + 1] > array[maxPosition]) {
        maxPosition = 2 * i + 1;
    }
    if (2 * i + 2 < length && array[2 * i + 2] > array[maxPosition]) {
        maxPosition = 2 * i + 2;
    }

    if (maxPosition != i) {
        swap(array, maxPosition, i);
        heapify(length, array, maxPosition);
    }
}

void heapsort(size_t &length, unsigned int *&array) {
    size_t leftSize = length;

    for (int i = (int) length / 2 - 1; i >= 0; i--) {
        heapify(length, array, i);
    }

    while (leftSize > 1) {
        swap(array, 0, leftSize - 1);
        leftSize--;
        heapify(leftSize, array, 0);
    }
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
    if ((array[left] <= array[mid] and array[mid] < array[right]) or
        (array[right] < array[mid] and array[mid] <= array[left])) {
        swap(array, mid, right);
    } else if ((array[mid] <= array[left] and array[left] < array[right]) or
               (array[right] < array[left] and array[left] <= array[mid])) {
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

void merge(size_t l1, size_t l2, unsigned int *&array1, unsigned int *&array2, unsigned int *&array) {
    size_t i = 0, j = 0, k = 0;
    while (i < l1 && j < l2) {
        if (array1[i] <= array2[j]) {
            array[k++] = array1[i++];
        } else {
            array[k++] = array2[j++];
        }
    }
    while (i < l1) {
        array[k++] = array1[i++];
    }
    while (j < l2) {
        array[k++] = array2[j++];
    }
}

void mergesort(size_t &length, unsigned int *&array) {
    if (length <= 1) {
        return;
    }
    size_t l1, l2;
    unsigned int *array1, *array2;
    l1 = (length + 1) / 2;
    l2 = length - l1;
    array1 = new unsigned int[l1];
    array2 = new unsigned int[l2];
    for (size_t i = 0; i < l1; i++) {
        array1[i] = array[i];
    }
    for (size_t i = l1; i < length; i++) {
        array2[i - l1] = array[i];
    }
    mergesort(l1, array1);
    mergesort(l2, array2);
    merge(l1, l2, array1, array2, array);

    delete[] array1;
    delete[] array2;
}

void print_usage() {
    cerr << "Usage: <program name> <x> [length]" << endl;
    cerr << "Where <x> is one of:" << endl;
    cerr << "    -b:  Bubble sort" << endl;
    cerr << "    -s:  Selection sort" << endl;
    cerr << "    -i:  Insertion sort" << endl;
    cerr << "    -h:  Heapsort" << endl;
    cerr << "    -q:  Quicksort" << endl;
    cerr << "    -q3: Quicksort with median of three values" << endl;
    cerr << "    -m:  Mergesort" << endl;
    cerr << "And [length] is the length of the array that is to be sorted. This parameter is optional." << endl;
}


int main(int argc, char *argv[]) {
    if (argc != 2 and argc != 3) {
        print_usage();
        return 1;
    } else {
        size_t *length;
        if (argc == 2) {
            length = new size_t(0);
        } else {
            try {
                length = new size_t(stoi(argv[2]));
            } catch (const invalid_argument& e) {
                print_usage();
                return 3;
            }
        }
        auto array = new unsigned int *;
        if (std::strcmp(argv[1], "-b") == 0) {
            cout << "Bubble sort" << endl;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                bubblesort(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-s") == 0) {
            cout << "Selection sort" << endl;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                selectionsort(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-i") == 0) {
            cout << "Insertion sort" << endl;
            auto length = new size_t(0);
            auto array = new unsigned int *;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                insertionsort(*length, *array);
                cout << *length << endl;
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-h") == 0) {
            cout << "Heapsort" << endl;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                heapsort(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-q") == 0) {
            cout << "Quicksort" << endl;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                quicksort(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-q3") == 0) {
            cout << "Quicksort Median 3" << endl;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                quicksort_median3(*length, *array);
                checkSolution(*array);
            }
        } else if (std::strcmp(argv[1], "-m") == 0) {
            cout << "Mergesort" << endl;
            for (size_t i = 1; i <= num_examples; i++) {
                getExample(i, *length, *array);
                mergesort(*length, *array);
                checkSolution(*array);
            }
        } else {
            print_usage();
            delete length;
            delete array;
            return 2;
        }
        delete length;
        delete array;
    }

    return 0;
}
