// Buggy Java Code: Simple Banking System (with bugs)
import java.util.Scanner;
import java.util.ArrayList;
import java.util.List;

public class BankSystem {
    public static void main(String[] args) {
        Bank bank = new Bank();
        Scanner sc = new Scanner(System.in);
        int choice = 0;

        while (choice != 4) {
            System.out.println("\n1. Create Account\n2. Deposit\n3. Withdraw\n4. Exit");
            System.out.print("Enter choice: ");
            choice = sc.nextInt();

            switch (choice) {
                case 1:
                    System.out.print("Enter account holder name: ");
                    sc.nextLine(); // Consume newline
                    String name = sc.nextLine();
                    bank.createAccount(name);
                    break;
                case 2:
                    System.out.print("Enter account number: ");
                    int accNum = sc.nextInt();
                    System.out.print("Enter amount to deposit: ");
                    double depAmt = sc.nextDouble();
                    bank.deposit(accNum, depAmt);
                    break;
                case 3:
                    System.out.print("Enter account number: ");
                    int accNumW = sc.nextInt();
                    System.out.print("Enter amount to withdraw: ");
                    double withAmt = sc.nextDouble();
                    bank.withdraw(accNumW, withAmt);
                    break;
                case 4:
                    System.out.println("Exiting...");
                    break;
                default:
                    System.out.println("Invalid option. Try again.");
            }
        }

        sc.close();
    }
}

class Bank {
    private List<Account> accounts;

    public Bank() {
        accounts = new ArrayList<>();
    }

    public void createAccount(String name) {
        Account acc = new Account(name);
        accounts.add(acc);
        System.out.println("Account created! Account number: " + acc.getAccountNumber());
    }

    public void deposit(int accNum, double amount) {
        Account acc = findAccount(accNum);
        if (acc != null) {
            if (acc.deposit(amount)) {
                System.out.println("Deposited " + amount + " to account " + accNum);
            } else {
                System.out.println("Deposit failed: Invalid amount for account " + accNum);
            }
        } else {
            System.out.println("Account not found.");
        }
    }

    public void withdraw(int accNum, double amount) {
        Account acc = findAccount(accNum);
        if (acc != null) {
            if (acc.withdraw(amount)) {
                System.out.println("Withdrawn " + amount + " from account " + accNum);
            } else {
                System.out.println("Withdrawal failed: Insufficient balance or invalid amount for account " + accNum);
            }
        } else {
            System.out.println("Account not found.");
        }
    }

    private Account findAccount(int accNum) {
        for (Account a : accounts) {
            if (a.getAccountNumber() == accNum) {
                return a;
            }
        }
        return null;
    }
}

class Account {
    private static int count = 1000;
    private int accountNumber;
    private String holderName;
    private double balance;

    public Account(String name) {
        holderName = name;
        accountNumber = count++;
        balance = 0.0;
    }

    public int getAccountNumber() {
        return accountNumber;
    }

    public boolean deposit(double amt) {
        if (amt < 0) {
            return false;
        }
        balance += amt;
        return true;
    }

    public boolean withdraw(double amt) {
        if (amt < 0 || amt > balance) {
            return false;
        }
        balance -= amt;
        return true;
    }
}