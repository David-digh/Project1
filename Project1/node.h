#pragma once
#include <iostream>
template <class T>
struct node
{
    T data;
    node<T>* next;
    node(T a):data(a)
    {
        next = NULL;
    }
    node() { next = NULL; }
};