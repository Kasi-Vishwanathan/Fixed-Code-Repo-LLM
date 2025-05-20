#include <iostream>
#include <string>
#include <limits>

struct Student {
    int id;
    std::string name;
    std::string major;
    int age;
    Student* next;
};

class StudentList {
private:
    Student* head;
    Student* tail;

public:
    StudentList() : head(nullptr), tail(nullptr) {}

    ~StudentList() {
        Student* current = head;
        while (current != nullptr) {
            Student* next = current->next;
            delete current;
            current = next;
        }
    }

    void addStudent(Student* newStudent) {
        newStudent->next = nullptr;
        if (head == nullptr) {
            head = tail = newStudent;
        } else {
            tail->next = newStudent;
            tail = newStudent;
        }
    }

    void deleteStudent(int id) {
        Student* current = head;
        Student* prev = nullptr;

        while (current != nullptr && current->id != id) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            std::cout << "Student with ID " << id << " not found!\n";
            return;
        }

        if (prev == nullptr) {
            head = current->next;
        } else {
            prev->next = current->next;
        }

        if (current == tail) {
            tail = prev;
        }

        delete current;
    }

    void listStudents() const {
        if (head == nullptr) {
            std::cout << "No students in the system.\n";
            return;
        }

        Student* current = head;
        while (current != nullptr) {
            std::cout << "Student ID: " << current->id
                      << ", Name: " << current->name
                      << ", Major: " << current->major
                      << ", Age: " << current->age << '\n';
            current = current->next;
        }
    }
};

int main() {
    StudentList students;

    // Pre-populate with sample data
    for (int i = 0; i < 5; ++i) {
        Student* newStudent = new Student;
        newStudent->id = i;
        newStudent->name = "John Doe";
        newStudent->major = "Computer Science";
        newStudent->age = 20;
        students.addStudent(newStudent);
    }

    int choice;
    do {
        std::cout << "\n1. Add student\n2. Delete student\n3. List students\n4. Exit\nEnter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                Student* newStudent = new Student;
                newStudent->next = nullptr;

                std::cout << "Enter student ID: ";
                std::cin >> newStudent->id;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Enter student name: ";
                std::getline(std::cin, newStudent->name);

                std::cout << "Enter student major: ";
                std::getline(std::cin, newStudent->major);

                std::cout << "Enter student age: ";
                std::cin >> newStudent->age;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                students.addStudent(newStudent);
                break;
            }
            case 2: {
                int id;
                std::cout << "Enter student ID to delete: ";
                std::cin >> id;
                students.deleteStudent(id);
                break;
            }
            case 3:
                students.listStudents();
                break;
            case 4:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
// File: FullyBuggyStudentSystem.cpp_chunk2

#include <iostream>
#include <string>
#include <memory>
#include <limits>

struct Student {
    std::string name;
    std::string department;
    std::unique_ptr<Student> next;
};

std::unique_ptr<Student> head;

void addStudent(const std::string& name, const std::string& department) {
    auto new_student = std::make_unique<Student>();
    new_student->name = name;
    new_student->department = department;
    new_student->next = nullptr;

    if (!head) {
        head = std::move(new_student);
    } else {
        Student* current = head.get();
        while (current->next) {
            current = current->next.get();
        }
        current->next = std::move(new_student);
    }
}

void displayStudents() {
    Student* current = head.get();
    if (!current) {
        std::cout << "No students to display.\n";
        return;
    }
    while (current) {
        std::cout << "Name: " << current->name << "\n";
        std::cout << "Department: " << current->department << "\n\n";
        current = current->next.get();
    }
}

int main() {
    int choice;
    std::string input;

    do {
        std::cout << "1. Add Student\n2. Display Students\n3. Exit\n";
        std::cout << "Enter choice: ";
        std::getline(std::cin, input);

        try {
            choice = std::stoi(input);
        } catch (...) {
            choice = 0;
        }

        switch (choice) {
            case 1: {
                std::string name, dept;
                std::cout << "Enter student name: ";
                std::getline(std::cin, name);
                std::cout << "Enter department: ";
                std::getline(std::cin, dept);
                addStudent(name, dept);
                break;
            }
            case 2:
                displayStudents();
                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            case 3:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}
// File: FullyBuggyStudentSystem.cpp_chunk3
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

class Student {
private:
    std::string name;
    int age;

public:
    Student(std::string n, int a) : name(std::move(n)), age(a) {}

    void display() const {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }
};

double computeAverage(const std::vector<int>& grades) {
    if (grades.empty()) {
        throw std::invalid_argument("Cannot compute average of empty grades list");
    }
    
    int sum = 0;
    for (int grade : grades) {
        sum += grade;
    }
    return static_cast<double>(sum) / grades.size();
}

int main() {
    // Stack-allocated student
    Student s1("Alice", 20);
    s1.display();

    // Heap-allocated student using smart pointer
    auto studentPtr = std::make_unique<Student>("Bob", 22);
    studentPtr->display();

    // Grade calculation
    std::vector<int> grades = {60, 70, 80};
    try {
        double average = computeAverage(grades);
        std::cout << "Average grade: " << average << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}