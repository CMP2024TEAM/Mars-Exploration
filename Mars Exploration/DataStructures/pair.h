#pragma once
template<class T, class U>
struct pair {
    T first;
    U second;

    pair(const T& First_, const U& Second_) :
        first(First_),
        second(Second_)
    {}
};