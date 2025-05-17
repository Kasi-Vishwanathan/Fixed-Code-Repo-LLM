package com.example;

import java.util.ArrayList;
import java.util.List;

public class Buggy {
    public static void main(String[] args) {
        List<Integer> numbers = new ArrayList<>();
        numbers.add(1);
        numbers.add(2);
        numbers.add(3);

        int sum = 0;
        for (int i = 0; i < numbers.size(); i++) {
            int number = numbers.get(i);
            System.out.println("Number: " + number);
            sum += number;
        }

        double average = numbers.isEmpty() ? 0.0 : (double) sum / numbers.size();
        System.out.println("Average: " + average);
    }
}