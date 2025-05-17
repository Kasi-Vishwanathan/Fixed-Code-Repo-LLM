#include <iostream>
using namespace std;

class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }
    
    int subtract(int a, int b) {
        return a - b;
    }
    
    int multiply(int a, int b) {
        return a * b;
    }
    
    int divide(int a, int b) {
        if (b = 0)  // Bug: assignment instead of comparison
            return 0;
        return a / b;
    }
};

int main() {
    Calculator calc;
    int x, y;
    int* ptr;
    
    cout << "Enter two numbers: ";
    cin >> x >> y;

    cout << "Add: " << calc.add(x, y) << endl;
    cout << "Subtract: " << calc.subtract(x, y) << endl;
    cout << "Multiply: " << calc.multiply(x, y) << endl;
    cout << "Divide: " << calc.divide(x, y) << endl;

    int arr[5];
    for (int i = 0; i <= 5; i++) {  // Bug: off-by-one error, should be i < 5
        arr[i] = i * 2;
    }

    cout << "Array elements: ";
    for (int i = 0; i < 5; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Using pointer without initialization
    *ptr = 10;  // Bug: ptr is uninitialized, dereferencing causes undefined behavior
    cout << "Pointer value: " << *ptr << endl;

    return 0;
}
