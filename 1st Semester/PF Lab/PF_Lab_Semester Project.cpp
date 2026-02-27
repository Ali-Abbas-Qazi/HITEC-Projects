#include <iostream> // For input and output operations
#include <string>   // For handling strings
using namespace std; // To avoid using std:: prefix repeatedly

// Define a structure to represent a book in the library
struct Book {
    int id;                // Unique ID for the book
    string title;          // Title of the book
    string author;         // Author's name
    bool isAvailable;      // Availability status (true = available, false = not available)
};

// Function to add a new book to the library
void addBook(Book &book) {
    cout << "\n====================================\n";
    cout << "          Add New Book              \n";
    cout << "====================================\n";
    cout << "Enter book ID: ";
    cin >> book.id;
    cin.ignore(); // Ignore newline left in the input buffer

    cout << "Enter book title: ";
    getline(cin, book.title); // Input the full title of the book

    cout << "Enter author name: ";
    getline(cin, book.author); // Input the full name of the author

    book.isAvailable = true; // Default the book as available when added
    cout << "\n>>> Book added successfully! <<<\n\n"; // Confirmation message
}

// Function to display the details of a book
void displayBook(const Book &book) {
    cout << "\n------------------------------------\n";
    cout << "          Book Details             \n";
    cout << "------------------------------------\n";
    cout << "Book ID       : " << book.id << endl;
    cout << "Title         : " << book.title << endl;
    cout << "Author        : " << book.author << endl;
    cout << "Availability  : " << (book.isAvailable ? "Available" : "Not Available") << endl;
    cout << "------------------------------------\n";
}

// Function to search for a book by its title
void searchBookByTitle(Book books[], int size, const string &title) {
    cout << "\n====================================\n";
    cout << "       Search Book by Title        \n";
    cout << "====================================\n";
    bool found = false; // Flag to check if the book is found

    for (int i = 0; i < size; i++) {
        if (books[i].title == title) { // Compare titles
            displayBook(books[i]); // Display book details if found
            found = true;
            break; // Exit loop after finding the book
        }
    }

    if (!found) {
        cout << "\n>>> Book with title '" << title << "' not found. <<<\n\n"; // Message if not found
    }
}

// Function to update the availability status of a book
void updateAvailability(Book &book, bool status) {
    book.isAvailable = status; // Update the status
}

int main() {
    Book books[1000]; // Array to store up to 1000 books
    int bookCount = 0; // Counter for the number of books added
    int choice; // User's menu choice

    do {
        // Display the main menu
        cout << "\n====================================\n";
        cout << "       Library Management System    \n";
        cout << "====================================\n";
        cout << "1. Add a new book                   \n";
        cout << "2. Search for a book by title       \n";
        cout << "3. Display all books                \n";
        cout << "4. Update availability status       \n";
        cout << "5. Exit                             \n";
        cout << "====================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (bookCount < 1000) { // Ensure the library is not full
                    addBook(books[bookCount]); // Add a new book
                    bookCount++; // Increment the book counter
                } else {
                    cout << "\n>>> Library is full! <<<\n"; // Message if library is full
                }
                break;
            }
            case 2: {
                cin.ignore(); // Ignore leftover newline character
                string searchTitle;
                cout << "\nEnter the title of the book to search: ";
                getline(cin, searchTitle); // Input the title to search for
                searchBookByTitle(books, bookCount, searchTitle); // Search and display result
                break;
            }
            case 3: {
                if (bookCount == 0) { // Check if there are no books
                    cout << "\n>>> No books in the library. <<<\n";
                } else {
                    cout << "\n====================================\n";
                    cout << "           All Books                \n";
                    cout << "====================================\n";
                    for (int i = 0; i < bookCount; i++) {
                        displayBook(books[i]); // Display all books
                    }
                }
                break;
            }
            case 4: {
                int id;
                bool status;
                cout << "\nEnter the book ID to update status: ";
                cin >> id; // Input book ID
                cout << "Enter new status (1 for Available, 0 for Not Available): ";
                cin >> status; // Input new status

                bool found = false; // Flag to check if the book is found
                for (int i = 0; i < bookCount; i++) {
                    if (books[i].id == id) { // Compare book IDs
                        updateAvailability(books[i], status); // Update the status
                        cout << "\n>>> Availability updated successfully. <<<\n";
                        found = true;
                        break; // Exit loop after updating
                    }
                }
                if (!found) {
                    cout << "\n>>> Book with ID " << id << " not found. <<<\n"; // Message if not found
                }
                break;
            }
            case 5:
                cout << "\n>>> Program has been terminated. <<<\n"; // Exit message
                break;
            default:
                cout << "\n>>> Invalid choice. Try again. <<<\n"; // Message for invalid input
        }
    } while (choice != 5); // Loop until user chooses to exit

    // Display credits
    cout << "\n====================================\n";
    cout << "         Project Made By            \n";
    cout << "====================================\n";
    cout << "Ali Abbas Qazi\t24-CyS-050.\n";
    cout << "Musaib Khan\t24-CyS-036.\n";
    cout << "====================================\n";

    return 0;
}
