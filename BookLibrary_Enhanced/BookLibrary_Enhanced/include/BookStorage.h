#ifndef BOOK_STORAGE_H
#define BOOK_STORAGE_H

#include "Book.h"
#include <string>
#include <vector>

enum class FileOperationResult {
    Success,
    FileNotFound,
    InvalidFormat,
    IoError
};

// Storage responsibility: read/write the book file only.
class BookStorage {
public:
    FileOperationResult save(const std::vector<Book>& books,
                             const std::string& filename) const;
    FileOperationResult load(std::vector<Book>& books,
                             const std::string& filename) const;
};

#endif
