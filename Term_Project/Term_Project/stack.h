#pragma once
#include <typeinfo>
#include <iostream>
#define MAX_SIZE (10000)
using namespace std;

template <typename T>

class Stack {
private:
	T data[MAX_SIZE];
	int _top;

public:
	Stack() {
		_top = -1;
	}
	~Stack() {

	}
	void push(T inputdata) {
		data[_top + 1] = inputdata;
		cout << typeid(data[_top + 1]).name();
		_top += 1;
	}
	T pop() {
		T res = data[_top];
		_top -= 1;
		return res;
	}
	int size() {
		return _top + 1;
	}
	bool empty() {
		return size() == 0 ? true : false;
	}
	T top() {
		T res = data[_top];
		return res;
	}
};