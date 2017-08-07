#include "Alloc.h"
#include "Vector.h"
#include "List.h"

void Test1()
{
	_DefaultAllocTemplate<false, 0>::Allocate(200);
	void* p1 = _DefaultAllocTemplate<false, 0>::Allocate(5);
	void* p2 = _DefaultAllocTemplate<false, 0>::Allocate(5);
	_DefaultAllocTemplate<false, 0>::Deallocate(p1, 5);
	_DefaultAllocTemplate<false, 0>::Deallocate(p2, 5);

	for (size_t i = 0; i < 21; ++i)
	{
		void* p1 = _DefaultAllocTemplate<false, 0>::Allocate(5);
	}
}

void Test2()
{
	cout << "²âÊÔÏµÍ³¶ÑÄÚ´æºÄ¾¡" << endl;
	_DefaultAllocTemplate<false, 0>::Allocate(1024 * 1024 * 1024);
	_DefaultAllocTemplate<false, 0>::Allocate(1024 * 1024 * 256);
	_DefaultAllocTemplate<false, 0>::Allocate(1024 * 1024);

	for (int i = 0; i < 100000; i++)
	{
		void *p1 = _DefaultAllocTemplate<false, 0>::Allocate(128);
	}
}
void Test3()
{
	void *p1 = _DefaultAllocTemplate<false, 0>::Allocate(6);
	for (int i = 0; i < 18; i++)
	{
		void *p2 = _DefaultAllocTemplate<false, 0>::Allocate(8);
	}
	void *p3 = _DefaultAllocTemplate<false, 0>::Allocate(64);

}

int main()
{
	Test1();
	//Test2();
	//Test3();
	//Test4();
	//Test5();
	//Test6();
	//Test7();
	Test8();
	system("pause");
	return 0;
}


