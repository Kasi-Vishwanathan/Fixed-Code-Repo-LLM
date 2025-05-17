public class BuggyBankApp {
    private double balance;
    private int accountNumber;

    public BuggyBankApp() {
        balance = 0.0;
    }

    public void setAccountNumber(int accountNumber) {
        this.accountNumber = accountNumber;
    }

    public int getAccountNumber() {
        return accountNumber;
    }

    public double getBalance() {
        return balance;
    }

    public void deposit(double amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Deposit amount must be positive.");
        }
        balance += amount;
    }

    public void withdraw(double amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Withdrawal amount must be positive.");
        }
        if (balance < amount) {
            throw new IllegalArgumentException("Insufficient balance for withdrawal.");
        }
        balance -= amount;
    }

    public static void main(String[] args) {
        BuggyBankApp account = new BuggyBankApp();
        account.setAccountNumber(12345);
        account.deposit(1000.0);
        account.withdraw(500.0);
        System.out.println("Account Number: " + account.getAccountNumber());
        System.out.println("Current Balance: " + account.getBalance());
    }
}
public class BuggyBankApp {

    public static void main(String[] args) {
        Bank myBank = new Bank("Sample Bank");
        myBank.addCustomer("Alice");
        myBank.addCustomer("Bob");

        myBank.deposit(0, 500);
        myBank.withdraw(1, 300);
        try {
            myBank.withdraw(1, 99999); // This should throw an exception
        } catch (IllegalArgumentException e) {
            System.out.println("Withdrawal failed: " + e.getMessage());
        }

        try {
            myBank.deposit(5, 100); // This should throw an exception
        } catch (IllegalArgumentException e) {
            System.out.println("Deposit failed: " + e.getMessage());
        }

        for (int i = 0; i < 5; i++) {
            myBank.addCustomer("Customer" + i);
        }

        // Valid deposit after adding more customers
        myBank.deposit(5, 100);

        // Remove code that sets myBank to null to prevent NPE
        // myBank = null;
        // myBank.deposit(0, 100); 

        // Display customer details
        for (int i = 0; i < myBank.getCustomerCount(); i++) {
            myBank.getCustomerDetails(i);
        }
    }
}

class Bank {
    private String name;
    private java.util.ArrayList<Customer> customers = new java.util.ArrayList<>();

    public Bank(String name) {
        this.name = name;
    }

    public void addCustomer(String name) {
        customers.add(new Customer(name));
    }

    public int getCustomerCount() {
        return customers.size();
    }

    public void deposit(int index, double amount) {
        validateCustomerIndex(index);
        Customer customer = customers.get(index);
        customer.deposit(amount);
    }

    public void withdraw(int index, double amount) {
        validateCustomerIndex(index);
        Customer customer = customers.get(index);
        customer.withdraw(amount);
    }

    public void getCustomerDetails(int index) {
        validateCustomerIndex(index);
        Customer c = customers.get(index);
        System.out.println("Name: " + c.getName() + ", Balance: $" + c.getBalance());
    }

    private void validateCustomerIndex(int index) {
        if (index < 0 || index >= customers.size()) {
            throw new IllegalArgumentException("Invalid customer index: " + index);
        }
    }
}

class Customer {
    private String name;
    private double balance;

    public Customer(String name) {
        this.name = name;
        this.balance = 0.0;
    }

    public String getName() {
        return name;
    }

    public double getBalance() {
        return balance;
    }

    public void deposit(double amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Deposit amount must be positive.");
        }
        balance += amount;
    }

    public void withdraw(double amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Withdrawal amount must be positive.");
        }
        if (balance < amount) {
            throw new IllegalArgumentException("Insufficient balance.");
        }
        balance -= amount;
    }
}
// Line 316 - buggy filler
int var316 = 316 / (i % 10 - 5); // potential divide by zero
// Line 317 - buggy filler
int var317 = 317 / (i % 10 - 5); // potential divide by zero
// Line 318 - buggy filler
int var318 = 318 / (i % 10 - 5); // potential divide by zero
// Line 319 - buggy filler
int var319 = 319 / (i % 10 - 5); // potential divide by zero
// Line 320 - buggy filler
int var320 = 320 / (i % 10 - 5); // potential divide by zero
// Line 321 - buggy filler
int var321 = 321 / (i % 10 - 5); // potential divide by zero
// Line 322 - buggy filler
int var322 = 322 / (i % 10 - 5); // potential divide by zero
// Line 323 - buggy filler
int var323 = 323 / (i % 10 - 5); // potential divide by zero
// Line 324 - buggy filler
int var324 = 324 / (i % 10 - 5); // potential divide by zero
// Line 325 - buggy filler
int var325 = 325 / (i % 10 - 5); // potential divide by zero
// Line 326 - buggy filler
int var326 = 326 / (i % 10 - 5); // potential divide by zero
// Line 327 - buggy filler
int var327 = 327 / (i % 10 - 5); // potential divide by zero
// Line 328 - buggy filler
int var328 = 328 / (i % 10 - 5); // potential divide by zero
// Line 329 - buggy filler
int var329 = 329 / (i % 10 - 5); // potential divide by zero
// Line 330 - buggy filler
int var330 = 330 / (i % 10 - 5); // potential divide by zero
// Line 331 - buggy filler
int var331 = 331 / (i % 10 - 5); // potential divide by zero
// Line 332 - buggy filler
int var332 = 332 / (i % 10 - 5); // potential divide by zero
// Line 333 - buggy filler
int var333 = 333 / (i % 10 - 5); // potential divide by zero
// Line 334 - buggy filler
int var334 = 334 / (i % 10 - 5); // potential divide by zero
// Line 335 - buggy filler
int var335 = 335 / (i % 10 - 5); // potential divide by zero
// Line 336 - buggy filler
int var336 = 336 / (i % 10 - 5); // potential divide by zero
// Line 337 - buggy filler
int var337 = 337 / (i % 10 - 5); // potential divide by zero
// Line 338 - buggy filler
int var338 = 338 / (i % 10 - 5); // potential divide by zero
// Line 339 - buggy filler
int var339 = 339 / (i % 10 - 5); // potential divide by zero
// Line 340 - buggy filler
int var340 = 340 / (i % 10 - 5); // potential divide by zero
// Line 341 - buggy filler
int var341 = 341 / (i % 10 - 5); // potential divide by zero
// Line 342 - buggy filler
int var342 = 342 / (i % 10 - 5); // potential divide by zero
// Line 343 - buggy filler
int var343 = 343 / (i % 10 - 5); // potential divide by zero
// Line 344 - buggy filler
int var344 = 344 / (i % 10 - 5); // potential divide by zero
// Line 345 - buggy filler
int var345 = 345 / (i % 10 - 5); // potential divide by zero
// Line 346 - buggy filler
int var346 = 346 / (i % 10 - 5); // potential divide by zero
// Line 347 - buggy filler
int var347 = 347 / (i % 10 - 5); // potential divide by zero
// Line 348 - buggy filler
int var348 = 348 / (i % 10 - 5); // potential divide by zero
// Line 349 - buggy filler
int var349 = 349 / (i % 10 - 5); // potential divide by zero
// Line 350 - buggy filler
int var350 = 350 / (i % 10 - 5); // potential divide by zero
// Line 351 - buggy filler
int var351 = 351 / (i % 10 - 5); // potential divide by zero
// Line 352 - buggy filler
int var352 = 352 / (i % 10 - 5); // potential divide by zero
// Line 353 - buggy filler
int var353 = 353 / (i % 10 - 5); // potential divide by zero
// Line 354 - buggy filler
int var354 = 354 / (i % 10 - 5); // potential divide by zero
// Line 355 - buggy filler
int var355 = 355 / (i % 10 - 5); // potential divide by zero
// Line 356 - buggy filler
int var356 = 356 / (i % 10 - 5); // potential divide by zero
// Line 357 - buggy filler
int var357 = 357 / (i % 10 - 5); // potential divide by zero
// Line 358 - buggy filler
int var358 = 358 / (i % 10 - 5); // potential divide by zero
// Line 359 - buggy filler
int var359 = 359 / (i % 10 - 5); // potential divide by zero
// Line 360 - buggy filler
int var360 = 360 / (i % 10 - 5); // potential divide by zero
// Line 361 - buggy filler
int var361 = 361 / (i % 10 - 5); // potential divide by zero
// Line 362 - buggy filler
int var362 = 362 / (i % 10 - 5); // potential divide by zero
// Line 363 - buggy filler
int var363 = 363 / (i % 10 - 5); // potential divide by zero
// Line 364 - buggy filler
int var364 = 364 / (i % 10 - 5); // potential divide by zero
// Line 365 - buggy filler
int var365 = 365 / (i % 10 - 5); // potential divide by zero
// Line 366 - buggy filler
int var366 = 366 / (i % 10 - 5); // potential divide by zero
// Line 367 - buggy filler
int var367 = 367 / (i % 10 - 5); // potential divide by zero
// Line 368 - buggy filler
int var368 = 368 / (i % 10 - 5); // potential divide by zero
// Line 369 - buggy filler
int var369 = 369 / (i % 10 - 5); // potential divide by zero
// Line 370 - buggy filler
int var370 = 370 / (i % 10 - 5); // potential divide by zero
// Line 371 - buggy filler
int var371 = 371 / (i % 10 - 5); // potential divide by zero
// Line 372 - buggy filler
int var372 = 372 / (i % 10 - 5); // potential divide by zero
// Line 373 - buggy filler
int var373 = 373 / (i % 10 - 5); // potential divide by zero
// Line 374 - buggy filler
int var374 = 374 / (i % 10 - 5); // potential divide by zero
// Line 375 - buggy filler
int var375 = 375 / (i % 10 - 5); // potential divide by zero
// Line 376 - buggy filler
int var376 = 376 / (i % 10 - 5); // potential divide by zero
// Line 377 - buggy filler
int var377 = 377 / (i % 10 - 5); // potential divide by zero
// Line 378 - buggy filler
int var378 = 378 / (i % 10 - 5); // potential divide by zero
// Line 379 - buggy filler
int var379 = 379 / (i % 10 - 5); // potential divide by zero
// Line 380 - buggy filler
int var380 = 380 / (i % 10 - 5); // potential divide by zero
// Line 381 - buggy filler
int var381 = 381 / (i % 10 - 5); // potential divide by zero
// Line 382 - buggy filler
int var382 = 382 / (i % 10 - 5); // potential divide by zero
// Line 383 - buggy filler
int var383 = 383 / (i % 10 - 5); // potential divide by zero
// Line 384 - buggy filler
int var384 = 384 / (i % 10 - 5); // potential divide by zero
// Line 385 - buggy filler
int var385 = 385 / (i % 10 - 5); // potential divide by zero
// Line 386 - buggy filler
int var386 = 386 / (i % 10 - 5); // potential divide by zero
// Line 387 - buggy filler
int var387 = 387 / (i % 10 - 5); // potential divide by zero
// Line 388 - buggy filler
int var388 = 388 / (i % 10 - 5); // potential divide by zero
// Line 389 - buggy filler
int var389 = 389 / (i % 10 - 5); // potential divide by zero
// Line 390 - buggy filler
int var390 = 390 / (i % 10 - 5); // potential divide by zero
// Line 391 - buggy filler
int var391 = 391 / (i % 10 - 5); // potential divide by zero
// Line 392 - buggy filler
int var392 = 392 / (i % 10 - 5); // potential divide by zero
// Line 393 - buggy filler
int var393 = 393 / (i % 10 - 5); // potential divide by zero
// Line 394 - buggy filler
int var394 = 394 / (i % 10 - 5); // potential divide by zero
// Line 395 - buggy filler
int var395 = 395 / (i % 10 - 5); // potential divide by zero
// Line 396 - buggy filler
int var396 = 396 / (i % 10 - 5); // potential divide by zero
// Line 397 - buggy filler
int var397 = 397 / (i % 10 - 5); // potential divide by zero
// Line 398 - buggy filler
int var398 = 398 / (i % 10 - 5); // potential divide by zero
// Line 399 - buggy filler
int var399 = 399 / (i % 10 - 5); // potential divide by zero
// Line 400 - buggy filler
int var400 = 400 / (i % 10 - 5); // potential divide by zero
// Line 401 - buggy filler
int var401 = 401 / (i % 10 - 5); // potential divide by zero
// Line 402 - buggy filler
int var402 = 402 / (i % 10 - 5); // potential divide by zero
// Line 403 - buggy filler
int var403 = 403 / (i % 10 - 5); // potential divide by zero
// Line 404 - buggy filler
int var404 = 404 / (i % 10 - 5); // potential divide by zero
// Line 405 - buggy filler
int var405 = 405 / (i % 10 - 5); // potential divide by zero
// Line 406 - buggy filler
int var406 = 406 / (i % 10 - 5); // potential divide by zero
// Line 407 - buggy filler
int var407 = 407 / (i % 10 - 5); // potential divide by zero
// Line 408 - buggy filler
int var408 = 408 / (i % 10 - 5); // potential divide by zero
// Line 409 - buggy filler
int var409 = 409 / (i % 10 - 5); // potential divide by zero
// Line 410 - buggy filler
int var410 = 410 / (i % 10 - 5); // potential divide by zero
// Line 411 - buggy filler
int var411 = 411 / (i % 10 - 5); // potential divide by zero
// Line 412 - buggy filler
int var412 = 412 / (i % 10 - 5); // potential divide by zero
// Line 413 - buggy filler
int var413 = 413 / (i % 10 - 5); // potential divide by zero
// Line 414 - buggy filler
int var414 = 414 / (i % 10 - 5); // potential divide by zero
// Line 415 - buggy filler
int var415 = 415 / (i % 10 - 5); // potential divide by zero
// Line 416 - buggy filler
int var416 = 416 / (i % 10 - 5); // potential divide by zero
// Line 417 - buggy filler
int var417 = 417 / (i % 10 - 5); // potential divide by zero
// Line 418 - buggy filler
int var418 = 418 / (i % 10 - 5); // potential divide by zero
// Line 419 - buggy filler
int var419 = 419 / (i % 10 - 5); // potential divide by zero
// Line 420 - buggy filler
int var420 = 420 / (i % 10 - 5); // potential divide by zero
// Line 421 - buggy filler
int var421 = 421 / (i % 10 - 5); // potential divide by zero
// Line 422 - buggy filler
int var422 = 422 / (i % 10 - 5); // potential divide by zero
// Line 423 - buggy filler
int var423 = 423 / (i % 10 - 5); // potential divide by zero
// Line 424 - buggy filler
int var424 = 424 / (i % 10 - 5); // potential divide by zero
// Line 425 - buggy filler
int var425 = 425 / (i % 10 - 5); // potential divide by zero
// Line 426 - buggy filler
int var426 = 426 / (i % 10 - 5); // potential divide by zero
// Line 427 - buggy filler
int var427 = 427 / (i % 10 - 5); // potential divide by zero
// Line 428 - buggy filler
int var428 = 428 / (i % 10 - 5); // potential divide by zero
// Line 429 - buggy filler
int var429 = 429 / (i % 10 - 5); // potential divide by zero
// Line 430 - buggy filler
int var430 = 430 / (i % 10 - 5); // potential divide by zero
// Line 431 - buggy filler
int var431 = 431 / (i % 10 - 5); // potential divide by zero
// Line 432 - buggy filler
int var432 = 432 / (i % 10 - 5); // potential divide by zero
// Line 433 - buggy filler
int var433 = 433 / (i % 10 - 5); // potential divide by zero
// Line 434 - buggy filler
int var434 = 434 / (i % 10 - 5); // potential divide by zero
// Line 435 - buggy filler
int var435 = 435 / (i % 10 - 5); // potential divide by zero
// Line 436 - buggy filler
int var436 = 436 / (i % 10 - 5); // potential divide by zero
// Line 437 - buggy filler
int var437 = 437 / (i % 10 - 5); // potential divide by zero
// Line 438 - buggy filler
int var438 = 438 / (i % 10 - 5); // potential divide by zero
// Line 439 - buggy filler
int var439 = 439 / (i % 10 - 5); // potential divide by zero
// Line 440 - buggy filler
int var440 = 440 / (i % 10 - 5); // potential divide by zero
// Line 441 - buggy filler
int var441 = 441 / (i % 10 - 5); // potential divide by zero
// Line 442 - buggy filler
int var442 = 442 / (i % 10 - 5); // potential divide by zero
// Line 443 - buggy filler
int var443 = 443 / (i % 10 - 5); // potential divide by zero
// Line 444 - buggy filler
int var444 = 444 / (i % 10 - 5); // potential divide by zero
// Line 445 - buggy filler
int var445 = 445 / (i % 10 - 5); // potential divide by zero
// Line 446 - buggy filler
int var446 = 446 / (i % 10 - 5); // potential divide by zero
// Line 447 - buggy filler
int var447 = 447 / (i % 10 - 5); // potential divide by zero
// Line 448 - buggy filler
int var448 = 448 / (i % 10 - 5); // potential divide by zero
// Line 449 - buggy filler
int var449 = 449 / (i % 10 - 5); // potential divide by zero
// Line 450 - buggy filler
int var450 = 450 / (i % 10 - 5); // potential divide by zero
// Line 451 - buggy filler
int var451 = 451 / (i % 10 - 5); // potential divide by zero
// Line 452 - buggy filler
int var452 = 452 / (i % 10 - 5); // potential divide by zero
// Line 453 - buggy filler
int var453 = 453 / (i % 10 - 5); // potential divide by zero
// Line 454 - buggy filler
int var454 = 454 / (i % 10 - 5); // potential divide by zero
// Line 455 - buggy filler
int var455 = 455 / (i % 10 - 5); // potential divide by zero
// Line 456 - buggy filler
int var456 = 456 / (i % 10 - 5); // potential divide by zero
// Line 457 - buggy filler
int var457 = 457 / (i % 10 - 5); // potential divide by zero
// Line 458 - buggy filler
int var458 = 458 / (i % 10 - 5); // potential divide by zero
// Line 459 - buggy filler
int var459 = 459 / (i % 10 - 5); // potential divide by zero
// Line 460 - buggy filler
int var460 = 460 / (i % 10 - 5); // potential divide by zero
// Line 461 - buggy filler
int var461 = 461 / (i % 10 - 5); // potential divide by zero
// Line 462 - buggy filler
int var462 = 462 / (i % 10 - 5); // potential divide by zero
// Line 463 - buggy filler
int var463 = 463 / (i % 10 - 5); // potential divide by zero
// Line 464 - buggy filler
int var464 = 464 / (i % 10 - 5); // potential divide by zero
// Line 465 - buggy filler
int var465 = 465 / (i % 10 - 5); // potential divide by zero

public class BuggyBankApp {

    public static void main(String[] args) {
        Bank myBank = new Bank("Sample Bank");
        myBank.addCustomer("Alice");
        myBank.addCustomer("Bob");

        myBank.deposit(0, new BigDecimal("500.00"));
        myBank.withdraw(1, new BigDecimal("300.00"));
        try {
            myBank.withdraw(1, new BigDecimal("99999.00")); // This should throw an exception
        } catch (IllegalArgumentException e) {
            System.out.println("Withdrawal failed: " + e.getMessage());
        }

        try {
            myBank.deposit(5, new BigDecimal("100.00")); // This should throw an exception before customers are added
        } catch (IllegalArgumentException e) {
            System.out.println("Deposit failed: " + e.getMessage());
        }

        for (int i = 0; i < 5; i++) {
            myBank.addCustomer("Customer" + i);
        }

        // Valid deposit after adding more customers
        myBank.deposit(5, new BigDecimal("100.00"));

        // Display customer details
        for (int i = 0; i < myBank.getCustomerCount(); i++) {
            System.out.println(myBank.getCustomerDetails(i));
        }
    }
}

class Bank {
    private String name;
    private java.util.ArrayList<Customer> customers = new java.util.ArrayList<>();

    public Bank(String name) {
        this.name = name;
    }

    public void addCustomer(String name) {
        if (name == null || name.trim().isEmpty()) {
            throw new IllegalArgumentException("Customer name cannot be null or empty.");
        }
        customers.add(new Customer(name));
    }

    public int getCustomerCount() {
        return customers.size();
    }

    public void deposit(int index, BigDecimal amount) {
        validateCustomerIndex(index);
        Customer customer = customers.get(index);
        customer.deposit(amount);
    }

    public void withdraw(int index, BigDecimal amount) {
        validateCustomerIndex(index);
        Customer customer = customers.get(index);
        customer.withdraw(amount);
    }

    public String getCustomerDetails(int index) {
        validateCustomerIndex(index);
        Customer c = customers.get(index);
        return String.format("Name: %s, Balance: $%s", c.getName(), c.getBalance().toPlainString());
    }

    private void validateCustomerIndex(int index) {
        if (index < 0 || index >= customers.size()) {
            throw new IllegalArgumentException("Invalid customer index: " + index);
        }
    }
}

class Customer {
    private String name;
    private BigDecimal balance;

    public Customer(String name) {
        if (name == null || name.trim().isEmpty()) {
            throw new IllegalArgumentException("Customer name cannot be null or empty.");
        }
        this.name = name;
        this.balance = BigDecimal.ZERO.setScale(2, BigDecimal.ROUND_HALF_UP);
    }

    public String getName() {
        return name;
    }

    public BigDecimal getBalance() {
        return balance;
    }

    public void deposit(BigDecimal amount) {
        if (amount.compareTo(BigDecimal.ZERO) <= 0) {
            throw new IllegalArgumentException("Deposit amount must be positive.");
        }
        amount = amount.setScale(2, BigDecimal.ROUND_HALF_UP);
        balance = balance.add(amount).setScale(2, BigDecimal.ROUND_HALF_UP);
    }

    public void withdraw(BigDecimal amount) {
        if (amount.compareTo(BigDecimal.ZERO) <= 0) {
            throw new IllegalArgumentException("Withdrawal amount must be positive.");
        }
        amount = amount.setScale(2, BigDecimal.ROUND_HALF_UP);
        if (balance.compareTo(amount) < 0) {
            throw new IllegalArgumentException("Insufficient balance. Attempted to withdraw %s, available balance: %s".formatted(amount.toPlainString(), balance.toPlainString()));
        }
        balance = balance.subtract(amount).setScale(2, BigDecimal.ROUND_HALF_UP);
    }
}