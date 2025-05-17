// c3.cpp

#include <iostream>
#include <vector>
#include <stdexcept>

class Stack {
public:
    static const size_t MAX_CAPACITY = 100;

    void push(int x) {
        if (data.size() >= MAX_CAPACITY) {
            throw std::overflow_error("Stack overflow");
        }
        data.push_back(x);
    }

    int pop() {
        if (data.empty()) {
            throw std::underflow_error("Stack underflow on pop");
        }
        int v = data.back();
        data.pop_back();
        return v;
    }

    int top() {
        if (data.empty()) {
            throw std::underflow_error("Stack underflow on top");
        }
        return data.back();
    }

    void print() {
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i] << ' ';
        }
        std::cout << '\n';
    }

    bool empty() const {
        return data.empty();
    }

private:
    std::vector<int> data;
};

int main() {
    Stack st;
    try {
        for (int i = 1; i <= 5; ++i) {
            st.push(i);
        }
        st.print();

        if (!st.empty()) {
            std::cout << "Popped: " << st.pop() << '\n';
            st.print();
        }

        // Attempt to pop 6 times (after 5 elements initially, then 4 left)
        for (int i = 0; i < 6; ++i) {
            if (st.empty()) {
                std::cerr << "Cannot pop: Stack is empty\n";
                break;
            }
            std::cout << "Popped: " << st.pop() << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    try {
        if (!st.empty()) {
            std::cout << "Top element: " << st.top() << '\n';
        } else {
            std::cout << "Stack is empty, no top element.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
