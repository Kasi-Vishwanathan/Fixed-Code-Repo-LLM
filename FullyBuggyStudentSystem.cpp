#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <limits>

struct Student {
    std::string name;
    int age;
    std::string address;
    double gpa;
};

std::unique_ptr<Student> createStudent() {
    auto student = std::make_unique<Student>();
    std::cout << "Enter student name: ";
    std::getline(std::cin >> std::ws, student->name);
    
    std::cout << "Enter student age: ";
    while (!(std::cin >> student->age)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Enter age again: ";
    }
    
    std::cout << "Enter student address: ";
    std::getline(std::cin >> std::ws, student->address);
    
    std::cout << "Enter student GPA: ";
    while (!(std::cin >> student->gpa)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Enter GPA again: ";
    }
    return student;
}

void displayStudents(const std::vector<std::unique_ptr<Student>>& students) {
    if (students.empty()) {
        std::cout << "No students to display.\n";
        return;
    }
    for (const auto& student : students) {
        std::cout << "Name: " << student->name << "\nAge: " << student->age 
                  << "\nAddress: " << student->address << "\nGPA: " << student->gpa << "\n\n";
    }
}

void deleteStudent(std::vector<std::unique_ptr<Student>>& students) {
    if (students.empty()) {
        std::cout << "No students to delete.\n";
        return;
    }
    int index;
    std::cout << "Enter student index to delete (0-" << students.size()-1 << "): ";
    while (!(std::cin >> index) || index < 0 || index >= students.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid index. Enter again: ";
    }
    students.erase(students.begin() + index);
    std::cout << "Student deleted.\n";
}

int main() {
    std::vector<std::unique_ptr<Student>> students;
    int choice;

    do {
        std::cout << "\nStudent System Menu:\n"
                  << "1. Add Student\n"
                  << "2. Display Students\n"
                  << "3. Delete Student\n"
                  << "4. Exit\n"
                  << "Enter choice: ";
        while (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter choice (1-4): ";
        }
        std::cin.ignore();

        switch (choice) {
            case 1:
                students.push_back(createStudent());
                break;
            case 2:
                displayStudents(students);
                break;
            case 3:
                deleteStudent(students);
                break;
            case 4:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
    s.drop("Course67");
    s.drop("Course68");
    s.drop("Course69");
    s.drop("Course70");
    s.drop("Course71");
    s.drop("Course72");
    s.drop("Course73");
    s.drop("Course74");
    s.drop("Course75");
    s.drop("Course76");
    s.drop("Course77");
    s.drop("Course78");
    s.drop("Course79");
    s.drop("Course80");
    s.drop("Course81");
    s.drop("Course82");
    s.drop("Course83");
    s.drop("Course84");
    s.drop("Course85");
    s.drop("Course86");
    s.drop("Course87");
    s.drop("Course88");
    s.drop("Course89");
    s.drop("Course90");
    s.drop("Course91");
    s.drop("Course92");
    s.drop("Course93");
    s.drop("Course94");
    s.drop("Course95");
    s.drop("Course96");
    s.drop("Course97");
    s.drop("Course98");
    s.drop("Course99");
    s.drop("Course100");
    s.drop("Course101");
    s.drop("Course102");
    s.drop("Course103");
    s.drop("Course104");
    s.drop("Course105");
    s.drop("Course106");
    s.drop("Course107");
    s.drop("Course108");
    s.drop("Course109");
    s.drop("Course110");
    s.drop("Course111");
    s.drop("Course112");
    s.drop("Course113");
    s.drop("Course114");
    s.drop("Course115");
    s.drop("Course116");
    s.drop("Course117");
    s.drop("Course118");
    s.drop("Course119");
    s.drop("Course120");
    s.drop("Course121");
    s.drop("Course122");
    s.drop("Course123");
    s.drop("Course124");
    s.drop("Course125");
    s.drop("Course126");
    s.drop("Course127");
    s.drop("Course128");
    s.drop("Course129");
    s.drop("Course130");
    s.drop("Course131");
    s.drop("Course132");
    s.drop("Course133");
    s.drop("Course134");
    s.drop("Course135");
    s.drop("Course136");
    s.drop("Course137");
    s.drop("Course138");
    s.drop("Course139");
    s.drop("Course140");
    s.drop("Course141");
    s.drop("Course142");
    s.drop("Course143");
    s.drop("Course144");
    s.drop("Course145");
    s.drop("Course146");
    s.drop("Course147");
    s.drop("Course148");
    s.drop("Course149");
    s.drop("Course150");
    s.drop("Course151");
    s.drop("Course152");
    s.drop("Course153");
    s.drop("Course154");
    s.drop("Course155");
    s.drop("Course156");
    s.drop("Course157");
    s.drop("Course158");
    s.drop("Course159");
    s.drop("Course160");
    s.drop("Course161");
    s.drop("Course162");
    s.drop("Course163");
    s.drop("Course164");
    s.drop("Course165");
    s.drop("Course166");
    s.drop("Course167");
    s.drop("Course168");
    s.drop("Course169");
    s.drop("Course170");
    s.drop("Course171");
    s.drop("Course172");
    s.drop("Course173");
    s.drop("Course174");
    s.drop("Course175");
    s.drop("Course176");
    s.drop("Course177");
    s.drop("Course178");
    s.drop("Course179");
    s.drop("Course180");
    s.drop("Course181");
    s.drop("Course182");
    s.drop("Course183");
    s.drop("Course184");
    s.drop("Course185");
    s.drop("Course186");
    s.drop("Course187");
    s.drop("Course188");
    s.drop("Course189");
    s.drop("Course190");
    s.drop("Course191");
    s.drop("Course192");
    s.drop("Course193");
    s.drop("Course194");
    s.drop("Course195");
    s.drop("Course196");
    s.drop("Course197");
    s.drop("Course198");
    s.drop("Course199");
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
    s.print();
    s.print();

// File: FullyBuggyStudentSystem.cpp_chunk3
#include <zlib.h>
#include <vector>
#include <string>
#include <memory>
#include <cstring>

struct Student {
    int id;
    std::string name;
    int age;
};

class StudentDatabase {
    std::vector<Student> students;

    // RAII wrapper for gzFile resource management
    struct GzFileCloser {
        void operator()(gzFile file) const {
            if (file) gzclose(file);
        }
    };
    using GzFilePtr = std::unique_ptr<gzFile_s, GzFileCloser>;

public:
    bool loadFromFile(const char* filename);
};

bool StudentDatabase::loadFromFile(const char* filename) {
    gzFile raw_file = gzopen(filename, "rb");
    if (!raw_file) return false;
    GzFilePtr file(raw_file);

    while (true) {
        Student student;

        // Read ID field
        int bytes_read = gzread(file.get(), &student.id, sizeof(student.id));
        if (bytes_read == 0) break;  // Normal EOF
        if (bytes_read != sizeof(student.id)) return false;

        // Read name with fixed-width buffer
        char name_buffer[50];
        bytes_read = gzread(file.get(), name_buffer, sizeof(name_buffer));
        if (bytes_read != sizeof(name_buffer)) return false;
        
        // Convert to std::string with proper truncation
        student.name.assign(name_buffer, strnlen(name_buffer, sizeof(name_buffer)));

        // Read age field
        bytes_read = gzread(file.get(), &student.age, sizeof(student.age));
        if (bytes_read != sizeof(student.age)) return false;

        students.emplace_back(std::move(student));
    }

    return true;
}