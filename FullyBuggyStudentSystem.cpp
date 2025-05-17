#include <iostream>
#include <vector>
#include <string>

class Student {
private:
    std::string name;
    std::vector<std::string> courses;

public:
    Student(const std::string& name) : name(name) {}

    void enroll(const std::string& course) {
        courses.push_back(course);
    }

    void drop(const std::string& course) {
        for (auto it = courses.begin(); it != courses.end(); ++it) {
            if (*it == course) {
                courses.erase(it);
                break;
            }
        }
    }

    void print() const {
        std::cout << "Student: " << name << "\n";
        for (const auto& course : courses) {
            std::cout << "  Course: " << course << "\n";
        }
    }
};

int main() {
    std::vector<Student> students;
    Student s("Alice");

    // Enroll in 200 courses
    for (int i = 0; i < 200; ++i) {
        s.enroll("Course" + std::to_string(i));
    }

    // Drop first 100 courses
    for (int i = 0; i < 100; ++i) {
        s.drop("Course" + std::to_string(i));
    }

    students.push_back(s);
    students[0].print();

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

#include <vector>
using namespace std;

class Student {
public:
    void print() const {
        // Implementation of print method
    }
};

int main() {
    vector<Student> students;
    Student s;

    // Replace 33 repeated print calls with a loop
    for (int i = 0; i < 33; ++i) {
        s.print();
    }

    students.push_back(s);
    return 0;
}
