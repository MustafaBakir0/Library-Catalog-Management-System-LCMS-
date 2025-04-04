#include "borrower.h"

using namespace std;
// paramterized constructor 
Borrower::Borrower(string Name, string ID) : name(Name), id(ID), books_borrowed() {}


void Borrower::listBooks() {
    if (books_borrowed.empty()) {
        cout << "No books currently borrowed by " << name << ".\n";
        return;
    }

    cout << "Books currently borrowed by " << name << ":\n";

    for (int i = 0; i < books_borrowed.size(); ++i) {
        const Book* book = books_borrowed[i];
        cout << "Title          : " << book->title << "\n";
        cout << "Author(s)      : " << book->author << "\n";
        cout << "ISBN           : " << book->isbn << "\n";
        cout << "Publication Year : " << book->publication_year << "\n";
        cout << "-------------------------\n";
    }
}
