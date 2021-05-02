#pragma once
template<class T, class U>
struct pair {
    T first;
    U second;

    pair() :
        first(),
        second()
    {}

    pair(const T& First, const U& Second) :
        first(First),
        second(Second)
    {}
};