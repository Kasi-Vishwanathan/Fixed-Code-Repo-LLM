// 50‑line Buggy C++: Simple stack
#include <iostream>
#include <vector>
#include <stdexcept>

class Stack {
public:
    void push(int x) { data.push_back(x); }

    int pop() {
        if (data.empty()) {
            throw std::out_of_range("Stack underflow");
        }
        int v = data.back();
        data.pop_back();
        return v;
    }

    int top() {
        if (data.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data.back();
    }

    void print() {
        for (size_t i = 0; i < data.size(); ++i)
            std::cout << data[i] << ' ';
        std::cout << '\n';
    }

private:
    std::vector<int> data;
};

int main() {
    Stack st;
    for (int i = 1; i <= 5; ++i) st.push(i);
    st.print();

    std::cout << "Popped: " << st.pop() << '\n';
    st.print();

    // Pop remaining 4 elements
    for (int i = 0; i < 4; ++i)
        std::cout << "Popped: " << st.pop() << '\n';

    try {
        std::cout << "Top element: " << st.top() << '\n';
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << '\n';
    }

    return 0;
}
