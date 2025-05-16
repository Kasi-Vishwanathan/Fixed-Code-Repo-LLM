// File: FullyBuggyStudentSystem.cpp_part2
#include <iostream>
#include <string>
#include <memory>
#include <vector>

class Student {
private:
    std::string name;
    int age;
public:
    // Constructor using modern initializer list
    Student(const std::string& name, int age) : name(name), age(age) {}

    // Automatic Rule of Five compliance via std::string and defaulted operations
    Student(const Student&) = default;
    Student& operator=(const Student&) = default;
    Student(Student&&) = default;
    Student& operator=(Student&&) = default;
    ~Student() = default;

    // Use const member function for correctness
    void displayInfo() const {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }
};

class StudentDatabase {
private:
    std::vector<std::unique_ptr<Student>> students;

public:
    void addStudent(const std::string& name, int age) {
        students.push_back(std::make_unique<Student>(name, age));
    }

    void displayAllStudents() const {
        for (const auto& student : students) {
            student->displayInfo();
        }
    }
};

int main() {
    StudentDatabase db;
    db.addStudent("Alice Johnson", 20);
    db.addStudent("Bob Smith", 22);
    db.displayAllStudents();
    return 0;
}