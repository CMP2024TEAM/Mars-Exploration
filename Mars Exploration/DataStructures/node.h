#pragma once
template<class T>
struct node {
    T val;
    node<T>* next;

    node() :
        next(nullptr)
    {}

    node(const T& Value, node<T>* NextPtr = nullptr) :
        val(Value),
        next(NextPtr)
    {}
};