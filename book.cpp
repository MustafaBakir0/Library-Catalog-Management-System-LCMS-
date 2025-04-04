#include "book.h"

// constructor
Book::Book(string title, string author, string isbn, int publication_year, int total_copies, int available_copies)
    : title(title), author(author), isbn(isbn),
    publication_year(publication_year),
    total_copies(total_copies),
    available_copies(available_copies) {
    currentBorrowers = MyVector<Borrower*>();
    allBorrowers = MyVector<Borrower*>();
}

void Book::display() { // display book information
    cout << "----------------------------------------------------\n";
    cout << "Title:               " << title << endl;
    cout << "Author(s):           " << author << endl;
    cout << "ISBN:                " << isbn << endl;
    cout << "Year:                " << publication_year << endl;
    cout << "Total copies:        " << total_copies << endl;
    cout << "Available copies:    " << available_copies - currentBorrowers.size() << endl;
    cout << "----------------------------------------------------\n";
}

// overloaded == operator to compare books by isbn
bool Book::operator==(const Book& b) const {
    return this->isbn == b.isbn;
}