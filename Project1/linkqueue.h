#pragma once
#include "node.h"
#include <iostream>
#include <cstdlib>

template <class T>
class linkqueue
{
private:
    node<T>* front;
    node<T>* rear;
public:
    linkqueue()
    {
        front = new node<T>;
        front->next = NULL;
        rear = front;
    }
    linkqueue(const linkqueue<T>& a)
    {
        front = new node<T>;        // 头结点只负责结构，不存元素
        front->next = NULL;
        rear = front;
        for (node<T>* p = a.front->next; p != NULL; p = p->next)
        {
            node<T>* s = new node<T>(p->data);
            rear->next = s;
            rear = s;
        }
    }
    node<T>* getfront()
    {
        return front;
    }
    node<T>* getrear()
    {
        return rear;
    }
    virtual ~linkqueue()
    {
        node<T>* p;
        while (front != NULL)
        {
            p = front;
            front = front->next;
            delete p;
        }
    }
    bool isempty()
    {
        return front == rear;
    }
    bool push(T x)                          // 入队，返回 true 表示入队成功
    {
        node<T>* s = new node<T>;
        s->data = x;
        s->next = NULL;
        rear->next = s;
        rear = s;
        return true;
    }
    bool pop(T& t)                          // 出队
    {
        if (isempty())
        {
            std::cerr << "队空，不能出队！" << std::endl;
            std::exit(1);
        }
        node<T>* p = front->next;
        T x = p->data;
        front->next = p->next;
        if (rear == p)
        {
            rear = front;
        }
        delete p;
        t = x;
        return true;
    }
    T gethead()                             // 取队头元素
    {
        if (isempty())
        {
            std::cerr << "队空，没有队头元素！" << std::endl;
            std::exit(1);
        }
        return front->next->data;
    }
};