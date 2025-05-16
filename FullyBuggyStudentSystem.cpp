// File: FullyBuggyStudentSystem.cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Student {
private:
    std::string name;
    std::vector<std::string> courses;

public:
    explicit Student(const std::string& studentName) : name(studentName) {}

    void enroll(const std::string& course) {
        courses.push_back(course);
    }

    void drop(const std::string& course) {
        auto it = std::find(courses.begin(), courses.end(), course);
        if (it != courses.end()) {
            courses.erase(it);
        }
    }

    void print() const {
        std::cout << "Student: " << name << "\n";
        for (const auto& course : courses) {
            std::cout << "  Course: " << course << "\n";
        }
        if (courses.empty()) {
            std::cout << "  No enrolled courses.\n";
        }
    }
};

int main() {
    std::vector<Student> students;
    Student s("Alice");

    // Enroll in 200 courses using a loop
    for (int i = 0; i < 200; ++i) {
        s.enroll("Course" + std::to_string(i));
    }

    // Demonstrate current state after enrollment
    std::cout << "After enrollment:\n";
    s.print();

    // Drop all 200 courses
    for (int i = 0; i < 200; ++i) {
        s.drop("Course" + std::to_string(i));
    }

    // Demonstrate final state after drops
    std::cout << "\nAfter dropping all courses:\n";
    s.print();

    students.push_back(s);
    return 0;
}