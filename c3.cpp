// 50‑line Buggy C++: Simple stack
#include <iostream>
#include <vector>

class Stack {
public:
    void push(int x) { data.push_back(x); }
    int pop() {                       // BUG: no empty‑check
        int v = data.back();
        data.pop_back();
        return v;
    }
    int top() { return data.back(); } // BUG: no empty‑check
    void print() {
        for (size_t i = 0; i <= data.size(); ++i) // BUG: off‑by‑one
            std::cout << data[i] << ' ';
        std::cout << '\n';
    }
private:
    std::vector<int> data;            // BUG: never caps capacity
};

int main() {
    Stack st;
    for (int i = 1; i <= 5; ++i) st.push(i);
    st.print();

    std::cout << "Popped: " << st.pop() << '\n';
    st.print();

    // BUG: pop more times than pushed
    for (int i = 0; i < 6; ++i)
        std::cout << "Popped: " << st.pop() << '\n';

    std::cout << "Top element: " << st.top() << '\n';
    return 0;
}
