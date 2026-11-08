#pragma once
#include<iostream>
#include<deque>
#include <cstdlib>
template<class T>
class increqueue
{
	std::deque<T> que;
public:
	increqueue()
	{
	}
	virtual ~increqueue()
	{
	}
	bool isempty()
	{
		return que.empty();
	}
	void push_back(T a)
	{
		if (que.empty())
		{
			que.push_back(a);
			return;
		}
		std::deque<T> linshi;
		while (!que.empty() && que.back() > a)
		{
			linshi.push_front(que.back());
			que.pop_back();
		}
		que.push_back(a);
		while (!linshi.empty())
		{
			que.push_back(linshi.front());
			linshi.pop_front();
		}

	}
	void push_front(T a)
	{
		if (que.empty())
		{
			que.push_back(a);
			return;
		}
		std::deque<T> linshi;
		while (!que.empty() && que.front() < a)
		{
			linshi.push_back(que.front());
			que.pop_front();
		}
		que.push_front(a);
		while (!linshi.empty())
		{
			que.push_front(linshi.back());
			linshi.pop_back();
		}

	}
	bool pop_back(T& a)
	{
		if (this->isempty())return false;
		a = que.back();
		que.pop_back();
		return true;
	}
	bool pop_front(T& a)
	{
		if (this->isempty())return false;
		a = que.front();
		que.pop_front();
		return true;
	}
	T qback()
	{
		if (isempty())
		{
			std::cerr << "队空，没有元素！" << std::endl;
			std::exit(1);
		}
		return que.back();
	}
	T qfront()
	{
		if (isempty())
		{
			std::cerr << "队空，没有元素！" << std::endl;
			std::exit(1);
		}
		return que.front();
	}
	//virtual bool operator<(increqueue<T>& b) = 0;
	//virtual bool operator>(increqueue<T>& b) = 0;
};
