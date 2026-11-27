#ifndef BOOK_H
#define BOOK_H

#include <string>

struct Book {
    int id = 0;
    std::string title;
    bool isIssued = false;

    Book() = default;
    Book(int bookId, const std::string& bookTitle, bool issued = false)
        : id(bookId), title(bookTitle), isIssued(issued) {}
};

#endif
