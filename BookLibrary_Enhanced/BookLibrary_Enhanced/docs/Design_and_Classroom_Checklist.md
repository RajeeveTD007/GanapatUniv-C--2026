# Design and Classroom Checklist

## Refactoring decisions
- **Repeated logic:** `findBook()` is used by add/search/issue/return.
- **UI:** menu display, console input and user messages stay in `main.cpp`.
- **Business rules:** add/issue/return/search/sort stay in `Library`.
- **Storage:** file format, file I/O and record validation stay in `BookStorage`.
- **Model:** `Book` contains only book data.

## Group demonstration (5–8 minutes)
- Show adding two books and rejecting a duplicate ID.
- Issue one book; show its status.
- Save, exit or reload, then verify the status remains issued.
- Show one invalid input and explain the message.

## Individual task (choose one)
- Reject titles containing only spaces and add a test.
- Add a test for returning a book that was never issued.
- Debug a deliberately failing save/load assertion.

## Explain these design decisions
- Why does `main.cpp` not write directly to `books.txt`?
- Why does `BookStorage` load into a temporary vector before replacing current data?
- Why are duplicate IDs checked both when adding and when loading?
- What behavior should be tested after a persistence round trip?
