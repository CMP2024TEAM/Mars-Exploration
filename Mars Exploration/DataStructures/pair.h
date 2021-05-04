#pragma once
template<class T, class U>
struct MyPair {
    T first;
    U second;

    MyPair() :
        first(),
        second()
    {}

    MyPair(const T& First, const U& Second) :
        first(First),
        second(Second)
    {}
};