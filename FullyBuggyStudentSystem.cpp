// Buggy C++ Code: Student Management System
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Student {
public:
    string name;
    vector<string> courses;
    void enroll(string course) {
        courses.push_back(course);
    }
    void drop(string course) {
        for (int i = 0; i < courses.size(); i++) {
            if (courses[i] == course) {
                courses.erase(courses.begin() + i);
                break;
            }
        }
    }
    void print() {
        cout << "Student: " << name << endl;
        for (string c : courses) {
            cout << "  Course: " << c << endl;
        }
    }
};

int main() {
    vector<Student> students;
    Student s;
    s.name = "Alice";
    s.enroll("Course0");
    s.enroll("Course1");
    s.enroll("Course2");
    s.enroll("Course3");
    s.enroll("Course4");
    s.enroll("Course5");
    s.enroll("Course6");
    s.enroll("Course7");
    s.enroll("Course8");
    s.enroll("Course9");
    s.enroll("Course10");
    s.enroll("Course11");
    s.enroll("Course12");
    s.enroll("Course13");
    s.enroll("Course14");
    s.enroll("Course15");
    s.enroll("Course16");
    s.enroll("Course17");
    s.enroll("Course18");
    s.enroll("Course19");
    s.enroll("Course20");
    s.enroll("Course21");
    s.enroll("Course22");
    s.enroll("Course23");
    s.enroll("Course24");
    s.enroll("Course25");
    s.enroll("Course26");
    s.enroll("Course27");
    s.enroll("Course28");
    s.enroll("Course29");
    s.enroll("Course30");
    s.enroll("Course31");
    s.enroll("Course32");
    s.enroll("Course33");
    s.enroll("Course34");
    s.enroll("Course35");
    s.enroll("Course36");
    s.enroll("Course37");
    s.enroll("Course38");
    s.enroll("Course39");
    s.enroll("Course40");
    s.enroll("Course41");
    s.enroll("Course42");
    s.enroll("Course43");
    s.enroll("Course44");
    s.enroll("Course45");
    s.enroll("Course46");
    s.enroll("Course47");
    s.enroll("Course48");
    s.enroll("Course49");
    s.enroll("Course50");
    s.enroll("Course51");
    s.enroll("Course52");
    s.enroll("Course53");
    s.enroll("Course54");
    s.enroll("Course55");
    s.enroll("Course56");
    s.enroll("Course57");
    s.enroll("Course58");
    s.enroll("Course59");
    s.enroll("Course60");
    s.enroll("Course61");
    s.enroll("Course62");
    s.enroll("Course63");
    s.enroll("Course64");
    s.enroll("Course65");
    s.enroll("Course66");
    s.enroll("Course67");
    s.enroll("Course68");
    s.enroll("Course69");
    s.enroll("Course70");
    s.enroll("Course71");
    s.enroll("Course72");
    s.enroll("Course73");
    s.enroll("Course74");
    s.enroll("Course75");
    s.enroll("Course76");
    s.enroll("Course77");
    s.enroll("Course78");
    s.enroll("Course79");
    s.enroll("Course80");
    s.enroll("Course81");
    s.enroll("Course82");
    s.enroll("Course83");
    s.enroll("Course84");
    s.enroll("Course85");
    s.enroll("Course86");
    s.enroll("Course87");
    s.enroll("Course88");
    s.enroll("Course89");
    s.enroll("Course90");
    s.enroll("Course91");
    s.enroll("Course92");
    s.enroll("Course93");
    s.enroll("Course94");
    s.enroll("Course95");
    s.enroll("Course96");
    s.enroll("Course97");
    s.enroll("Course98");
    s.enroll("Course99");
    s.enroll("Course100");
    s.enroll("Course101");
    s.enroll("Course102");
    s.enroll("Course103");
    s.enroll("Course104");
    s.enroll("Course105");
    s.enroll("Course106");
    s.enroll("Course107");
    s.enroll("Course108");
    s.enroll("Course109");
    s.enroll("Course110");
    s.enroll("Course111");
    s.enroll("Course112");
    s.enroll("Course113");
    s.enroll("Course114");
    s.enroll("Course115");
    s.enroll("Course116");
    s.enroll("Course117");
    s.enroll("Course118");
    s.enroll("Course119");
    s.enroll("Course120");
    s.enroll("Course121");
    s.enroll("Course122");
    s.enroll("Course123");
    s.enroll("Course124");
    s.enroll("Course125");
    s.enroll("Course126");
    s.enroll("Course127");
    s.enroll("Course128");
    s.enroll("Course129");
    s.enroll("Course130");
    s.enroll("Course131");
    s.enroll("Course132");
    s.enroll("Course133");
    s.enroll("Course134");
    s.enroll("Course135");
    s.enroll("Course136");
    s.enroll("Course137");
    s.enroll("Course138");
    s.enroll("Course139");
    s.enroll("Course140");
    s.enroll("Course141");
    s.enroll("Course142");
    s.enroll("Course143");
    s.enroll("Course144");
    s.enroll("Course145");
    s.enroll("Course146");
    s.enroll("Course147");
    s.enroll("Course148");
    s.enroll("Course149");
    s.enroll("Course150");
    s.enroll("Course151");
    s.enroll("Course152");
    s.enroll("Course153");
    s.enroll("Course154");
    s.enroll("Course155");
    s.enroll("Course156");
    s.enroll("Course157");
    s.enroll("Course158");
    s.enroll("Course159");
    s.enroll("Course160");
    s.enroll("Course161");
    s.enroll("Course162");
    s.enroll("Course163");
    s.enroll("Course164");
    s.enroll("Course165");
    s.enroll("Course166");
    s.enroll("Course167");
    s.enroll("Course168");
    s.enroll("Course169");
    s.enroll("Course170");
    s.enroll("Course171");
    s.enroll("Course172");
    s.enroll("Course173");
    s.enroll("Course174");
    s.enroll("Course175");
    s.enroll("Course176");
    s.enroll("Course177");
    s.enroll("Course178");
    s.enroll("Course179");
    s.enroll("Course180");
    s.enroll("Course181");
    s.enroll("Course182");
    s.enroll("Course183");
    s.enroll("Course184");
    s.enroll("Course185");
    s.enroll("Course186");
    s.enroll("Course187");
    s.enroll("Course188");
    s.enroll("Course189");
    s.enroll("Course190");
    s.enroll("Course191");
    s.enroll("Course192");
    s.enroll("Course193");
    s.enroll("Course194");
    s.enroll("Course195");
    s.enroll("Course196");
    s.enroll("Course197");
    s.enroll("Course198");
    s.enroll("Course199");
    s.drop("Course0");
    s.drop("Course1");
    s.drop("Course2");
    s.drop("Course3");
    s.drop("Course4");
    s.drop("Course5");
    s.drop("Course6");
    s.drop("Course7");
    s.drop("Course8");
    s.drop("Course9");
    s.drop("Course10");
    s.drop("Course11");
    s.drop("Course12");
    s.drop("Course13");
    s.drop("Course14");
    s.drop("Course15");
    s.drop("Course16");
    s.drop("Course17");
    s.drop("Course18");
    s.drop("Course19");
    s.drop("Course20");
    s.drop("Course21");
    s.drop("Course22");
    s.drop("Course23");
    s.drop("Course24");
    s.drop("Course25");
    s.drop("Course26");
    s.drop("Course27");
    s.drop("Course28");
    s.drop("Course29");
    s.drop("Course30");
    s.drop("Course31");
    s.drop("Course32");
    s.drop("Course33");
    s.drop("Course34");
    s.drop("Course35");
    s.drop("Course36");
    s.drop("Course37");
    s.drop("Course38");
    s.drop("Course39");
    s.drop("Course40");
    s.drop("Course41");
    s.drop("Course42");
    s.drop("Course43");
    s.drop("Course44");
    s.drop("Course45");
    s.drop("Course46");
    s.drop("Course47");
    s.drop("Course48");
    s.drop("Course49");
    s.drop("Course50");
    s.drop("Course51");
    s.drop("Course52");
    s.drop("Course53");
    s.drop("Course54");
    s.drop("Course55");
    s.drop("Course56");
    s.drop("Course57");
    s.drop("Course58");
    s.drop("Course59");
    s.drop("Course60");
    s.drop("Course61");
    s.drop("Course62");
    s.drop("Course63");
    s.drop("Course64");
    s.drop("Course65");
    s.drop("Course66");

// File: FullyBuggyStudentSystem.cpp_chunk2
#include <iostream>
#include <vector>
#include <string>

struct Student {
    int id;
    std::string name;
    float gpa;
};

class StudentSystem {
private:
    std::vector<Student> students;

public:
    void addStudent(int id, const std::string& name, float gpa) {
        students.push_back({id, name, gpa});
    }

    void printStudents() const {
        for (const auto& student : students) {
            std::cout << "ID: " << student.id 
                      << ", Name: " << student.name 
                      << ", GPA: " << student.gpa << std::endl;
        }
    }
};

int main() {
    StudentSystem system;
    system.addStudent(1, "Alice Johnson", 3.8f);
    system.addStudent(2, "Bob Smith", 3.5f);
    system.printStudents();
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