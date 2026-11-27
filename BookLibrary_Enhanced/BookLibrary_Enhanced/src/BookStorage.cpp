#include "BookStorage.h"

#include <cerrno>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <unordered_set>

FileOperationResult BookStorage::save(const std::vector<Book>& books,
                                      const std::string& filename) const {
    const std::string temporary = filename + ".tmp";
    const std::string backup = filename + ".bak";

    {
        std::ofstream out(temporary.c_str(), std::ios::trunc);
        if (!out) return FileOperationResult::IoError;

        out << "BOOK_LIBRARY_V1\n" << books.size() << '\n';
        for (const Book& book : books) {
            out << book.id << ' ' << (book.isIssued ? 1 : 0) << ' '
                << std::quoted(book.title) << '\n';
            if (!out) {
                out.close();
                std::remove(temporary.c_str());
                return FileOperationResult::IoError;
            }
        }
        out.flush();
        if (!out) {
            out.close();
            std::remove(temporary.c_str());
            return FileOperationResult::IoError;
        }
    }

    std::remove(backup.c_str());
    bool hadOriginal = false;
    errno = 0;
    if (std::rename(filename.c_str(), backup.c_str()) == 0) {
        hadOriginal = true;
    } else if (errno != ENOENT) {
        std::remove(temporary.c_str());
        return FileOperationResult::IoError;
    }

    if (std::rename(temporary.c_str(), filename.c_str()) != 0) {
        if (hadOriginal) std::rename(backup.c_str(), filename.c_str());
        std::remove(temporary.c_str());
        return FileOperationResult::IoError;
    }

    if (hadOriginal) std::remove(backup.c_str());
    return FileOperationResult::Success;
}

FileOperationResult BookStorage::load(std::vector<Book>& books,
                                     const std::string& filename) const {
    errno = 0;
    std::FILE* file = std::fopen(filename.c_str(), "rb");
    if (!file) {
        return errno == ENOENT ? FileOperationResult::FileNotFound
                               : FileOperationResult::IoError;
    }
    std::fclose(file);

    std::ifstream in(filename.c_str());
    if (!in) return FileOperationResult::IoError;

    std::string header;
    if (!std::getline(in, header) || header != "BOOK_LIBRARY_V1")
        return FileOperationResult::InvalidFormat;

    std::size_t count = 0;
    if (!(in >> count) || count > 100000)
        return FileOperationResult::InvalidFormat;

    std::vector<Book> loaded;
    loaded.reserve(count);
    std::unordered_set<int> ids;

    for (std::size_t i = 0; i < count; ++i) {
        int id = 0, issued = -1;
        std::string title;
        if (!(in >> id >> issued >> std::quoted(title)))
            return FileOperationResult::InvalidFormat;

        if (id <= 0 || (issued != 0 && issued != 1) ||
            title.empty() || !ids.insert(id).second)
            return FileOperationResult::InvalidFormat;

        loaded.emplace_back(id, title, issued == 1);
    }

    in >> std::ws;
    if (!in.eof()) return FileOperationResult::InvalidFormat;

    // Replace current data only after the whole file passes validation.
    books.swap(loaded);
    return FileOperationResult::Success;
}
