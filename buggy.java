public class StudentGradingSystem {

    public static void main(String[] args) {
        Student student = new Student("Alice", 102);
        student.addGrade(95);
        student.addGrade(87);
        student.addGrade(85); // Fixed: Replaced "A" with 85
        student.printAverage();

        Student secondStudent = new Student("Bob", 101); // Fixed: Added missing id argument
        secondStudent.addGrade(65);
        secondStudent.addGrade(75);
        secondStudent.printAverage();

        // Removed the code causing NullPointerException
        // student = null;
        // student.printAverage();
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
        if (gradeCount >= grades.length) { // Fixed: Changed to >= to prevent out-of-bounds
            System.out.println("Cannot add more grades");
            return;
        }
        grades[gradeCount] = grade;
        gradeCount++;
    }

    public void printAverage() {
        if (gradeCount == 0) {
            System.out.println("Average for " + name + ": No grades available");
            return;
        }
        int sum = 0;
        for (int i = 0; i < gradeCount; i++) { // Fixed: Changed loop condition to <
            sum += grades[i];
        }
        double average = (double) sum / gradeCount; // Fixed: Cast to double for correct division
        System.out.printf("Average for %s: %.2f%n", name, average); // Formatted output to two decimal places
    }
}