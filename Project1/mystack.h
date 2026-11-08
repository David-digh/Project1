#pragma once
#include <iostream>
#include <cstdlib>


template <class T>
class mystack
{
private:

    static const int STACKSIZE = 1000005;
    T data[STACKSIZE];
    int top;
public:
    mystack()
    {
        top = -1;
    }
    bool isempty()
    {
        return top == -1;
    }
    bool push(T x)                          // 入栈，返回 true 表示成功
    {
        if (top == STACKSIZE - 1)
        {
            std::cerr << "栈满，不能入栈！" << std::endl;
            std::exit(1);
        }
        data[++top] = x;
        return true;
    }
    bool pop(T& t)
    {
        if (isempty())
        {
            std::cerr << "栈空，不能出栈！" << std::endl;
            std::exit(1);
            return false;
        }
        t = data[top--];
        return true;
    }
    T gethead()
    {
        if (isempty())
        {
            std::cerr << "栈空，没有栈顶元素！" << std::endl;
            std::exit(1);
        }
        return data[top];
    }
    
};
