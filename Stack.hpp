#ifndef PROJ3_STACK_HPP
#define PROJ3_STACK_HPP
#include "Stack.h"
#include <deque>

using namespace std;

template <typename T>
Stack<T>::Stack() {

}

template <typename T>
Stack<T>::~Stack() {

}

template <typename T>
Stack<T>::Stack(const Stack &rhs) {
    iDeque = rhs.iDeque;
}

template <typename T>
Stack<T>::Stack(Stack &&rhs) {
    iDeque = rhs.iDeque;
    rhs = nullptr;
}

template <typename T>
const Stack<T>& Stack<T>::operator=(const Stack &rhs) {
    iDeque = rhs.iDeque;
    return *this;
}

template <typename T>
Stack<T>& Stack<T>::operator=(Stack &&rhs) {
    iDeque = rhs.iDeque;
    rhs = nullptr;
    return *this;
}

template <typename T>
bool Stack<T>::empty () const {
    return iDeque.empty();
}

template <typename T>
T& Stack<T>::top() {
    return iDeque.back();
}

template <typename T>
const T& Stack<T>::top() const {
    return iDeque.back();
}

template <typename T>
void Stack<T>::pop() {
    iDeque.pop_back();
}

template <typename T>
void Stack<T>::push(const T& val) {
    iDeque.push_back(val);
}

template <typename T>
void Stack<T>::push(T&& val) {
    iDeque.push_back(val);
}

template <typename T>
int Stack<T>::size() {
    return iDeque.size();
}

#endif //PROJ3_STACK_HPP
