import java.util.Scanner;

public class BuggyBankApp {
    public static void main(String[] args) {
        try (Scanner input = new Scanner(System.in)) {
            double balance = getValidDouble(input, "Enter initial balance: ");
            double interestRate = getValidDouble(input, "Enter annual interest rate (%): ");
            double years = getValidDouble(input, "Enter number of years: ");

            double newBalance = calculateCompoundInterest(balance, interestRate, years);
            System.out.printf("Your new balance after %.2f years will be: %.2f%n", years, newBalance);
        }
    }

    private static double getValidDouble(Scanner input, String prompt) {
        while (true) {
            System.out.print(prompt);
            if (input.hasNextDouble()) {
                return input.nextDouble();
            } else {
                System.out.println("Invalid input. Please enter a number.");
                input.next(); // Discard invalid input
            }
        }
    }

    private static double calculateCompoundInterest(double principal, double rate, double time) {
        return principal * Math.pow(1 + (rate / 100), time);
    }
}
import java.util.Scanner;

public class BuggyBankApp {
    public static void main(String[] args) {
        double balance = 1000.0;
        boolean running = true;
        Scanner input = new Scanner(System.in);
        
        do {
            System.out.println("\n1. Deposit");
            System.out.println("2. Withdraw");
            System.out.println("3. Check Balance");
            System.out.println("4. Exit");
            System.out.print("Enter your choice: ");
            
            String choiceStr = input.nextLine();
            try {
                int choice = Integer.parseInt(choiceStr);
            
                switch (choice) {
                    case 1: {
                        System.out.print("Enter amount to deposit: ");
                        String depositStr = input.nextLine();
                        try {
                            int depositAmount = Integer.parseInt(depositStr);
                            if (depositAmount <= 0) {
                                System.out.println("Invalid amount. Please enter a positive value.");
                            } else {
                                balance += depositAmount;
                                System.out.println("Deposit successful. Current balance: $" + String.format("%.2f", balance));
                            }
                        } catch (NumberFormatException e) {
                            System.out.println("Invalid input. Please enter a valid number.");
                        }
                        break;
                    }
                    case 2: {
                        System.out.print("Enter amount to withdraw: ");
                        String withdrawStr = input.nextLine();
                        try {
                            int withdrawAmount = Integer.parseInt(withdrawStr);
                            if (withdrawAmount <= 0) {
                                System.out.println("Invalid amount. Please enter a positive value.");
                            } else if (withdrawAmount > balance) {
                                System.out.println("Insufficient funds!");
                            } else {
                                balance -= withdrawAmount;
                                System.out.println("Withdrawal successful. Current balance: $" + String.format("%.2f", balance));
                            }
                        } catch (NumberFormatException e) {
                            System.out.println("Invalid input. Please enter a valid number.");
                        }
                        break;
                    }
                    case 3:
                        System.out.printf("Your balance: $%.2f%n", balance);
                        break;
                    case 4:
                        running = false;
                        System.out.println("Thank you for using BuggyBankApp!");
                        break;
                    default:
                        System.out.println("Invalid choice! Please select 1-4.");
                }
            } catch (NumberFormatException e) {
                System.out.println("Invalid choice. Please enter a valid number (1-4).");
            }
        } while (running);
        input.close();
    }
}
import java.util.ArrayList;
import java.util.List;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.List;
import java.util.Scanner;

class Account {
    private String accountNumber;
    private double balance;
    private String accountHolder;
    private List<Transaction> transactions;

    public Account(String accountNumber, double balance, String accountHolder) {
        if (balance < 0) {
            throw new IllegalArgumentException("Initial balance cannot be negative.");
        }
        this.accountNumber = accountNumber;
        this.balance = balance;
        this.accountHolder = accountHolder;
        this.transactions = new ArrayList<>();
    }

    public void deposit(double amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Deposit amount must be positive.");
        }
        balance += amount;
        transactions.add(new Transaction(TransactionType.DEPOSIT, amount, LocalDateTime.now()));
    }

    public void withdraw(double amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Withdrawal amount must be positive.");
        }
        if (balance < amount) {
            throw new IllegalArgumentException("Insufficient balance for withdrawal.");
        }
        balance -= amount;
        transactions.add(new Transaction(TransactionType.WITHDRAWAL, amount, LocalDateTime.now()));
    }

    public double getBalance() {
        return balance;
    }

    public List<Transaction> getTransactions() {
        return new ArrayList<>(transactions);
    }
}

enum TransactionType {
    DEPOSIT, WITHDRAWAL
}

class Transaction {
    private TransactionType type;
    private double amount;
    private LocalDateTime timestamp;

    public Transaction(TransactionType type, double amount, LocalDateTime timestamp) {
        this.type = type;
        this.amount = amount;
        this.timestamp = timestamp;
    }

    @Override
    public String toString() {
        return String.format("%s: %.2f at %s", type, amount, timestamp);
    }
}

public class BuggyBankApp {
    public static void main(String[] args) {
        Account account = new Account("12345", 100.0, "John Doe");
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("\n1. Deposit");
            System.out.println("2. Withdraw");
            System.out.println("3. Exit");
            System.out.print("Choose an option: ");

            int choice = 0;
            try {
                choice = scanner.nextInt();
            } catch (InputMismatchException e) {
                System.out.println("Invalid input. Please enter a number.");
                scanner.nextLine();
                continue;
            }

            if (choice == 3) {
                System.out.println("Exiting...");
                break;
            }

            try {
                switch (choice) {
                    case 1:
                        handleDeposit(account, scanner);
                        break;
                    case 2:
                        handleWithdraw(account, scanner);
                        break;
                    default:
                        System.out.println("Invalid option. Try again.");
                }
            } catch (IllegalArgumentException e) {
                System.out.println("Error: " + e.getMessage());
            }
        }
        scanner.close();
    }

    private static void handleDeposit(Account account, Scanner scanner) {
        double amount = getValidAmount(scanner, "deposit");
        account.deposit(amount);
        System.out.printf("Deposit successful. New balance: %.2f%n", account.getBalance());
    }

    private static void handleWithdraw(Account account, Scanner scanner) {
        double amount = getValidAmount(scanner, "withdraw");
        account.withdraw(amount);
        System.out.printf("Withdrawal successful. New balance: %.2f%n", account.getBalance());
    }

    private static double getValidAmount(Scanner scanner, String operation) {
        while (true) {
            try {
                System.out.printf("Enter amount to %s: ", operation);
                return scanner.nextDouble();
            } catch (InputMismatchException e) {
                System.out.println("Invalid input. Please enter a numeric value.");
                scanner.nextLine();
            }
        }
    }
}