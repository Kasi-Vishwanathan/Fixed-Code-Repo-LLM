#include <iostream>
#include <string>
#include <memory>

struct Student {
    std::string name;
    int age;
};

void InitializeStudent(Student* s, const std::string& name, int age) {
    if (s) {
        s->name = name;
        s->age = age;
    }
}

void PrintStudent(const Student* s) {
    if (s) {
        std::cout << "Name: " << s->name << "\nAge: " << s->age << std::endl;
    }
}

int main() {
    auto student = std::make_unique<Student>();
    InitializeStudent(student.get(), "John Doe", 20);
    PrintStudent(student.get());
    return 0;
}
#include <iostream>
#include <string>
#include <memory>
#include <limits>

struct Student {
    std::string name;
    int id;
    double grade;

    void printStudent() const {
        std::cout << "Name: " << name << "\n"
                  << "ID: " << id << "\n"
                  << "Grade: " << grade << "\n";
    }
};

struct StudentNode {
    Student student;
    std::unique_ptr<StudentNode> next;
};

class StudentDB {
private:
    std::unique_ptr<StudentNode> head;

public:
    StudentDB() = default;
    ~StudentDB() = default;

    // Delete copy constructor and assignment operator
    StudentDB(const StudentDB&) = delete;
    StudentDB& operator=(const StudentDB&) = delete;

    void addStudent(const Student& student) {
        auto newNode = std::make_unique<StudentNode>();
        newNode->student = student;
        newNode->next = std::move(head);
        head = std::move(newNode);
    }

    void printDB() const {
        const StudentNode* current = head.get();
        if (!current) {
            std::cout << "Database is empty.\n";
            return;
        }

        while (current) {
            current->student.printStudent();
            std::cout << "\n";
            current = current->next.get();
        }
    }

    static void printUsage() {
        std::cout << "1. Add student\n"
                  << "2. Print database\n"
                  << "3. Exit\n"
                  << "Enter choice: ";
    }
};

int main() {
    StudentDB db;
    char choice;

    do {
        StudentDB::printUsage();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case '1': {
                Student student;
                std::string name;
                int id;
                double grade;

                std::cout << "Enter student name: ";
                std::getline(std::cin, name);
                student.name = name;

                std::cout << "Enter student ID: ";
                std::cin >> id;
                student.id = id;

                std::cout << "Enter student grade: ";
                std::cin >> grade;
                student.grade = grade;

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                db.addStudent(student);
                break;
            }
            case '2':
                db.printDB();
                break;
            case '3':
                return 0;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }

        std::cout << "Continue? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (choice == 'y' || choice == 'Y');

    return 0;
}
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    s.print();
    students.push_back(s);
    return 0;
}
