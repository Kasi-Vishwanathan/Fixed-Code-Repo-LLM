#include <iostream>
using namespace std;

int main() {
    int value = 42;
    int *ptr = &value; // Assign valid address
    cout << "Dereferencing ptr: " << *ptr << endl;
    return 0;
}