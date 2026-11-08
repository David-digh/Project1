#pragma once
#include "node.h"
#include <iostream>
#include <cstdlib>

template <class T>
class linkstack
{
private:
    node<T>* top;
public:
    linkstack()
    {
        top = NULL;
    }
    virtual ~linkstack()
    {
        node<T>* p;
        while (top != NULL)
        {
            p = top;
            top = top->next;
            delete p;
        }
    }
    linkstack(const linkstack<T>& a)
    {
        top = NULL;
        if (a.gettop() == NULL)         
        {
            return;
        }
        top = new node<T>(a.top->data);
        node<T>* p = top;
        for (node<T>* t = a.top->next; t != NULL; t = t->next)
        {
            p->next = new node<T>(t->data);
            p = p->next;
        }
    }
    node<T>* gettop()
    {
        return top;
    }
    bool isempty()
    {
        return top == NULL;
    }
    bool push(T x)                          // 入栈，返回 true 表示入栈成功
    {
        node<T>* s = new node<T>;
        s->data = x;
        s->next = top;
        top = s;
        return true;
    }
    bool pop(T& t)                          // 出栈
    {
        if (isempty())
        {
            std::cerr << "栈空，不能出栈！" << std::endl;
            std::exit(1);
        }
        node<T>* p = top;
        T x = p->data;
        top = p->next;
        delete p;
        t = x;
        return true;
    }
    T gethead()                             // 取栈顶元素
    {
        if (isempty())
        {
            std::cerr << "栈空，没有栈顶元素！" << std::endl;
            std::exit(1);
        }
        return top->data;
    }
};