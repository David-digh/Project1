#pragma once

#define MAXSIZE 1000005
#include<iostream>
#include <cstdlib>
template <typename T>
class circuqueue
{
private:
	T data[MAXSIZE];
	int front;
	int rear;
public:
	circuqueue() :front(0), rear(0)
	{
	};
	bool push(T x)
	{
		if ((rear + 1) % MAXSIZE == front)
		{
			return false;
		}
		data[rear] = x;
		rear = (rear + 1) % MAXSIZE;
		return true;
	}
	bool isempty()
	{
		if (rear == front)return true;
		return false;
	}
	bool pop(T &e)
	{
		if (isempty())
		{
			return false;
		}
		e = data[front];
		front = (front + 1) % MAXSIZE;
		return true;
	}
	T gethead()
	{
		if (isempty())
		{
			std::cerr << "队空，没有队头元素！" << std::endl;
			std::exit(1);
		}
		return data[front];
	}
};