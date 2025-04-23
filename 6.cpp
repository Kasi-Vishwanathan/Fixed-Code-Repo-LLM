#include <iostream>
using namespace std;

int main() {
    int n = 1000000;
    int *arr = new int[n]; // Reduced allocation size to prevent memory issues
    for (int i = 0; i < n; ++i) {
        arr[i] = i;
    }
    cout << "Array filled!" << endl;
    delete[] arr;
    return 0;
}