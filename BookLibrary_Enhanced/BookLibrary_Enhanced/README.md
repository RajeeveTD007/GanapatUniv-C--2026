# Enhanced Book Library (C++17)

## What changed
- Repeated book lookup is refactored into private `findBook()` helpers.
- Responsibilities are separated:
  - `main.cpp`: console UI and input handling
  - `Library`: book rules (add, search, issue, return, sort)
  - `BookStorage`: file save/load and file-format validation
  - `Book`: simple data model
- Existing behavior is retained: add/list/search/sort/issue/return/save/reload.
- Tests cover duplicate IDs, invalid input, invalid file records and save/load round trips.

## Build and run (Windows MinGW)
```bat
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
ctest --test-dir build --output-on-failure
build\BookLibrary.exe
```

The program stores data in `books.txt` in the current working directory.

## Student activity
1. In pairs/small groups, run the app and demonstrate add, search, issue, return, save and reload.
2. Individually complete one task: reject whitespace-only titles, add a test for it, or debug a failing persistence test.
3. Explain: why is file I/O in `BookStorage` and book rules in `Library`? Why is duplicate ID validation needed?

## Acceptance checks
- Duplicate IDs are rejected through the API and from a malformed data file.
- Invalid ID/title input is rejected.
- Issued status and titles survive save-load-save round trips.
- A failed load does not replace the existing in-memory library.
- `ctest` reports the library test as passed.
