#include "Library.h"

#include <algorithm>
#include <iostream>

Book* Library::findBook(int id) {
    auto it = std::find_if(books.begin(), books.end(),
        [id](const Book& book) { return book.id == id; });
    return it == books.end() ? nullptr : &*it;
}

const Book* Library::findBook(int id) const {
    auto it = std::find_if(books.begin(), books.end(),
        [id](const Book& book) { return book.id == id; });
    return it == books.end() ? nullptr : &*it;
}

bool Library::addBook(int id, const std::string& title) {
    if (id <= 0 || title.empty() || findBook(id)) return false;
    books.emplace_back(id, title);
    return true;
}

void Library::listBooks() const {
    if (books.empty()) {
        std::cout << "No books available.\n";
        return;
    }
    for (const Book& book : books) {
        std::cout << "ID: " << book.id << " | Title: " << book.title
                  << " | Status: " << (book.isIssued ? "Issued" : "Available")
                  << '\n';
    }
}

const Book* Library::searchBook(int id) const {
    return findBook(id);
}

void Library::sortBooks() {
    std::sort(books.begin(), books.end(),
        [](const Book& a, const Book& b) { return a.title < b.title; });
}

std::size_t Library::size() const {
    return books.size();
}

BookOperationResult Library::issueBook(int id) {
    Book* book = findBook(id);
    if (!book) return BookOperationResult::BookNotFound;
    if (book->isIssued) return BookOperationResult::AlreadyIssued;
    book->isIssued = true;
    return BookOperationResult::Success;
}

BookOperationResult Library::returnBook(int id) {
    Book* book = findBook(id);
    if (!book) return BookOperationResult::BookNotFound;
    if (!book->isIssued) return BookOperationResult::NotIssued;
    book->isIssued = false;
    return BookOperationResult::Success;
}

FileOperationResult Library::saveToFile(const std::string& filename) const {
    BookStorage storage;
    return storage.save(books, filename);
}

FileOperationResult Library::loadFromFile(const std::string& filename) {
    BookStorage storage;
    return storage.load(books, filename);
}
