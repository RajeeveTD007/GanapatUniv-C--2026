#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "BookStorage.h"
#include <cstddef>
#include <string>
#include <vector>

enum class BookOperationResult {
    Success,
    BookNotFound,
    AlreadyIssued,
    NotIssued,
    InvalidData,
    DuplicateId
};

// Business rules: add/search/sort/issue/return.
// File details are delegated to BookStorage.
class Library {
private:
    std::vector<Book> books;

    Book* findBook(int id);
    const Book* findBook(int id) const;

public:
    bool addBook(int id, const std::string& title);
    void listBooks() const;
    const Book* searchBook(int id) const;
    void sortBooks();
    std::size_t size() const;

    BookOperationResult issueBook(int id);
    BookOperationResult returnBook(int id);

    FileOperationResult saveToFile(const std::string& filename) const;
    FileOperationResult loadFromFile(const std::string& filename);
};

#endif
