#include "tree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

Node::Node(string name) : name(name), bookCount(0), parent(nullptr) {
    children = MyVector<Node*>(); // initialize children vector
    books = MyVector<Book*>(); // initialize books vector
}

Node::~Node() {
    for (int i = 0; i < children.size(); i++) {
        delete children[i]; // delete each child node to free memory
    }

    for (int i = 0; i < books.size(); i++) {
        delete books[i]; // delete each book to free memory
    }
}

void Node::setParent(Node* parentNode) {
    parent = parentNode; // set the parent node for the current node
}

string Node::getCategory(Node* node) {
    if (node->parent == nullptr) {
        return ""; // return empty string if node has no parent (root node)
    }

    string categoryPath = node->name; // start building path with nodes name
    Node* current = node->parent; // initialize current as the parent of the node

    while (current != nullptr && current->parent != nullptr) {
        categoryPath = current->name + "/" + categoryPath; // prepend parents name to path
        current = current->parent; // move up to the next parent
    }

    return categoryPath; // return the complete path
}

Tree::Tree(string rootName) {
    root = new Node(rootName); // create the root node with given name
}

Tree::~Tree() {
    delete root; // delete the root node and whole tree
}

Node* Tree::getRoot() {
    return root; // return pointer to the root node
}

bool Tree::isLastChild(Node* ptr) {
    return ptr != root && ptr == ptr->parent->children[ptr->parent->children.size() - 1];
    // check if node is last child in its parents children vector
}

bool Tree::isRoot(Node* node) {
    return node == root; // check if node is the root of the tree
}

Node* Tree::getNode(string path) {
    stringstream ss(path); // create a stringstream to parse the path
    string segment = ""; // temporary variable for each path segment
    Node* current = root; // start from the root node

    while (getline(ss, segment, '/')) { // parse path segments
        bool found = false; // flag to check if segment found in children
        for (int i = 0; i < current->children.size(); ++i) {
            if (current->children[i]->name == segment) { // check if childs name matches segment
                current = current->children[i]; // move to the matching child node
                found = true;
                break;
            }
        }
        if (!found) {
            return nullptr; // return null if segment is not found
        }
    }

    return current; // return the found node after iterating through path
}

Node* Tree::createNode(string path) {
    Node* current = root; // start from the root node
    string temp = path; // copy the path to a temporary variable
    size_t pos = 0; // variable for the position of /
    string segment; // variable to hold each path segment

    try {
        while ((pos = temp.find('/')) != string::npos) { // find position /
            segment = temp.substr(0, pos); // get segment
            temp = temp.substr(pos + 1); // update temp to remove processed segment

            if (!segment.empty()) { // check if segment is not empty
                bool found = false; // flag to check if segment is found in children
                for (int i = 0; i < current->children.size(); i++) {
                    if (current->children[i] && current->children[i]->name == segment) {
                        current = current->children[i]; // move to existing child node
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    Node* newNode = new Node(segment); // create a new node with the segment name
                    if (!newNode) {
                        cout << "Memory allocation failed" << endl;
                        return nullptr; // return null if memory allocation fails
                    }
                    newNode->setParent(current); // set current node as parent
                    current->children.push_back(newNode); // add new node to children
                    current = newNode; // move to the new node
                }
            }
        }

        if (!temp.empty()) { // if there is a remaining segment in temp
            bool found = false;
            for (int i = 0; i < current->children.size(); i++) {
                if (current->children[i] && current->children[i]->name == temp) {
                    current = current->children[i]; // move to existing child node
                    found = true;
                    break;
                }
            }

            if (!found) {
                Node* newNode = new Node(temp); // create a new node for the remaining segment
                if (!newNode) {
                    cout << "Memory allocation failed" << endl;
                    return nullptr; // return null if memory allocation fails
                }
                newNode->setParent(current); // set parent of the new node
                current->children.push_back(newNode); // add new node to children
                current = newNode; // move to the new node
            }
        }

        return current; // return the created or found node
    }
    catch (...) {
        cout << "Error processing path: " << path << endl;
        return nullptr; // return null on any error
    }
}

Node* Tree::getChild(Node* ptr, string childname) {
    if (ptr == nullptr) return nullptr; // return null if ptr is null

    for (int i = 0; i < ptr->children.size(); ++i) {
        if (ptr->children[i]->name == childname) {
            return ptr->children[i]; // return the child node if name matches
        }
    }

    return nullptr; // return null if child not found
}

void Tree::updateBookCount(Node* ptr, int offset) {
    if (ptr == nullptr) return; // do nothing if ptr is null

    ptr->bookCount += offset; // adjust the book count by offset value

    if (ptr->bookCount < 0) {
        ptr->bookCount = 0; // set to zero if count becomes negative
    }
}

Book* Tree::findBook(Node* node, string bookTitle) {
    if (node == nullptr) return nullptr; // return null if node is null

    // search for book in the current node’s books
    for (int i = 0; i < node->books.size(); ++i) {
        if (node->books[i]->title == bookTitle) {
            return node->books[i]; // return the book if found
        }
    }

    // if not found, recursively search in children
    for (int i = 0; i < node->children.size(); ++i) {
        Book* foundBook = findBook(node->children[i], bookTitle);
        if (foundBook != nullptr) {
            return foundBook; // return the book if found in child
        }
    }

    return nullptr; // return null if book is not found in the entire subtree
}

bool Tree::removeBook(Node* node, const string bookTitle) {
    if (node == nullptr) return false; // return false if node is null

    // search for book in the current node
    for (int i = 0; i < node->books.size(); ++i) {
        if (node->books[i]->title == bookTitle) {
            delete node->books[i]; // delete the book object
            node->books.erase(i); // remove the book from vector
            node->bookCount--; // decrement the book count

            // propagate book count change to parent nodes
            Node* current = node->parent;
            while (current != nullptr) {
                current->bookCount--;
                current = current->parent;
            }

            return true; // return true if book removed
        }
    }

    // if not found, recursively search in children
    for (int i = 0; i < node->children.size(); ++i) {
        if (removeBook(node->children[i], bookTitle)) {
            return true; // return true if book was removed from child
        }
    }

    return false; // return false if book not found
}

void Tree::printAll(Node* node) {
    if (node == nullptr) return; // do nothing if node is null

    cout << "Books in category \"" << node->name << "\":" << endl;
    for (int i = 0; i < node->books.size(); ++i) {
        node->books[i]->display(); // display each book in the category
    }

    for (int i = 0; i < node->children.size(); ++i) {
        printAll(node->children[i]); // recursively print books in child categories
    }
}

int Tree::exportData(Node* node, ofstream& file) {
    if (node == nullptr) return 0;

    int count = 0;

    for (int i = 0; i < node->books.size(); ++i) {
        Book* book = node->books[i];
        file << "\"" << book->title << "\","
            << "\"" << book->author << "\","
            << book->isbn << ","
            << book->publication_year << ","
            << "\"" << node->getCategory(node) << "\","
            << book->total_copies << ","
            << book->available_copies << "\n";
        ++count;
    }

    for (int i = 0; i < node->children.size(); ++i) {
        count += exportData(node->children[i], file);
    }

    return count; // Remove the cout statement from here
}

bool Tree::isEmpty() {
    return root == nullptr; // check if tree is empty
}

void Tree::print() {
    print_helper("", "", root); // call helper function
}

void Tree::print_helper(string padding, string pointer, Node* node) {
    if (node != nullptr) {
        cout << padding << pointer << node->name << "(" << node->bookCount << ")" << endl;

        if (node != root) padding += (isLastChild(node)) ? "   " : "|  ";

        for (int i = 0; i < node->children.size(); i++) {
            string marker = isLastChild(node->children[i]) ? "|___" : "|---";
            print_helper(padding, marker, node->children[i]); // recurse on child
        }
    }
}

void Tree::insert(Node* node, string name) {
    if (node == nullptr) {
        cerr << "Error: Parent node is null, cannot insert child node." << endl;
        return; // return if node is null
    }

    Node* newNode = new Node(name); // create new node
    node->children.push_back(newNode); // add to children
    newNode->setParent(node); // set parent

    cout << "Node with name " << name << " inserted as child of " << node->name << "." << endl;
}

void Tree::remove(Node* node, string child_name) {
    if (node == nullptr) {
        cerr << "Error: Parent node is null, cannot remove child node." << endl;
        return; // return if node is null
    }

    bool child_removed = false;

    for (int i = 0; i < node->children.size(); ++i) {
        if (node->children[i]->name == child_name) {
            for (int j = 0; j < node->children[i]->children.size(); ++j) {
                remove(node->children[i], node->children[i]->children[j]->name); // remove descendants
            }

            delete node->children[i]; // delete child node

            int index = i; // get index

            node->children.erase(index); // remove from children vector

            child_removed = true;
            cout << "Node with name \"" << child_name << "\" and its children removed from \"" << node->name << "\"." << endl;
            break;
        }
    }


    if (!child_removed) {
        cerr << "Error: Child with name \"" << child_name << "\" not found in node \"" << node->name << "." << endl;
    }

}