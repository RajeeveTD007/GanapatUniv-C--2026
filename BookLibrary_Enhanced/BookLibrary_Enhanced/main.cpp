
#include "Library.h"

#include <iostream>
#include <limits>
#include <string>

// UI helper: recover from non-numeric console input.
static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Display file operation result.
static void showFileResult(FileOperationResult result,
                           const std::string& action) {
    switch (result) {
    case FileOperationResult::Success:
        std::cout << action << " completed successfully.\n";
        break;

    case FileOperationResult::FileNotFound:
        std::cout << "Data file not found. "
                  << "Current library was not changed.\n";
        break;

    case FileOperationResult::InvalidFormat:
        std::cout << "Invalid file format. "
                  << "Current library was not changed.\n";
        break;

    case FileOperationResult::IoError:
        std::cout << "File operation failed.\n";
        break;
    }
}

// Display a summary of the project enhancements.
static void showEnhancementDemo() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "       BOOK LIBRARY ENHANCEMENTS\n";
    std::cout << "========================================\n";

    std::cout << "\n1. Duplicate Book ID Validation\n";
    std::cout << "   Prevents adding books with duplicate IDs.\n";

    std::cout << "\n2. Reusable Search Function\n";
    std::cout << "   findBook() reduces repeated search logic.\n";

    std::cout << "\n3. Separate Storage Module\n";
    std::cout << "   BookStorage handles file saving and loading.\n";

    std::cout << "\n4. Improved Input Validation\n";
    std::cout << "   Handles invalid IDs and empty book titles.\n";

    std::cout << "\n5. Improved Testing\n";
    std::cout << "   Tests duplicate records, invalid operations,\n";
    std::cout << "   and persistence round trips.\n";

    std::cout << "\n6. Better Code Organization\n";
    std::cout << "   UI, library logic, and storage are separated.\n";

    std::cout << "\n========================================\n";
    std::cout << "       END OF ENHANCEMENT DEMO\n";
    std::cout << "========================================\n";
}

int main() {
    Library library;

    const std::string dataFile = "books.txt";

    // Load saved books when the program starts.
    const auto startup = library.loadFromFile(dataFile);

    if (startup == FileOperationResult::Success) {
        std::cout << "Saved books loaded: "
                  << library.size() << '\n';
    }
    else if (startup != FileOperationResult::FileNotFound) {
        showFileResult(startup, "Startup load");
    }

    int choice = 0;

    do {
        std::cout << "\n===== BOOK LIBRARY SYSTEM =====\n"
                  << "1. Add Book\n"
                  << "2. List Books\n"
                  << "3. Search Book\n"
                  << "4. Sort Books\n"
                  << "5. Issue Book\n"
                  << "6. Return Book\n"
                  << "7. Save Books\n"
                  << "8. Reload Books\n"
                  << "9. Exit\n"
                  << "10. Show Enhancement Demo\n"
                  << "Enter your choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Enter a number.\n";
            clearInput();
            continue;
        }

        switch (choice) {

        case 1: {
            int id = 0;
            std::string title;

            std::cout << "Enter Book ID: ";

            if (!(std::cin >> id)) {
                std::cout << "Invalid book ID.\n";
                clearInput();
                break;
            }

            clearInput();

            std::cout << "Enter Book Title: ";
            std::getline(std::cin, title);

            if (library.addBook(id, title)) {
                std::cout << "Book added successfully.\n";
            }
            else {
                std::cout << "Invalid ID/title or "
                          << "duplicate Book ID.\n";
            }

            break;
        }

        case 2:
            library.listBooks();
            break;

        case 3: {
            int id = 0;

            std::cout << "Enter Book ID to search: ";

            if (!(std::cin >> id)) {
                std::cout << "Invalid book ID.\n";
                clearInput();
                break;
            }

            const Book* book = library.searchBook(id);

            if (book) {
                std::cout << "Book Found: "
                          << book->title
                          << " | Status: "
                          << (book->isIssued
                                  ? "Issued"
                                  : "Available")
                          << '\n';
            }
            else {
                std::cout << "Book not found.\n";
            }

            break;
        }

        case 4:
            library.sortBooks();
            std::cout << "Books sorted by title.\n";
            break;

        case 5:
        case 6: {
            int id = 0;

            std::cout << (choice == 5
                              ? "Enter Book ID to issue: "
                              : "Enter Book ID to return: ");

            if (!(std::cin >> id)) {
                std::cout << "Invalid book ID.\n";
                clearInput();
                break;
            }

            const auto result =
                choice == 5
                    ? library.issueBook(id)
                    : library.returnBook(id);

            if (result == BookOperationResult::Success) {
                std::cout << (choice == 5
                                  ? "Book issued.\n"
                                  : "Book returned.\n");
            }
            else if (result ==
                     BookOperationResult::BookNotFound) {
                std::cout << "Book ID not found.\n";
            }
            else if (result ==
                     BookOperationResult::AlreadyIssued) {
                std::cout << "Book is already issued.\n";
            }
            else {
                std::cout << "Book was not issued.\n";
            }

            break;
        }

        case 7:
            showFileResult(
                library.saveToFile(dataFile),
                "Save"
            );
            break;

        case 8:
            showFileResult(
                library.loadFromFile(dataFile),
                "Reload"
            );
            break;

        case 9:
            std::cout << "Exiting. Use option 7 to save "
                      << "changes before exit.\n";
            break;

        case 10:
            showEnhancementDemo();
            break;

        default:
            std::cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 9);

    return 0;
}
