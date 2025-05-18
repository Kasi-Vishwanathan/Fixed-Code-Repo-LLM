import java.util.Scanner;

public class qm {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        boolean quit = false;

        do {
            System.out.println("Menu:");
            System.out.println("1. Add");
            System.out.println("2. Subtract");
            System.out.println("3. Multiply");
            System.out.println("4. Divide");
            System.out.println("5. Exit");
            System.out.print("Enter your choice: ");

            int choice;
            try {
                choice = scanner.nextInt();
            } catch (Exception e) {
                System.out.println("Invalid input. Please enter a number.");
                scanner.nextLine();
                continue;
            }

            switch (choice) {
                case 1:
                case 2:
                case 3:
                case 4:
                    handleOperations(choice, scanner);
                    break;
                case 5:
                    quit = true;
                    break;
                default:
                    System.out.println("Invalid choice!");
            }

            scanner.nextLine();
        } while (!quit);

        scanner.close();
    }

    private static void handleOperations(int operation, Scanner scanner) {
        int num1 = 0, num2 = 0;

        System.out.print("Enter first number: ");
        try {
            num1 = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("Invalid number.");
            scanner.nextLine();
            return;
        }

        System.out.print("Enter second number: ");
        try {
            num2 = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("Invalid number.");
            scanner.nextLine();
            return;
        }

        int result = 0;
        switch (operation) {
            case 1:
                result = num1 + num2;
                break;
            case 2:
                result = num1 - num2;
                break;
            case 3:
                result = num1 * num2;
                break;
            case 4:
                if (num2 == 0) {
                    System.out.println("Error: Cannot divide by zero.");
                    return;
                }
                result = num1 / num2;
                break;
        }

        System.out.println("Result: " + result);

        for (int i = 1; i <= result; i++) {
            System.out.println("*".repeat(i));
        }
        for (int i = result; i >= 1; i--) {
            System.out.println("*".repeat(i));
        }
    }
}