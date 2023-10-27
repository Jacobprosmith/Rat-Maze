#ifndef PROJ3_STACK_H
#define PROJ3_STACK_H
#include <deque>


namespace cop4530 {
    template <typename T>
    class Stack {
    public:
        // Using iterator??

        Stack();
        ~Stack();
        Stack(const Stack &rhs);
        Stack(Stack &&rhs);
        const Stack& operator=(const Stack &rhs);
        Stack& operator=(Stack &&rhs);
        bool empty() const;
        T& top();
        const T& top() const;
        void pop();
        void push(const T& val);
        void push(T&& val);
        int size();

        // Data members
    private:
        std::deque<T> iDeque;
    };

#include "Stack.hpp"
}
#endif //PROJ3_STACK_H