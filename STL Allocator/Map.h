#pragma once

#include "Alloc.h"
#include "Construct.h"
#include "IteratorTraits.h"
#include "RBTree.h"


//pair键值对的定义
template<class T1,class T2>
struct pair
{
	typedef T1 firstType;  
	typedef T2 secondType; 

	T1 first;     //key
	T2 second;	  //value

	pair()
		:first(T1())
		,second(T2())
	{}

	pair(const T1& a,const T2& b)
		:first(a)
		,second(b)
	{}

};

//map的数据结构
template<class Key,class T,class Compare = less<key>,class Alloc = alloc >
class Map
{
public:
	typedef Key KeyType;  //键值类型
	typedef T DataType;   //实值类型
	typedef T MapType;
	typedef pair<const Key, T> ValueType;//元素型别
	typedef Compare KeyCompare; //键值比较函数

	//元素比较函数
	class ValueCompare
		:public binary_function<ValueType, ValueType, bool>
	{
		friend class Map<Key, T, Compare, Alloc>; //将Map定义为ValueCompare的友元
	protected:
		Compare comp;
		ValueCompare(Compare c): comp(c)
		{}
	public:
		bool operator()(const ValueType& x, const ValueType& y)const //仿函数，重载()
		{
			return comp(x.first, y.first);
		}
	};

	//map的底层采用红黑树作为底层容器
	typedef RBTree<KeyType, ValueType, KeyCompare, Alloc> repType;
	repType t;   //t为内部RBTree的容器

public:
	//用于提供iterator_traits<i>支持
	typedef typename repType::Pointer Pointer;
	typedef typename repType::ConstPointer ConstPointer;
	typedef typename repType::reference reference;
	typedef typename repType::ConstReference ConstReference;
	typedef typename repType::Iterator Iterator;
	//map并不像set那样将iterator设置为const 类型，因为它允许通过其迭代器修改元素的value
	typedef typename repType::ConstIterator ConstIterator;
	
	//反向迭代器
	typedef typename repType::ReverseIterator ReverseIterator;
	typename typename repType::ConstReverseIterator ConstReverseIterator;

	typedef typename repType::SizeType SizeType;
	typedef typename repType::DifferenceType DifferenceType;


	//常规的迭代器操作，下列方法全部调用红黑树的方法，不予解释
	Iterator begin() 
	{
		return t.begin(); 
	}
	ConstIterator begin() const 
	{
		return t.begin(); 
	}
	
	Iterator end() 
	{
		return t.end();
	}
	ConstIterator end() const 
	{ 
		return t.end();
	}
	ReverseIterator rbegin() 
	{
		return t.rbegin();
	}
	ConstReverseIterator rbegin() const
	{
		return t.rbegin(); 
	}
	ReverseIterator rend() 
	{
		return t.rend();
	}
	ConstReverseIterator rend() const
	{
		return t.rend(); 
	}
	bool empty() const
	{
		return t.empty();
	}
	SizeType size() const
	{
		return t.size(); 
	}
	SizeType MaxSize() const 
	{
		return t.MaxSize(); 
	}

	T& operator[](const KeyType& k)
	{
		return (*((Insert(ValueType(k, T()))).first)).second;
	}

	// 返回用于key比较的函数
	KeyCompare KeyComp() const
	{
		return t.KeyComp();
	}

	// 由于map的性质, value和key使用同一个比较函数, 实际上我们并不使用value比较函数
	ValueCompare ValueComp() const
	{
		return ValueCompare(t.KeyComp());
	}

	// 注意: 这里有一个常见的陷阱, 如果访问的key不存在, 会新建立一个
	T& operator[](const KeyType& k)
	{
		return (*((Insert(ValueType(k, T()))).first)).second;
	}

	//空构造函数，直接调用红黑树的空构造函数
	Map()
		:t(Compare)
	{}

	//拷贝构造函数
	explicit Map(const Compare& comp)
		:t(comp)
	{}

	template<class InputIterator>
	Map(InputIterator first, InputIterator last)
		: t(Compare())
	{
		t.InsertUnique(first, last);
	}

	template<class InputIterator>
	Map(InputIterator first, InputIterator last, const Compare& comp)
		: t(comp)
	{
		t.InsertUnique(first, last);
	}

	Map(const Map<Key,T,Compare,Alloc>& x)
		:t(x.t)
	{}

private:
	// 对于相同的key, 只允许出现一次, 用bool标识
	pair<Iterator, bool> Insert(const ValueType& x) 
	{ 
		return t.InsertUnique(x);
	}

	// 在position处插入元素, 但是position仅仅是个提示, 如果给出的位置不能进行插入,
	// STL会进行查找, 这会导致很差的效率
	iterator Insert(Iterator position, const ValueType& x)
	{
		return t.InsertUnique(position, x);
	}
	// 将[first，last)区间内的元素插入到map中
	template <class Inputiterator>
	void Insert(InputIterator first, InputIterator last)
	{
		t.InsertUnique(first, last);
	}
	
	// 擦除指定位置的元素, 会导致内部的红黑树重新排列
	void erase(Iterator position) 
	{
		t.erase(position);
	}

	// 会返回擦除元素的个数, 其实就是标识map内原来是否有指定的元素
	size_type erase(const KeyType& x)
	{
		return t.erase(x); 
	}
	void erase(Iterator first, Iterator last)
	{
		t.erase(first, last);
	}

	//清空整个map容器
	void clear()
	{
		t.clear();
	}

	//查找键值为key的元素
	Iterator find(const KeyType& x)
	{
		return t.Find();
	}
	ConstIterator find(const KeyType& x)const
	{
		return t.Find(x);
	}
	//返回指定元素的个数
	SizeType count(const KeyType& x)const
	{
		return t.NodeCount(x);
	}

public:
	//重载比较运算符==，并声明为友元
	friend bool operator== __STL_NULL_TMPL_ARGS(const Map&, const Map&);
	
	//重载比较运算符<，并声明为友元
	friend bool operator< __STL_NULL_TMPL_ARGS(const Map&, const Map&);

	inline bool operator==(const Map<Key, T, Compare, Alloc>& y)
	{
		return x.t == y.t;
	}

	inline bool operator<(const Map<Key, T, Compare, Alloc>& y,const Map<Key,T,Compare,Alloc>)
	{
		return x.t < y.t;
	}

};