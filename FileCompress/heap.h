#pragma once
#include <iostream>
#include <vector>

using namespace std;
template<class T>
class Greate//大堆
{
public:
	bool operator()(const T& left, const T& right)//仿函数，重载()大于
	{
		return left > right;
	}
};
template<class T>
class Less//小堆
{
public:
	bool operator()(const T& left, const T& right)//仿函数，重载()小于
	{
		return left < right;
	}
};
template<class T,class Compare = Less<T>>//第二个模板参数默认为大堆
class Myheap
{
public:
	//空堆
	Myheap()
	{}
	Myheap(const T arr[], size_t size)
	{
		assert(arr);
		_heap.resize(size);//开辟空间(比vector的pushback好)
		for (size_t idx = 0; idx < size; ++idx)
		{
			_heap[idx] = arr[idx];
		}
		//size-1为最后一个节点的下标，(size-1-1)/2 = 父节点
		int Root = (_heap.size() - 2) >> 1;
		for (;Root >= 0;--Root)
		{
			AdjustDown(Root);
		}
	}
	//判空
	bool Empty()const
	{
		return _heap.empty();
	}
	//堆的大小
	size_t Size()const
	{
		return _heap.size();
	}
	//堆顶元素
	T& Top()
	{
		assert(_heap.size() > 0);
		return _heap[0];
	}
	const T& Top()const
	{
		assert(_heap.size() > 0);
		return _heap[0];
	}
	void PushHeap(const T& elem)//向堆中插入元素
	{
		//先将新节点插入末尾
		_heap.push_back(elem);
		//在将最后一个元素向上调整
		AdjustUp(_heap.size() - 1);
	}
	void PopHeap()//从堆中取出堆顶的元素
	{
		assert(!Empty());//堆不为空
		//先交换堆顶节点与堆尾节点
		std::swap(_heap[0], _heap[_heap.size() - 1]);
		//删除最后一个节点
		_heap.pop_back();
		//从堆顶向下调整,存在多个节点时才调整
		if (_heap.size() > 1)
		{
			AdjustDown(0);
		}
	}
	void AdjustDown(size_t _root)//下滤
	{
		size_t Parent = _root;//此时传入的_root为倒数第一个非叶子结点
		size_t Child = Parent * 2 + 1;//左孩子的下标等于父节点*2+1
		Compare _Compare;//仿函数用于比较优先级
		//左孩子存在
		while (Child < _heap.size())
		{
			//右孩子存在且右孩子的值大于左孩子
			if (Child + 1 < _heap.size() && _Compare(_heap[Child+1],_heap[Child]))
			{
				Child = Child + 1;//让Child结点保存大值
			}
			//孩子结点的优先级大于父节点(Child的值大于Parent的值)
			if (_Compare(_heap[Child], _heap[Parent]))
			{
				std::swap(_heap[Child], _heap[Parent]);//交换两节点的值
				//下滤
				Parent = Child;//让父节点指向孩子节点
				Child = Parent * 2 + 1;//孩子节点指向孙子节点
			}
			//孩子结点的优先级小于父节点(Child的值小于Parent的值)，则不需要调整
			else
				return;
		}
	}
	void AdjustUp(size_t Child)//上滤
	{
		//此时传入的Child为最后一个节点的坐标
		size_t Parent = (Child - 1) >> 1;
		Compare _Compare;//仿函数用于比较优先级
		//上滤的过程，只要没到根节点就一直上移
		while (Child > 0 )
		{
			//孩子节点的优先级大于父节点的优先级
			if (_Compare(_heap[Child], _heap[Parent]))
			{
				std::swap(_heap[Child], _heap[Parent]);//交换节点的值
				//上滤
				Child = Parent;//孩子节点指向父节点
				Parent = (Child - 1) >> 1;//父节点指向爷爷节点
			}
			//孩子节点的优先级小于父节点的优先级，不需要调整
			else
				return;
		}
	}
private:
	std::vector<T> _heap;
};
//优先级队列(调用堆的方法即可)
//template<class T,class Compare = Greate<T>>
//class PriorityQueue
//{
//public:
//	PriorityQueue()
//	{}
//	void Push(const T& data)
//	{
//		_hp.PushHeap(data);
//	}
//	void Pop()
//	{
//		_hp.PopHeap();
//	}
//	bool Empty()const
//	{
//		return _hp.Empty();
//	}
//	const T& Top()const
//	{
//		return _hp.Top();
//	}
//	size_t Size()
//	{
//		return _hp.Size();
//	}
//protected:
//	Myheap<T, Greate<T>> _hp;
//};


