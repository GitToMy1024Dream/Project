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
int main()
{
	int _array1[] = { 3,2,7,8,1,9,0,11,23,4 };
	char _array2[] = { 'c','w','y','m','d','s','q','o','a','b' };
	BubbleSort<int, Greate<int>>(_array1,sizeof(_array1)/sizeof(_array1[0]));//升序int
	BubbleSort<int, Less<int>>(_array1, sizeof(_array1) / sizeof(_array1[0]));//降序int
	BubbleSort<char, Greate<char>>(_array2, sizeof(_array2) / sizeof(_array2[0]));//升序char
	BubbleSort<char, Less<char>>(_array2, sizeof(_array2) / sizeof(_array2[0]));//降序char
	system("pause");
	return 0;
}
