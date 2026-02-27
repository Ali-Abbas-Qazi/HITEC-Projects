/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.assignmentsystemgui;

/**
 *
 * @author aliab
 */

// Import Swing components for GUI and AWT for layout and events
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;

// Main class for the Assignment System GUI
public class AssignmentSystemGUI {
    // Static field for assignment deadline
    static String deadline = "01-07-2025";  // Deadline date
    // Static field for total marks per assignment
    static int totalMarks = 15;               // Total marks available

    // 2D array holding questions for five assignments
    static String[][] allQuestions = {
        {  // Assignment 1 questions
            "1. What is the capital of France?",
            "2. Name the largest planet in our solar system.",
            "3. Who wrote 'Romeo and Juliet'?"
        },
        {  // Assignment 2 questions
            "1. What is the boiling point of water in Celsius?",
            "2. Who developed the theory of relativity?",
            "3. What is the chemical symbol for gold?"
        },
        {  // Assignment 3 questions
            "1. In which year did World War II end?",
            "2. Who was the first man to step on the Moon?",
            "3. What was the name of the ancient Egyptian writing system?"
        },
        {  // Assignment 4 questions
            "1. What is the value of pi up to two decimal places?",
            "2. Solve for x: 2x + 5 = 13.",
            "3. What is the derivative of x^2?"
        },
        {  // Assignment 5 questions
            "1. What does CPU stand for?",
            "2. Name one programming language that is statically typed.",
            "3. What is the purpose of an operating system?"
        }
    };

    // Main entry point; schedules GUI creation on the Event Dispatch Thread
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> showMainMenu());  // Initialize main menu
    }

    // Display the main menu frame with three buttons
    static void showMainMenu() {
        JFrame frame = new JFrame("Assignment System");   // Create window
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // Exit on close
        frame.setSize(300, 200);                             // Set window size
        frame.setLayout(new GridLayout(3, 1));               // Arrange components in grid

        JButton teacherBtn = new JButton("Teacher Mode");  // Button for teacher mode
        JButton studentBtn = new JButton("Student Mode");  // Button for student mode
        JButton exitBtn = new JButton("Exit");             // Button to exit

        teacherBtn.addActionListener(e -> openTeacherMode()); // Open teacher mode on click
        studentBtn.addActionListener(e -> openStudentMode()); // Open student mode on click
        exitBtn.addActionListener(e -> System.exit(0));       // Exit application on click

        frame.add(teacherBtn);  // Add teacher button to frame
        frame.add(studentBtn);  // Add student button to frame
        frame.add(exitBtn);     // Add exit button to frame

        frame.setVisible(true); // Show the window
    }

    // Handler for Teacher Mode
    static void openTeacherMode() {
        String input = JOptionPane.showInputDialog("Enter assignment number (1-5):"); // Prompt for assignment
        if (input == null) return;  // Cancel if no input

        try {
            int num = Integer.parseInt(input);      // Convert input to number
            if (num < 1 || num > 5) {               // Validate range
                JOptionPane.showMessageDialog(null, "Invalid number!"); // Show error
                return;
            }

            String fileName = "Assignment_0" + num + ".txt"; // Determine filename
            File file = new File(fileName);                   // Create file object
            if (!file.exists()) {                             // Check existence
                JOptionPane.showMessageDialog(null, "File not found!"); // Show error
                return;
            }

            BufferedReader br = new BufferedReader(new FileReader(file)); // Reader for file
            JTextArea area = new JTextArea(20, 40);                     // Text area to display
            area.read(br, null);                                        // Load content
            br.close();                                                  // Close reader

            JScrollPane scroll = new JScrollPane(area);                  // Make scrollable
            JOptionPane.showMessageDialog(null, scroll, "Assignment Content", JOptionPane.INFORMATION_MESSAGE); // Show content

            String grade = JOptionPane.showInputDialog("Enter grade (out of " + totalMarks + "):" ); // Prompt for grade
            JOptionPane.showMessageDialog(null, "Assignment graded: " + grade + "/" + totalMarks);      // Show confirmation

        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, "Error: " + e.getMessage()); // Show any error
        }
    }

    // Handler for Student Mode
    static void openStudentMode() {
        String input = JOptionPane.showInputDialog("Enter assignment number (1-5):"); // Prompt for assignment
        if (input == null) return; // Cancel if no input

        try {
            int num = Integer.parseInt(input);      // Convert input to number
            if (num < 1 || num > 5) {               // Validate range
                JOptionPane.showMessageDialog(null, "Invalid number!"); // Show error
                return;
            }

            String fileName = "Assignment_0" + num + ".txt"; // Determine filename
            StringBuilder content = new StringBuilder();       // Builder for submission content
            content.append("Assignment: ").append(fileName).append("\n");         // Append file info
            content.append("Deadline: ").append(deadline).append("\n");         // Append deadline
            content.append("Total Marks: ").append(totalMarks).append("\n\n"); // Append marks

            String[] questions = allQuestions[num - 1]; // Get questions array
            for (String q : questions) {               // Loop through each question
                String ans = JOptionPane.showInputDialog(q);    // Prompt for answer
                if (ans == null) return;                        // Cancel if no input
                content.append(q).append("\nAnswer: ").append(ans).append("\n\n"); // Append Q&A
            }

            String name = JOptionPane.showInputDialog("Enter your name:"); // Prompt for student name
            if (name == null) return;                                       // Cancel if no input
            content.append("Student Name: ").append(name).append("\n"); // Append name

            String reg = JOptionPane.showInputDialog("Enter your Reg. number (e.g., 24-CyS-050):"); // Prompt for registration
            if (reg == null) return;                                           // Cancel if no input
            content.append("Reg. Number: ").append(reg).append("\n");     // Append registration

            int confirm = JOptionPane.showConfirmDialog(null, "Submit assignment?", "Confirm", JOptionPane.YES_NO_OPTION); // Confirm submission
            if (confirm == JOptionPane.YES_OPTION) {                           // Check choice
                FileWriter fw = new FileWriter(fileName);                    // Writer to file
                fw.write(content.toString());                                // Write content
                fw.close();                                                  // Close writer
                JOptionPane.showMessageDialog(null, "Assignment submitted successfully!"); // Success message
            } else {
                JOptionPane.showMessageDialog(null, "Submission cancelled."); // Cancel message
            }

        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, "Error: " + e.getMessage()); // Show any error
        }
    }
}
