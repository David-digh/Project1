#pragma once
#include<iostream>
#include<stack>
#include <cstdlib>
template<class T>
class increstack
{
	std::stack<T> sta;
public:
	increstack()
	{
	}
	virtual ~increstack()
	{
		while (!sta.empty())
		{
			sta.pop();
		}
	}
	bool isempty()
	{
		return sta.empty();
	}
	bool push(T a)
	{
		if (this->isempty())sta.push(a);
		else
		{
			std::stack<T> linshi;
			while (!sta.empty() && a < sta.top())
			{
				linshi.push(sta.top());
				sta.pop();
			}
			sta.push(a);
			while (!linshi.empty())
			{
				sta.push(linshi.top());
				linshi.pop();
			}
		}
		return true;
	}
	bool pop(T& a)
	{
		if (this->isempty())return false;
		a = sta.top();
		sta.pop();
		return true;
	}
	T gethead()
	{
		if (isempty())
		{
			std::cerr << "Õ»¿Õ£¬Ã»ÓÐÔªËØ£¡" << std::endl;
			std::exit(1);
		}
		return sta.top();
	}
	//virtual bool operator<(increstack<T>& b) = 0;
};
