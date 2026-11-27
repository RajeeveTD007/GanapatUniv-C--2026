#include "Library.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    const std::string validFile = "test_books.txt";
    const std::string duplicateFile = "test_duplicate_books.txt";
    const std::string badFile = "test_bad_books.txt";
    const std::string missingFile = "test_missing_books.txt";

    Library original;
    assert(original.size() == 0);

    // Invalid input and duplicate record checks.
    assert(!original.addBook(0, "Invalid ID"));
    assert(!original.addBook(10, ""));
    assert(original.addBook(101, "C++ Programming"));
    assert(original.addBook(102, "Python Programming"));
    assert(!original.addBook(101, "Duplicate ID"));
    assert(original.size() == 2);

    assert(original.issueBook(102) == BookOperationResult::Success);
    assert(original.issueBook(102) == BookOperationResult::AlreadyIssued);
    assert(original.issueBook(999) == BookOperationResult::BookNotFound);
    assert(original.returnBook(101) == BookOperationResult::NotIssued);

    // Persistence round trip: save -> load -> compare important fields.
    assert(original.saveToFile(validFile) == FileOperationResult::Success);
    Library restored;
    assert(restored.loadFromFile(validFile) == FileOperationResult::Success);
    assert(restored.size() == original.size());
    assert(restored.searchBook(101)->title == "C++ Programming");
    assert(!restored.searchBook(101)->isIssued);
    assert(restored.searchBook(102)->title == "Python Programming");
    assert(restored.searchBook(102)->isIssued);

    // Missing file should not erase current data.
    std::remove(missingFile.c_str());
    assert(restored.loadFromFile(missingFile) == FileOperationResult::FileNotFound);
    assert(restored.size() == 2);

    // Invalid status flag.
    {
        std::ofstream out(badFile);
        out << "BOOK_LIBRARY_V1\n1\n103 7 \"Invalid Status\"\n";
    }
    assert(restored.loadFromFile(badFile) == FileOperationResult::InvalidFormat);
    assert(restored.size() == 2);

    // Duplicate IDs in a storage file.
    {
        std::ofstream out(duplicateFile);
        out << "BOOK_LIBRARY_V1\n2\n201 0 \"First\"\n201 0 \"Duplicate\"\n";
    }
    assert(restored.loadFromFile(duplicateFile) == FileOperationResult::InvalidFormat);
    assert(restored.size() == 2);

    std::remove(validFile.c_str());
    std::remove(badFile.c_str());
    std::remove(duplicateFile.c_str());
    std::cout << "All library and persistence tests passed.\n";
    return 0;
}
