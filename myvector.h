//============================================================================
// Name         : myvector.h
// Author       : 
// Version      : 1.0
// Date Created : 
// Date Modified:
// Description  : Vector implementation in C++
//============================================================================
#ifndef MYVECTOR_H
#define MYVECTOR_H
#include<iostream>
#include<cstdlib>
#include<iomanip>
#include <stdexcept>
#include<sstream>

using namespace std;
template <typename T>
class MyVector
{
	private:
		T *data;						//pointer to int(array) to store elements
		int v_size;						//current size of vector (number of elements in vector)
		int v_capacity;					//capacity of vector
	public:
		MyVector();						//No argument constructor
		MyVector(int cap);				//One Argument Constructor
		MyVector(const MyVector& other);		//Copy Constructor
		~MyVector();					//Destructor
		void push_back(T element);		//Add an element at the end of vector
		void insert(int index, T element); //Add an element at the index 
		void erase(int index);			//Removes an element from the index
		T& operator[](int index);		//return reference of the element at index
		T& at(int index); 				//return reference of the element at index
		const T& front();				//Returns reference of the first element in the vector
		const T& back();				//Returns reference of the Last element in the vector
		int size() const;				//Return current size of vector
		int capacity() const;			//Return capacity of vector
		bool empty() const; 			//Return true if the vector is empty, False otherwise
		void shrink_to_fit();			//Reduce vector capacity to fit its size

		void resize(int v_size); // resize vector
		MyVector<T> operator++(int);  // postfix increment
        MyVector<T>& operator=(const MyVector<T>& other);


        // Iterator support
        typedef T* iterator;
        typedef const T* const_iterator;

        class Iterator;  // forward declaration for custom iterator class

        Iterator begin(); // returns an iterator to the first element
        Iterator end();  // returns an iterator past the last element

public:
    // custom iterator class
    class Iterator {
    private:
        T* ptr;  // pointer to the current element

    public:
        // constructor
        Iterator(T* p = nullptr) : ptr(p) {}

        // dereference operator
        T& operator*() const { return *ptr; }

        // pre-increment operator (++it)
        Iterator& operator++() {
            ++ptr;
            return *this;
        }

        // post-increment operator (it++)
        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr;
            return temp;
        }

        // equality comparison operator
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }

        // inequality comparison operator
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }

        // add integer to iterator
        Iterator operator+(int i) const {
            return Iterator(ptr + i);
        }

        // subtract integer from iterator
        Iterator operator-(int i) const {
            return Iterator(ptr - i);
        }
    };
};
//========================================
// no argument constructor
template <typename T>
MyVector<T>::MyVector() : data(new T[1]), v_size(0), v_capacity(1) { }

//========================================
// one argument constructor with capacity
template <typename T>
MyVector<T>::MyVector(int cap) : v_size(0), v_capacity(cap) {
    if (cap < 1) throw std::invalid_argument("capacity must be positive");
    data = new T[v_capacity];
}
//========================================
// copy constructor
template <typename T>
MyVector<T>::MyVector(const MyVector& other) : v_size(other.v_size), v_capacity(other.v_capacity) {
    data = new T[v_capacity];
    for (int i = 0; i < v_size; ++i) {
        data[i] = other.data[i];
    }
}
//========================================
// destructor to free memory
template <typename T>
MyVector<T>::~MyVector() {
    delete[] data;
}
//========================================
// return current size of vector
template <typename T>
int MyVector<T>::size() const {
    return v_size;
}
//========================================
// return current capacity of vector
template <typename T>
int MyVector<T>::capacity() const {
    return v_capacity;
}
//========================================
// check if vector is empty
template <typename T>
bool MyVector<T>::empty() const {
    return v_size == 0;
}
//========================================
// add an element to the end of vector
template <typename T>
void MyVector<T>::push_back(T element) {
    if (v_size == v_capacity) {
        resize(2 * v_capacity); // double the capacity if full
    }
    data[v_size++] = element; // add element and increase size
}
//===============================================================================
// insert an element at a specific index
template <typename T>
void MyVector<T>::insert(int index, T element) {
    if (index < 0 || index > v_size) {
        throw std::out_of_range("index out of range");
    }
    if (v_size == v_capacity) {
        resize(2 * v_capacity); // double the capacity if full
    }
    // shift elements to the right
    for (int i = v_size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = element; // insert the new element
    ++v_size; // increase size
}
//================================================================================
// erase an element at a specific index
template <typename T>
void MyVector<T>::erase(int index) {
    if (index < 0 || index >= v_size) {
        throw std::out_of_range("index out of range");
    }
    // shift elements to the left
    for (int i = index; i < v_size - 1; ++i) {
        data[i] = data[i + 1];
    }
    --v_size; // decrease size
}
//==================================================================================
// access element without bounds checking
template <typename T>
T& MyVector<T>::operator[](int index) {
    return data[index];
}
//========================================
// access element with bounds checking
template <typename T>
T& MyVector<T>::at(int index) {
    if (index < 0 || index >= v_size) {
        throw std::out_of_range("index out of range");
    }
    return data[index];
}
//========================================
// get the first element
template <typename T>
const T& MyVector<T>::front() {
    if (empty()) {
        throw std::out_of_range("vector is empty");
    }
    return data[0];
}
//========================================
// get the last element
template <typename T>
const T& MyVector<T>::back() {
    if (empty()) {
        throw std::out_of_range("vector is empty");
    }
    return data[v_size - 1];
}
//======================================
// reduce capacity to fit size
template <typename T>
void MyVector<T>::shrink_to_fit() {
    if (v_capacity > v_size) {
        T* new_data = new T[v_size];
        for (int i = 0; i < v_size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        v_capacity = v_size;
    }
}
//======================================
// resize the vector to new capacity
template <typename T>
void MyVector<T>::resize(int new_capacity) {
    if (new_capacity < v_size) {
        v_size = new_capacity; // adjust size if new capacity is smaller
    }
    T* new_data = new T[new_capacity];
    for (int i = 0; i < v_size; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    v_capacity = new_capacity;
}
//======================================
// postfix increment operator
template <typename T>
MyVector<T> MyVector<T>::operator++(int) {
    MyVector<T> temp = *this;
    if (v_size == v_capacity) {
        resize(2 * v_capacity); // double the capacity if full
    }
    ++v_size; // increase size
    return temp;
}
//========================================
// begin iterator
template <typename T>
typename MyVector<T>::Iterator MyVector<T>::begin() {
    return Iterator(data);  // return iterator pointing to first element
}
//========================================
// end iterator
template <typename T>
typename MyVector<T>::Iterator MyVector<T>::end() {
    return Iterator(data + v_size);  // return iterator pointing past last element
}
//========================================
// assignment operator
template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other) {
    if (this != &other) {
        // delete current data
        delete[] data;
        // copy size and capacity
        v_size = other.v_size;
        v_capacity = other.v_capacity;
        // allocate new data
        data = new T[v_capacity];
        // copy elements
        for (int i = 0; i < v_size; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}
#endif







