/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.assignmentsystem;

/**
 *
 * @author aliab
 */
import java.io.*;  // for file handling
import java.util.*;  // for Scanner and collections

public class AssignmentSystem {
    static Scanner scanner = new Scanner(System.in);  // scanner for user input
    static String deadline = "01-07-2025";      // collective deadline
    static int totalMarks = 15;                   // total marks for assignment
    // placeholder questions for each assignment
    static String[] questions1 = {
        "1. What is the capital of France?",
        "2. Name the largest planet in our solar system.",
        "3. Who wrote 'Romeo and Juliet'?"
    };
    static String[] questions2 = {
        "1. What is the boiling point of water in Celsius?",
        "2. Who developed the theory of relativity?",
        "3. What is the chemical symbol for gold?"
    };
    static String[] questions3 = {
        "1. In which year did World War II end?",
        "2. Who was the first man to step on the Moon?",
        "3. What was the name of the ancient Egyptian writing system?"
    };
    static String[] questions4 = {
        "1. What is the value of pi up to two decimal places?",
        "2. Solve for x: 2x + 5 = 13.",
        "3. What is the derivative of x^2?"
    };
    static String[] questions5 = {
        "1. What does CPU stand for?",
        "2. Name one programming language that is statically typed.",
        "3. What is the purpose of an operating system?"
    };

    public static void main(String[] args) {
        while (true) {  // loop until exit
            System.out.println("\n==== MAIN MENU ====\n");  // show main menu with spacing
            System.out.println("1. Teacher Mode");
            System.out.println("2. Student Mode");
            System.out.println("3. Exit");
            System.out.print("Choose option: ");  // prompt choice
            int choice = scanner.nextInt();
            scanner.nextLine();  // consume newline

            switch (choice) {
                case 1:
                    teacherMode();  // teacher functionality
                    break;
                case 2:
                    studentMode();  // student functionality
                    break;
                case 3:
                    exitProgram();  // print credits and exit
                    return;
                default:
                    System.out.println("Invalid choice! Please try again.\n");
            }
        }
    }

    static void teacherMode() {
        System.out.println("--- TEACHER MODE ---\n");  // header with blank line
        System.out.println("Available Assignments: 1 to 5");  // simple list
        System.out.print("Choose assignment number (1-5) to open and grade: ");  // prompt number
        int num = scanner.nextInt();
        scanner.nextLine();  // consume newline
        if (num < 1 || num > 5) {
            System.out.println("Invalid assignment number!\n");
            return;
        }
        String fileName = "Assignment_0" + num + ".txt";  // build filename

        // read and display file content
        try {
            FileReader fr = new FileReader(fileName);  // open file reader
            BufferedReader br = new BufferedReader(fr);  // buffer reader
            String line;
            System.out.println("\n--- FILE CONTENT ---\n");  // header with blank line
            while ((line = br.readLine()) != null) {
                System.out.println(line);  // print each line
            }
            br.close();  // close reader

            // prompt for grade
            System.out.print("\nEnter grade for this assignment (out of " + totalMarks + "): ");
            String grade = scanner.nextLine();  // read grade
            System.out.println("\nAssignment " + fileName + " graded: " + grade + "/" + totalMarks + "\n");
        } catch (IOException e) {
            System.out.println("Error opening file: " + e.getMessage() + "\n");  // handle error
        }
    }

    static void studentMode() {
        System.out.println("--- STUDENT MODE ---\n");  // header with blank line
        System.out.print("Choose assignment number (1-5): ");  // prompt assignment
        int num = scanner.nextInt();
        scanner.nextLine();  // consume newline
        String fileName = "Assignment_0" + num + ".txt";  // build file name

        System.out.println("Deadline: " + deadline + ", Total Marks: " + totalMarks + "\n");  // show info with blank line

        // select questions based on assignment number
        String[] questions;
        switch (num) {
            case 1: questions = questions1; break;
            case 2: questions = questions2; break;
            case 3: questions = questions3; break;
            case 4: questions = questions4; break;
            case 5: questions = questions5; break;
            default:
                System.out.println("Invalid assignment number!\n");
                return;  // return to menu
        }

        boolean submitted = false;
        while (!submitted) {  // loop until confirmed
            try {
                FileWriter fw = new FileWriter(fileName);  // create/overwrite file
                // write header info
                fw.write("Assignment: " + fileName + "\n");
                fw.write("Deadline: " + deadline + "\n");
                fw.write("Total Marks: " + totalMarks + "\n\n");

                // write questions and capture answers
                for (String q : questions) {
                    System.out.println(q + "\n");  // display question with blank line
                    fw.write(q + "\n");  // write question
                    System.out.print("Your answer: ");
                    String ans = scanner.nextLine();  // read answer
                    fw.write("Answer: " + ans + "\n\n");  // write answer with gap
                }
                // ask for student details
                System.out.print("Enter your name: ");
                String name = scanner.nextLine();  // read name
                fw.write("Student Name: " + name + "\n\n");  // write name with gap

                System.out.print("Enter your Reg. number (format 24-CyS-050): ");
                String reg = scanner.nextLine();  // read reg
                fw.write("Reg. Number: " + reg + "\n");  // write reg

                fw.close();  // close writer

                // confirm submission
                System.out.print("\nSubmit assignment? (yes/no): ");
                String confirm = scanner.nextLine();  // read confirmation
                if (confirm.equalsIgnoreCase("yes")) {
                    System.out.println("\nAssignment submitted successfully!\n");
                    submitted = true;  // exit loop
                } else {
                    // delete file if not submitted and retry
                    File f = new File(fileName);
                    f.delete();  // delete file
                    System.out.println("Let's try again.\n");
                }
            } catch (IOException e) {
                System.out.println("Error writing file: " + e.getMessage() + "\n");  // handle error
                break;
            }
        }
    }

    static void exitProgram() {
        System.out.println("\n===== Program End =====");  // header with blank lines
        // System.out.println("           Made By:           ");  // centered-ish
        System.out.println();  // blank line
        System.out.println("Talal Atta");
        System.out.println("24-CyS-048");
        System.out.println();  // blank line between names
        System.out.println("Ali Abbas Qazi");
        System.out.println("24-CyS-050");
        System.out.println("\n======================");
    }
}
