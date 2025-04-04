#include "lcms.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

// constructor for LCMS class
LCMS::LCMS(string name) {
    libTree = new Tree("lib"); // create a new tree with root named 'lib'
}

// destructor for LCMS class
LCMS::~LCMS() {
    delete libTree; // delete the tree to free memory
    // delete all borrowers to free memory
    for (int i = 0; i < borrowers.size(); i++) {
        delete borrowers[i];
    }
}

// function to import data from a file
int LCMS::import(string path) {
    ifstream testFile(path); // try to open the file
    if (!testFile.good()) {
        cout << "error: file does not exist or cannot be accessed at: " << path << endl;
        return 0; // return if file cannot be opened
    }
    testFile.close();

    ifstream file(path); // open the file for reading
    if (!file.is_open()) {
        cout << "error opening file at: " << path << endl;
        return 0; // return if file cannot be opened
    }

    try {
        int importedCount = 0; // count of records imported
        string line;
        bool firstLine = true; // flag to skip the header line

        while (getline(file, line)) {
            if (firstLine) {
                firstLine = false; // skip the first line
                continue;
            }

            if (line.empty()) continue; // skip empty lines

            MyVector<string> fields; // vector to store the fields
            string currentField;
            bool inQuotes = false; // flag for quoted fields

            // parse the line into fields
            for (size_t i = 0; i < line.length(); i++) {
                if (line[i] == '"') {
                    inQuotes = !inQuotes; // toggle the inQuotes flag
                }
                else if (line[i] == ',' && !inQuotes) {
                    fields.push_back(currentField); // add field to vector
                    currentField.clear(); // clear current field
                }
                else {
                    currentField += line[i]; // append character to current field
                }
            }
            fields.push_back(currentField); // add the last field

            if (fields.size() != 7) {
                cout << "skipping line: invalid number of fields" << endl;
                continue; // skip lines with incorrect number of fields
            }

            try {
                // remove quotes from fields if present
                for (int i = 0; i < fields.size(); i++) {
                    if (fields[i].length() >= 2 && fields[i].front() == '"' && fields[i].back() == '"') {
                        fields[i] = fields[i].substr(1, fields[i].length() - 2); // remove quotes
                    }
                }

                // assign fields to variables
                string title = fields[0];
                string author = fields[1];
                string isbn = fields[2];
                int publication_year = stoi(fields[3]);
                string category = fields[4];
                int total_copies = stoi(fields[5]);
                int available_copies = stoi(fields[6]);

                // check for invalid numeric values
                if (total_copies < 0 || available_copies < 0) {
                    cout << "skipping line: invalid numeric values" << endl;
                    continue; // skip if numeric values are invalid
                }

                // check if available copies exceed total copies
                if (available_copies > total_copies) {
                    cout << "skipping line: available copies greater than total copies" << endl;
                    continue; // skip if available copies exceed total copies
                }

                // create a new book object
                Book* newBook = new Book(title, author, isbn, publication_year, total_copies, available_copies);
                if (!newBook) {
                    cout << "failed to create book object" << endl;
                    continue; // continue if book creation fails
                }

                // create or get the category node
                Node* categoryNode = libTree->createNode(category);
                if (!categoryNode) {
                    cout << "failed to create category node for: " << category << endl;
                    delete newBook; // delete the book if category node creation fails
                    continue;
                }

                // add the book to the category
                categoryNode->books.push_back(newBook);
                categoryNode->bookCount++; // increment the book count

                // update book count in parent nodes
                Node* current = categoryNode->parent;
                while (current != nullptr) {
                    current->bookCount++;
                    current = current->parent;
                }

                importedCount++; // increment the count of imported records
            }
            catch (const std::exception& e) {
                cout << "error processing line: " << e.what() << endl;
                continue; // continue on exception
            }
        }

        file.close();
        cout << importedCount << " records have been imported." << endl;
        return importedCount; // return the number of records imported
    }
    catch (...) {
        file.close();
        cout << "fatal error occurred during import" << endl;
        return 0; // return zero on fatal error
    }
}

// function to export data to a file
void LCMS::exportData(string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "error: unable to open file for writing." << endl;
        return; // return if file cannot be opened
    }

    // write header line
    file << "Title,Author,ISBN,Publication Year,Category,Total Copies,Available Copies\n";

    // export data from the tree
    int count = libTree->exportData(libTree->getRoot(), file);

    file.close();

    cout << count << " records have been fully exported to file" << endl; // output total count
}
// function to find all books in a category path
void LCMS::findAll(string categoryPath) {
    Node* categoryNode = libTree->getNode(categoryPath);
    if (!categoryNode) {
        cout << "category not found: " << categoryPath << endl;
        return;
    }

    libTree->printAll(categoryNode); // print all books in the category
}

// function to find a specific book by title
void LCMS::findBook(string bookTitle) {
    Book* book = libTree->findBook(libTree->getRoot(), bookTitle);
    if (book) {
        cout << "book found in the library:\n";
        book->display(); // display the book details
    }
    else {
        cout << "book not found!" << endl;
    }
}

// function to add a new book
void LCMS::addBook() {
    string title, author, isbn, category;
    int publication_year, total_copies, available_copies;

    cout << "enter title: ";
    getline(cin, title); // get book title

    cout << "enter author(s): ";
    getline(cin, author); // get book author(s)

    cout << "enter isbn: ";
    getline(cin, isbn); // get book isbn

    cout << "enter publication year: ";
    string year_str;
    getline(cin, year_str);
    try {
        publication_year = stoi(year_str); // convert publication year to integer
    }
    catch (...) {
        cout << "invalid publication year." << endl;
        return; // return if conversion fails
    }

    cout << "enter category: ";
    getline(cin, category); // get book category

    cout << "enter number of total copies: ";
    string total_str;
    getline(cin, total_str);
    try {
        total_copies = stoi(total_str); // convert total copies to integer
    }
    catch (...) {
        cout << "invalid number of total copies." << endl;
        return; // return if conversion fails
    }

    cout << "enter number of available copies: ";
    string available_str;
    getline(cin, available_str);
    try {
        available_copies = stoi(available_str); // convert available copies to integer
    }
    catch (...) {
        cout << "invalid number of available copies." << endl;
        return; // return if conversion fails
    }

    // check if available copies exceed total copies
    if (available_copies > total_copies) {
        cout << "available copies cannot exceed total copies." << endl;
        return; // return if available copies are more than total copies
    }

    // create a new book object
    Book* newBook = new Book(title, author, isbn, publication_year, total_copies, available_copies);

    // create or get the category node
    Node* categoryNode = libTree->createNode(category);
    if (!categoryNode) {
        cout << "failed to create/find category node." << endl;
        delete newBook; // delete the book if category node creation fails
        return;
    }

    // add the book to the category
    categoryNode->books.push_back(newBook);
    categoryNode->bookCount++; // increment the book count

    // update book count in parent nodes
    Node* current = categoryNode->parent;
    while (current != nullptr) {
        current->bookCount++;
        current = current->parent;
    }

    cout << title << " has been successfully added into the catalog." << endl;
}

// helper function to get a valid integer from user
int getValidInteger(const string& prompt) {
    string input;
    int value;
    while (true) {
        cout << prompt;
        getline(cin, input);
        istringstream stream(input);
        if (stream >> value && stream.eof()) {
            return value; // return the integer if valid
        }
        else {
            cout << "invalid input. please enter a valid integer." << endl;
        }
    }
}

// function to edit a book's details
void LCMS::editBook(string bookTitle) {
    Book* book = libTree->findBook(libTree->getRoot(), bookTitle);
    if (!book) {
        cout << "book not found!" << endl;
        return; // return if book not found
    }

    int choice;
    while (true) {
        // display edit options
        cout << "choose the field you want to edit:\n"
            << "1: title\n"
            << "2: author\n"
            << "3: isbn\n"
            << "4: publication year\n"
            << "5: total copies\n"
            << "6: available copies\n"
            << "7: exit\n"
            << "enter your choice: ";
        choice = getValidInteger("");

        switch (choice) {
        case 1:
            cout << "enter new title: ";
            getline(cin, book->title); // edit the title
            break;
        case 2:
            cout << "enter new author: ";
            getline(cin, book->author); // edit the author
            break;
        case 3:
            cout << "enter new isbn: ";
            getline(cin, book->isbn); // edit the isbn
            break;
        case 4:
            book->publication_year = getValidInteger("enter new publication year: "); // edit the publication year
            break;
        case 5:
            book->total_copies = getValidInteger("enter new total copies: "); // edit the total copies
            break;
        case 6:
            book->available_copies = getValidInteger("enter new available copies: "); // edit the available copies
            break;
        case 7:
            cout << "exiting edit..." << endl;
            return; // exit the edit menu
        default:
            cout << "invalid choice, please try again." << endl;
            break;
        }
        cout << "book details updated successfully!" << endl;
    }
}

// function to borrow a book
void LCMS::borrowBook(string bookTitle) {
    Book* book = libTree->findBook(libTree->getRoot(), bookTitle);
    if (!book) {
        cout << "book not found in the catalog." << endl;
        return; // return if book not found
    }

    if (book->available_copies <= 0) {
        cout << "no available copies of " << bookTitle << " at the moment." << endl;
        return; // return if no copies are available
    }

    string borrowerName, borrowerId;
    cout << "enter borrower's name: ";
    getline(cin, borrowerName); // get borrower's name
    cout << "enter borrower's id: ";
    getline(cin, borrowerId); // get borrower's id

    // check if borrower already exists
    Borrower* borrower = nullptr;
    for (int i = 0; i < borrowers.size(); i++) {
        if (borrowers[i]->id == borrowerId) {
            borrower = borrowers[i];
            break;
        }
    }
    if (!borrower) {
        // create new borrower
        borrower = new Borrower(borrowerName, borrowerId);
        borrowers.push_back(borrower);
    }

    // check if borrower has already borrowed the book
    for (int i = 0; i < book->currentBorrowers.size(); i++) {
        if (book->currentBorrowers[i]->id == borrowerId) {
            cout << "this borrower has already borrowed this book." << endl;
            return;
        }
    }

    // add to current borrowers
    book->currentBorrowers.push_back(borrower);

    // avoid duplicates in allBorrowers
    bool alreadyBorrowed = false;
    for (int i = 0; i < book->allBorrowers.size(); i++) {
        if (book->allBorrowers[i]->id == borrowerId) {
            alreadyBorrowed = true;
            break;
        }
    }
    if (!alreadyBorrowed) {
        book->allBorrowers.push_back(borrower);
    }

    // decrement available copies
    book->available_copies--;

    cout << "book " << bookTitle << " has been issued to " << borrowerName << endl;
}

// function to return a book
void LCMS::returnBook(string title) {
    Book* book = libTree->findBook(libTree->getRoot(), title);
    if (!book) {
        cout << "book not found in the catalog." << endl;
        return; // return if book not found
    }

    string borrowerName, borrowerId;
    cout << "enter borrower's name: ";
    getline(cin, borrowerName); // get borrower's name
    cout << "enter borrower's id: ";
    getline(cin, borrowerId); // get borrower's id

    bool found = false;
    for (int i = 0; i < book->currentBorrowers.size(); i++) {
        if (book->currentBorrowers[i]->id == borrowerId) {
            book->currentBorrowers.erase(i); // remove borrower from current borrowers
            book->available_copies++; // increment available copies
            found = true;
            break;
        }
    }

    if (found) {
        cout << "book has been successfully returned." << endl;
    }
    else {
        cout << "this borrower has not borrowed this book." << endl;
    }
}

// function to list current borrowers of a book
void LCMS::listCurrentBorrowers(string bookTitle) {
    Book* book = libTree->findBook(libTree->getRoot(), bookTitle);
    if (!book) {
        cout << "book not found in the catalog." << endl;
        return; // return if book not found
    }

    if (book->currentBorrowers.size() == 0) {
        cout << "no current borrowers for this book." << endl;
        return; // return if no current borrowers
    }

    cout << "current borrowers of " << bookTitle << ":" << endl;
    for (int i = 0; i < book->currentBorrowers.size(); i++) {
        cout << i + 1 << ". " << book->currentBorrowers[i]->name
            << " (" << book->currentBorrowers[i]->id << ")" << endl; // display borrower info
    }
}

// function to list all borrowers of a book
void LCMS::listAllBorrowers(string bookTitle) {
    Book* book = libTree->findBook(libTree->getRoot(), bookTitle);
    if (!book) {
        cout << "book not found in the catalog." << endl;
        return;
    }

    if (book->allBorrowers.size() == 0) {
        cout << "no borrowers have ever borrowed this book." << endl;
        return;
    }

    cout << "all borrowers of " << bookTitle << ":" << endl;
    for (int i = 0; i < book->allBorrowers.size(); i++) {
        cout << i + 1 << ". " << book->allBorrowers[i]->name << " (" << book->allBorrowers[i]->id << ")" << endl;
    }
}

// function to list all books borrowed by a user
void LCMS::listBooks(string borrower_name_id) {
    size_t commaPos = borrower_name_id.find(',');
    if (commaPos == string::npos) {
        cout << "invalid input format. use: name,id" << endl;
        return; // return if input format is invalid
    }

    string name = borrower_name_id.substr(0, commaPos);
    string id = borrower_name_id.substr(commaPos + 1);

    cout << "books borrowed by " << name << " (" << id << "):" << endl;
    int bookCount = 0;

    Node* current = libTree->getRoot();
    if (!current) {
        cout << "no books in the catalog." << endl;
        return; // return if root node is null
    }

    // use a helper function to traverse the tree
    bookCount = listAllBooksHelper(current, id);

    if (bookCount == 0) {
        cout << "no borrowing history found for this user." << endl;
    }
}

// helper function to list all books borrowed by a user
int LCMS::listAllBooksHelper(Node* node, const string& borrowerId) {
    if (!node) return 0;

    int count = 0;

    // check each book in the current node
    for (int i = 0; i < node->books.size(); i++) {
        Book* book = node->books[i];
        for (int j = 0; j < book->allBorrowers.size(); j++) {
            if (book->allBorrowers[j]->id == borrowerId) {
                cout << "- " << book->title << endl;
                count++;
                break;
            }
        }
    }

    // check each child node
    for (int i = 0; i < node->children.size(); i++) {
        count += listAllBooksHelper(node->children[i], borrowerId);
    }

    return count;
}

// function to remove a book from the catalog
void LCMS::removeBook(string bookTitle) {
    bool removed = libTree->removeBook(libTree->getRoot(), bookTitle);
    if (removed) {
        cout << "book removed successfully." << endl;
    }
    else {
        cout << "book not found!" << endl;
    }
}

// function to add a new category
void LCMS::addCategory(string path) {
    Node* newNode = libTree->createNode(path); // create or get the category node
    if (newNode) {
        cout << "category " << path << " has been created successfully." << endl;
    }
    else {
        cout << "failed to create category " << path << endl;
    }
}

// function to find a category
void LCMS::findCategory(const string categoryPath) {
    Node* categoryNode = libTree->getNode(categoryPath);
    if (categoryNode) {
        cout << "category found: " << categoryPath << endl;
    }
    else {
        cout << "category path not found." << endl;
    }
}

// function to remove a category
void LCMS::removeCategory(string path) {
    Node* node = libTree->getNode(path);
    if (!node) {
        cout << "category not found: " << path << endl;
        return; // return if category not found
    }

    if (libTree->isRoot(node)) {
        cout << "cannot remove root category." << endl;
        return; // return if attempting to remove root
    }

    if (node->bookCount > 0) {
        cout << "cannot remove category with books. remove all books first." << endl;
        return; // return if category has books
    }

    Node* parentNode = node->parent;
    if (parentNode) {
        for (int i = 0; i < parentNode->children.size(); i++) {
            if (parentNode->children[i] == node) {
                parentNode->children.erase(i); // remove node from parent's children
                delete node; // delete the node
                cout << "category " << path << " has been removed." << endl;
                return;
            }
        }
    }
}

// function to edit a categorys name
void LCMS::editCategory(string categoryPath) {
    Node* categoryNode = libTree->getNode(categoryPath);
    if (!categoryNode) {
        cout << "category not found: " << categoryPath << endl;
        return;
    }

    if (libTree->isRoot(categoryNode)) {
        cout << "cannot edit the root category name." << endl;
        return;
    }

    cout << "enter new name for the category: ";
    string newName;
    getline(cin, newName);

    if (newName.empty()) {
        cout << "category name cannot be empty." << endl;
        return;
    }

    // check if sibling categories have the same name
    Node* parentNode = categoryNode->parent;
    for (int i = 0; i < parentNode->children.size(); i++) {
        if (parentNode->children[i]->name == newName) {
            cout << "a category with this name already exists in the parent category." << endl;
            return;
        }
    }

    categoryNode->name = newName; // update the category name
    cout << "category name updated successfully." << endl;
}

// function to split a category path into individual categories
MyVector<string> LCMS::splitCategoryPath(const string& path) {
    MyVector<string> categories;
    string current;
    size_t pos = 0;
    size_t found;

    // split the path by '/'
    while ((found = path.find('/', pos)) != string::npos) {
        current = path.substr(pos, found - pos);
        if (!current.empty()) {
            categories.push_back(current); // add the category to the vector
        }
        pos = found + 1;
    }

    current = path.substr(pos);
    if (!current.empty()) {
        categories.push_back(current); // add the last category
    }

    return categories; // return the vector of categories
}
