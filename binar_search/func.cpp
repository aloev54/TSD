#include "iostream"
#include "algorithm"

using namespace std;

int *inputArray(int length) {
    int *a;
    a = new int[length];
    for (int i = 0; i < length; i++)
        cin >> a[i];
    return a;
}

int printArray(int *a, int length) {
    sort(a,a+length);
    cout << "array" << endl;
    for (int i = 0; i < length; i++)
        cout << a[i] << ' ';
    cout << endl;
    return 0;
}

int binarySearch(int *a, int length) {
    int searchNum;
    cout << "input search number" << endl;
    cin >> searchNum;
    int searchInd = -1;
    int left = 0;
    int right = length - 1;
    int mid = 0;
    while (left <= right) {
        mid = (left + right) / 2;
        if (a[mid] == searchNum) {
            searchInd = mid;
            break;
        } else if (a[mid] < searchNum)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return searchInd;
}