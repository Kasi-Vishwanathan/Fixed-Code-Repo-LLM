#include <iostream>
using namespace std;

int main() {
    int i = 0;
    while (i < 10) {
        cout << i << " ";
        i++; // Added increment to prevent infinite loop
    }
    return 0;
}