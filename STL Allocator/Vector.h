#pragma once

#include "Alloc.h"
#include "Construct.h"
#include <string>

//vector的迭代器操作
#if 0
template<class T,class Ptr,class Ref>
class VectorIterator
{
public:
	typedef T ValueType;
	typedef VectorIterator<T, T*, T&> Self;
public:
	VectorIterator(T* p = NULL)    //构造函数
		:_p(p)
	{}

	VectorIterator(const T& v)     //拷贝构造
		:_p(v._p)
	{}

	Ref operator*()                //重载*运算符
	{
		return *_p;
	}
	 
	Ptr operator->()               //重载->运算符
	{
		return &(operator*());
	}

	Self& operator++()      //前置++
	{
		_p++;
		return *this;
	}

	Self operator++(int)    //后置++
	{
		Self tmp(*this);
		_p++;
		return tmp;
	}

	Self& operator--()      //前置--
	{
		_p--;
		return *this;
	}

	Self operator--(int)    //后置--
	{
		Self tmp(*this);
		_p--;
		return tmp;
	}

	bool operator!=(const Self& s)
	{
		return _p != s._p;
	}

	bool operator==(const Self& s)
	{
		return _p == s._p;
	}

private:
	T* _p;
};

#endif

template<class T,class Alloc = alloc>
class Vector
{
public:
	typedef T ValueType;
	typedef T* Iterator;
	typedef T& Ref;
	typedef const T* ConstIterator;

	typedef SimpleAlloc<T, Alloc> dataAlloc; 
public:
	Vector()   //空构造函数
		:_start(0)
		,_finish(0)
		,_endOfStorage(0)
	{}

	//构造一个vector，包含n个值为value的元素
	Vector(size_t n, const T& value = T())
	{
		unInitFill(n, value);
	}

	//尾插
	void PushBack(const T& value)
	{
		_CheckCapacity();
		*_finish = value;
		++_finish;
	}

	//尾删
	void PopBack()
	{
		--_finish;
		Destroy(_finish);
	}

	//清除[first,last]中的所有元素
	Iterator Erase(Iterator first, Iterator last)
	{
		Iterator index = copy(last, _finish, first);
		Destroy(index, _finish);
		_finish = _finish - (last - first);
		return first;
	}


	//清空
	void Clear()
	{
		Erase(begin(), end());
	}

	//析构函数
	~Vector()
	{
		Destroy(_start, _endOfStorage);
		dataAlloc::Deallocate(&*_start, Capacity());
	}
	//返回第一个元素的地址
	Iterator begin()
	{
		return _start;
	}
	//返回最后一个元素的地址
	Iterator end()
	{
		return _finish;
	}

	//vector中有效元素的个数
	size_t Size()const
	{
		return _finish - _start;
	}

	//vector所能容纳的元素个数
	size_t Capacity()const
	{
		return _endOfStorage - _start;
	}

	//判断vector是否为空
	bool Empty()const
	{
		return _start == _finish;
	}

	//重载[]
	Ref operator[](size_t n)
	{
		return *(begin() + n);
	}

	//返回首元素
	Ref front()
	{
		return *begin();
	}
	//返回尾元素
	Ref back()
	{
		return *(end() - 1);
	}

protected:
	//检查是否需要扩容
	void _CheckCapacity()
	{
		if (_finish == _endOfStorage)
		{
			size_t capacity = Capacity();
			size_t newSize = 2 * capacity + 3;

			T* tmp = dataAlloc::Allocate(newSize);
			for (size_t idx = 0; idx < newSize; ++idx)
				Construct(tmp + idx, T());

			//内置类型->memcpy
			//自定义类型-> for=
			//拷贝元素
			for (size_t idx = 0; idx < Size(); ++idx)
				tmp[idx] = _start[idx];

			//释放旧空间
			//vector<int>  
			//vector<string>
			if (_start)
			{
				Destroy(_start, _finish);
				dataAlloc::Deallocate(_start, capacity);
			}
			//指向新空间,更新地址
			_start = tmp;
			_finish = _start + capacity;
			_endOfStorage = _start + newSize;
		}
	}

	//填充并初始化
	void unInitFill(size_t n, const T& value)
	{
		_start = dataAlloc::Allocate(n);
		for (size_t idx = 0; idx < n; ++idx)
		{
			Construct(_start + idx, value);
		}
	}

private:
	Iterator _start;//目前使用空间的头
	Iterator _finish;//目前使用空间的尾
	Iterator _endOfStorage;//目前可用空间的尾
};


void Test4()
{
	Vector<int> v;
	v.PushBack(10);
	v.PushBack(20);
	v.PushBack(30);
	v.PushBack(40);
	cout << "Distance:" << distance(v.begin(), v.end()) << endl;

	Vector<int>::Iterator it = v.begin();
	while (it != v.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}

void Test5()
{
	Vector<string> v;
	v.PushBack("apple");
	v.PushBack("noodles");
	v.PushBack("flower");
	v.PushBack("hello world");
	cout << "Distance:" << distance(v.begin(), v.end()) << endl;
	cout << "begin:" << v.begin() << endl;
	cout << "end:" << v.end() << endl;
	cout << "front:" << v.front() << endl;
	cout << "back:" << v.back() << endl;
	cout << "empty:" << v.Empty() << endl;
	cout << "Capacity:" << v.Capacity() << endl;
	cout << "operator[]:"<<v.operator[](3) << endl;
	Vector<string>::Iterator it = v.begin();
	while (it != v.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}