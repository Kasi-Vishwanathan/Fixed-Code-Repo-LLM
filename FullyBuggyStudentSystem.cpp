#include <iostream>
#include <string>
#include <unordered_set>

class Student {
private:
    std::unordered_set<std::string> courses;

public:
    void enroll(const std::string& course) {
        courses.insert(course);
    }

    void drop(const std::string& course) {
        courses.erase(course);
    }

    void print() const {
        for (const auto& course : courses) {
            std::cout << course << '\n';
        }
        std::cout << std::endl;
    }
};

int main() {
    Student s;
    
    // Enroll in courses (example setup; adjust as needed)
    for (int i = 1; i <= 200; ++i) {
        s.enroll("Course" + std::to_string(i));
    }
    
    // Drop courses 67 to 199
    for (int i = 67; i <= 199; ++i) {
        s.drop("Course" + std::to_string(i));
    }
    
    // Call print 179 times (replaces redundant calls)
    for (int i = 0; i < 179; ++i) {
        s.print();
    }

    return 0;
}
