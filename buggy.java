public class StudentGradingSystem {

    public static void main(String[] args) {
        Student student = new Student("Alice", 102);
        student.addGrade(95);
        student.addGrade(87);
        student.addGrade(93); // Fixed incorrect data type
        student.printAverage();

        Student secondStudent = new Student("Bob", 103); // Added missing ID
        secondStudent.addGrade(65);
        secondStudent.addGrade(75);
        secondStudent.printAverage();

        // Removed code causing NullPointerException
    }
}

class Student {
    private String name;
    private int id;
    private java.util.List<Integer> grades; // Using List for dynamic sizing

    public Student(String name, int id) {
        this.name = name;
        this.id = id;
        this.grades = new java.util.ArrayList<>();
    }

    public void addGrade(int grade) {
        grades.add(grade);
    }

    public void printAverage() {
        if (grades.isEmpty()) {
            System.out.println("No grades available for " + name);
            return;
        }
        int sum = 0;
        for (int grade : grades) {
            sum += grade;
        }
        double average = (double) sum / grades.size();
        System.out.printf("Average for %s: %.2f%n", name, average);
    }
}