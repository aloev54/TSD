#include <iostream>

#include "func.h"

using namespace std;


int main() {
    int *array;
    int length;
    int searchIndex;
    cout << "input length" << endl;
    cin >> length;
    cout << "input array" << endl;
    array = inputArray(length);
    printArray(array, length);
    searchIndex = binarySearch(array, length);
    cout << "search index" << " " << searchIndex << endl;
    return 0;
}
