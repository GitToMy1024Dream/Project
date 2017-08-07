#pragma once
#include <iostream>
#include <stdarg.h>
#include <string.h>
using namespace std;

#pragma warning (disable:4996)

#define __DEBUG__
static string GetFileName(const string& path)
{
	char ch = '/';

#ifdef _WIN32
	ch = '\\';
#endif
	size_t pos = path.rfind(ch);
	if (pos == string::npos)
		return path;
	else
		return path.substr(pos + 1);
}

//用于调试追溯的trace_log
inline static void __trace_debug(const char* function, \
	const char* filename, \
	int line, char* format, ...)
{
#ifdef __DEBUG__
	FILE* fout = fopen("trace.log", "a+");
	//输入调用函数的信息
	fprintf(stdout, "[%s:%d]%s", GetFileName(filename).c_str(), line, function);
	
	//输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	fclose(fout);
#endif 
}
#define __TRACE_DEBUG(...) __trace_debug(__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__);

typedef void(*HandlerFunc)(); //将void(*)()重命名成HandlerFunc

//一级空间配置器
template<int inst>   //预留参数
class _MallocAllocTemplate
{
public:
	//分配size字节的空间
	static void* Allocate(size_t size)
	{
		__TRACE_DEBUG("调用一级空间配置器申请[%ubytes]\n", size);

		void *ret = malloc(size); //一级配置器直接使用malloc()开辟空间
		//若malloc失败，则调用OOM_Malloc
		if (ret == 0)
		{
			ret = OOM_Malloc(size);
		}
		return ret;
	}

	//malloc失败时调用的处理函数
	static void* OOM_Malloc(size_t size)
	{
		//一直尝试释放、配置，直到malloc成功
		for (;;)
		{
			//未设置内存不足处理机制，则抛出异常
			if(_handler == NULL)   
				throw bad_alloc();
			_handler();  //调用处理历程，企图释放内存
	
			void* ret = malloc(size);
			//开辟成功，返回这块内存的地址
			if (ret)
				return ret;
		}
	}

	//直接调用free释放空间
	static void Deallocate(void* p, size_t n)
	{
		__TRACE_DEBUG("一级空间配置器释放内存[%p]\n", p);
		free(p);
	}

	//通过以下这个函数设置文件句柄，间接实现set_new_hanlder
	//参数与返回值均是函数指针
	static HandlerFunc SetMallocHandler(HandlerFunc f)
	{
		HandlerFunc old = f;
		_handler = f;   //将内存分配失败的句柄设置为f，让操作系统去释放其他的空间
		return old;
	}

private:
	static HandlerFunc _handler;//函数指针，用于内存不足时的处理机制
};
template<int inst>
HandlerFunc _MallocAllocTemplate<inst>::_handler = 0; //初始化，默认不设置内存处理机制


//二级空间配置器
template<bool threads,int inst>
class _DefaultAllocTemplate
{
private:
	enum { __ALIGN = 8 }; //限定最大对齐树
	enum { __MAX_BYTES = 128 }; //最大字节数
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; //自由链表的个数

	//自由链表(赋值管理分配出去的内存块)
	union Obj
	{
		union Obj* _freeListLink;//指向下一个自由链表节点的指针
		char client_data[1]; // The client sees this.
	};

	static Obj* _freeList[__NFREELISTS];

	//狭义内存池
	static char* _startFree; //内存池的起始地址
	static char* _endFree;   //内存池的末尾地址
	static size_t _heapSize; //已向系统申请的内存的大小

public:
	//bytes字节在自由链表中的对应位置
	static size_t FREELIST_INDEX(size_t bytes)
	{
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}

	//对bytes字节向上取8的倍数
	static size_t ROUND_UP(size_t bytes)
	{
		return (bytes + __ALIGN - 1)&(~(__ALIGN - 1));
	}

		//从内存池中取空间填充自由链表,每个对象大小为size,申请nobjs个
		static char* ChunkAlloc(size_t size, size_t& nobjs)
		{
			__TRACE_DEBUG("到内存池中期望取得[%u]个[%ubytes]对象\n", nobjs, size);

			size_t totalBytes = size*nobjs; //总共请求的内存大小
			size_t leftBytes = _endFree - _startFree;//内存池剩余空间大小

			//1.有足够nobjs个对象
			if (leftBytes >= totalBytes)
			{
				__TRACE_DEBUG("内存池拥有足够[%u]个对象的内存\n", nobjs);
				char *ret = _startFree;
				_startFree += totalBytes;//更新内存池的起始地址
				return ret;
			}

			//2.只够分配[1,size)个nobjs对象
			else if (leftBytes > size)
			{
				nobjs = leftBytes / size; //至多分配的对象个数
				__TRACE_DEBUG("内存池只有[%u]个对象的内存\n", nobjs);
				totalBytes = size*nobjs;
				char *ret = _startFree;
				_startFree += totalBytes; //更新内存池的起始位置
				return ret;
			}
		
			//3.一个对象都不能分配
			else
			{
				__TRACE_DEBUG("内存池连一个[%ubytes]的对象都没有\n", size);

				//处理残余的内存，挂在自由链表的对应位置
				if (leftBytes > 0)
				{
					size_t index = FREELIST_INDEX(leftBytes);
					((Obj*)_startFree)->_freeListLink = _freeList[index];//头插法
					_freeList[index] = (Obj*)_startFree;
				}

				size_t newBytes = 2 * totalBytes + ROUND_UP(_heapSize >> 4);
				__TRACE_DEBUG("向系统申请[%ubytes]给内存池\n", newBytes);
				_startFree = (char*)malloc(newBytes);//开辟新空间
				if (_startFree == NULL) //开辟失败
				{
					__TRACE_DEBUG("到系统申请内存失败\n");

					//内存已经吃紧，需要到更大的自由链表去取
					size_t i = FREELIST_INDEX(size);
					//在自由链表中找到一个更大的内存块
					for (size_t i = 0; i < __NFREELISTS; ++i)
					{
						if (_freeList[i])
						{
							//重新调整free_list
							Obj* ret = _freeList[i];
							_freeList[i] = ret->_freeListLink;
							_startFree = (char*)ret;
							_endFree = _startFree + (i + 1)*__ALIGN;
							return ChunkAlloc(size, nobjs); //开辟成功后，再次调用ChunkAllc分配内存
						}
					}

					//free_list没可用内存块时，调用一级配置器
					_startFree = (char*)_MallocAllocTemplate<0>::Allocate(newBytes);

				}

				//向系统申请内存成功
				//重新调整_heapSize与_endFree
				_heapSize += newBytes;
				_endFree  = _startFree + newBytes;
				return ChunkAlloc(size, nobjs);//递归调用ChunkAlloc，为了修正nobjs

			}
		}

	//为自由链表重新填充空间
	static char* Refill(size_t n)
	{
		size_t nobjs = 20;  //缺省取得20个新区块
		char* chunk = ChunkAlloc(n, nobjs);//调用ChunkAlloc进行内存分配

		__TRACE_DEBUG("到内存池中申请[%u]个的对象,\
一个返回用户使用，剩下挂在自由链表下面\n", nobjs);
		
		//只申请到一个内存块时，直接返回给用户使用，free_list无新节点
		if (nobjs == 1)
			return chunk;

		//将剩余的nobjs-1块内存挂在自由链表的对应位置下面
		size_t index = FREELIST_INDEX(n);
		_freeList[index] = (Obj*)(chunk + n);//将第二个对象的地址放在free_list中
		Obj* cur = _freeList[index];

		//将剩下的块依次挂在自由链表下面
		for (size_t i = 2; i < nobjs; ++i)
		{
			Obj* next = (Obj*)((char*)cur + n);
			cur->_freeListLink = next;
			cur = next;
		}
		cur->_freeListLink = NULL;//free_list的最后一个内存块指向NULL

		return chunk;//返回第一个内存块
	}

	//开辟nbytes的空间
	static void* Allocate(size_t n)
	{
		__TRACE_DEBUG("调用二级空间配置器申请[%ubytes]\n", n);
		
		//大于128字节调用一级空间配置器
		if (n > (size_t)__MAX_BYTES)
		{
			return _MallocAllocTemplate<0>::Allocate(n);
		}

		//寻找16个free_list中适当的一个
		size_t index = FREELIST_INDEX(n);
		//free_list中存在对应的内存块时，将第一块返回给用户
		if (_freeList[index])
		{
			__TRACE_DEBUG("在自由链表第[%d]个位置取一个内存块\n", index);
			Obj* ret = _freeList[index];
			_freeList[index] = ret->_freeListLink;//指向第二个内存块
			return ret;
		}
		else
		{
			return Refill(ROUND_UP(n)); //调用Refill()从内存池中申请空间来填充free_list
		}

	}


	//释放空间
	static void Deallocate(void *ptr, size_t n)
	{
		//大于128字节调用一级空间配置器释放
		if (n > __MAX_BYTES)
		{
			_MallocAllocTemplate<0>::Deallocate(ptr, n);
		}
		else
		{
			//将释放的内存块挂接在自由链表的对应位置
			size_t index = FREELIST_INDEX(n);

			__TRACE_DEBUG("将释放的[%ubytes]的对象挂在free_list[%u]下\n", n, index);
			((Obj*)ptr)->_freeListLink = _freeList[index];//头插法
			_freeList[index] = (Obj*)ptr; //更新freelist的指向
		}
	}

};


//初始化操作
template<bool threads,int inst>
typename _DefaultAllocTemplate<threads,inst>::Obj*
_DefaultAllocTemplate<threads, inst>::_freeList[__NFREELISTS] = { NULL };

template <bool threads, int inst>
char* _DefaultAllocTemplate<threads, inst>::_startFree = NULL;

template <bool threads, int inst>
char* _DefaultAllocTemplate<threads, inst>::_endFree = NULL;

template <bool threads, int inst>
size_t _DefaultAllocTemplate<threads, inst>::_heapSize = 0;

#ifdef __USE_MALLOC
typedef _MallocAllocTemplate<0> alloc;
#else
typedef _DefaultAllocTemplate<true, 0> alloc;
#endif 


//配置器标准接口
template<class T,class Alloc>
class SimpleAlloc
{
public:
	static T* Allocate(size_t n)
	{
		return n == 0 ? 0 : (T*)Alloc::Allocate(n * sizeof(T));
	}
	
	static T* Allocate(void)
	{
		return (T*)Alloc::Allocate(sizeof(T));
	}

	static void Deallocate(T* p, size_t n)
	{
		if (n != 0)
		{
			Alloc::Deallocate(p, n * sizeof(T));
		}
	}

	static void Deallocate(T* p)
	{
		Alloc::Deallocate(p, sizeof(T));
	}
};








