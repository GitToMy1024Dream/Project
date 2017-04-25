#include <iostream>
using namespace std;
template<class T>
class Greate
{
public:
	bool operator()(const T& left, const T& right)//仿函数，重载()
	{
		return left > right;
	}
};
template<class T>
class Less
{
public:
	bool operator()(const T& left, const T& right)//仿函数，重载()
	{
		return left < right;
	}
};
template<class T,class Compare>
void BubbleSort(T *_array, size_t _size)
{
	for (size_t idx = 0; idx < _size - 1; idx++)
	{
		bool iState = false;//初始化标志位
		for (size_t jdx = 0; jdx < _size - idx - 1; jdx++)
		{
			//调用Compare(),判断排序是升还是降
			if (Compare()(_array[jdx], _array[jdx + 1]))
			{
				iState = true;//交换时改变标志位的状态
				std::swap(_array[jdx], _array[jdx + 1]);
			}
		}
		if (!iState)//若标志位为false，说明未发生交换
			break;
	}
}
