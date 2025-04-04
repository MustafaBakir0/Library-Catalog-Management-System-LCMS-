
# Library Catalog Management System (LCMS)

## Overview
The **Library Catalog Management System (LCMS)** is a C++ application designed to manage books, categories, and borrowers in a library. It provides functionalities for importing/exporting book data, searching for books, managing categories, and tracking book borrowings.

---

## Key Features

### 📚 Book Management
- Add, edit, and remove books  
- Search for books by title  
- Display detailed book information  
- Import/export book data from/to CSV files  

### 🗂️ Category Management
- Hierarchical category system (categories/subcategories)  
- Add, remove, and find categories  
- Display category tree structure  

### 👤 Borrower Management
- Track current and historical borrowers  
- List books borrowed by a specific borrower  
- Manage book borrowing and returning  

---

## Technical Implementation

- **Custom vector implementation** (`MyVector`) with iterator support  
- **Tree data structure** for category hierarchy  
- **Object-oriented design** with proper encapsulation  
- **Memory management** with destructors  

---

## Data Structures

### 🔹 MyVector
A custom vector implementation with:
- Dynamic resizing  
- Iterator support  
- Common vector operations (`push_back`, `insert`, `erase`, etc.)

### 🔹 Tree
A hierarchical structure for categories with:
- Node-based implementation  
- Recursive traversal methods  
- Book count tracking at each node  

---

## Class Structure

### Main Classes

- `Book`: Represents a book with title, author, ISBN, etc.  
- `Borrower`: Represents a library member who can borrow books  
- `Node`: Represents a category or subcategory in the tree  
- `Tree`: Manages the hierarchical category structure  
- `LCMS`: Main system class that coordinates all functionality  

---

## Usage

The system is operated through a **command-line interface** with commands like:

```
import <file>            # Import books from a CSV file
findBook <title>         # Search for a book
addCategory <path>       # Add a new category
borrowBook <title>       # Borrow a book
list                     # Display the category tree
```

---

## Compilation and Execution

To compile and run the system:

1. Ensure all header and implementation files are in the same directory  
2. Compile with a C++ compiler (e.g., g++):

```bash
g++ main.cpp book.cpp borrower.cpp tree.cpp -o lcms
```

3. Run the executable:

```bash
./lcms
```

---

## Example Commands

```bash
import books.csv
findBook "The Great Gatsby"
addCategory Fiction/Classics
borrowBook "1984"
list
```

---

## Design Considerations

- Memory safety with proper destructors  
- Efficient searching through tree traversal  
- Flexible data storage with custom vector implementation  
- Clear separation of concerns between classes  

---

This system provides a **comprehensive solution** for managing library catalogs with a focus on **efficient organization** and **easy access** to book information.
