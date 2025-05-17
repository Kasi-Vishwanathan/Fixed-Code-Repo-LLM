// 50-line Buggy C++: Simple stack
#include <iostream>
#include <vector>
#include <stdexcept>

class Stack {
public:
    void push(int x) { data.push_back(x); }
    
    int pop() {
        if (data.empty()) {
            throw std::out_of_range("Stack underflow in pop()");
        }
        int v = data.back();
        data.pop_back();
        return v;
    }
    
    int top() const {
        if (data.empty()) {
            throw std::out_of_range("Stack is empty in top()");
        }
        return data.back();
    }
    
    void print() const {
        for (size_t i = 0; i < data.size(); ++i)
            std::cout << data[i] << ' ';
        std::cout << '\n';
    }
    
    bool empty() const { return data.empty(); }

private:
    std::vector<int> data;
};

int main() {
    Stack st;
    for (int i = 1; i <= 5; ++i) 
        st.push(i);
    st.print();

    if (!st.empty()) {
        std::cout << "Popped: " << st.pop() << '\n';
        st.print();
    }

    for (int i = 0; i < 6; ++i) {
        if (st.empty()) {
            std::cout << "Cannot pop, stack is empty.\n";
            break;
        }
        std::cout << "Popped: " << st.pop() << '\n';
    }

    if (!st.empty()) {
        std::cout << "Top element: " << st.top() << '\n';
    } else {
        std::cout << "Stack is empty, no top element.\n";
    }

    return 0;
}
