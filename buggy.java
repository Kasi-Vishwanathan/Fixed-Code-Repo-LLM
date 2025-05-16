public class StudentGradingSystem {

    public static void main(String[] args) {
        Student student = new Student("Alice", 102);
        student.addGrade(95);
        student.addGrade(87);
        student.addGrade("A"); // Bug: wrong datatype
        student.printAverage();

        Student secondStudent = new Student("Bob"); // Bug: missing id argument
        secondStudent.addGrade(65);
        secondStudent.addGrade(75);
        secondStudent.printAverage();

        student = null;
        student.printAverage(); // Bug: NullPointerException
    }
}

class Student {
    private String name;
    private int id;
    private int[] grades;
    private int gradeCount;

    public Student(String name, int id) {
        this.name = name;
        this.id = id;
        this.grades = new int[5];
        gradeCount = 0;
    }

    public void addGrade(int grade) {
        if (gradeCount > grades.length) { // Bug: should be >= not >
            System.out.println("Cannot add more grades");
            return;
        }
        grades[gradeCount] = grade;
        gradeCount++;
    }

    public void printAverage() {
        int sum = 0;
        for (int i = 0; i <= gradeCount; i++) { // Bug: should be i < gradeCount
            sum += grades[i];
        }
        double average = sum / gradeCount; // Bug: possible divide by zero
        System.out.println("Average for " + name + ": " + average);
    }

    public void Student(String name, int id) { // Bug: constructor wrongly defined as a method
        this.name = name;
        this.id = id;
    }
}
