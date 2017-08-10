#pragma once

template<class Key,class Compare = less<Key>,class Alloc = alloc>
class multimap
{
public:
	// ... 其他地方与map相同
	// 注意下面这些函数都调用的是insert_equal，而不是insert_unique
	template <class Inputiterator>
	multimap(InputIterator first, InputIterator last)
		: t(Compare()) {
		t.InsertEqual(first, last);
	}

	template <class inputiterator>
	multimap(InputIterator first, InputIterator last, const Compare& comp)
		: t(comp) {
		t.InsertEqual(first, last);
	}

	// 插入元素, 注意, 插入的元素key允许重复
	Iterator Insert(const ValueType& x)
	{
		return t.InsertEqual(x); 
	}

	// 在position处插入元素, 但是position仅仅是个提示, 如果给出的位置不能进行插入,
	// STL会进行查找, 这会导致很差的效率
	Iterator Insert(Iterator position, const ValueType& x)
	{
		return t.InsertEqual(position, x);
	}
	// 插入一个区间内的元素
	template <class inputiterator>
	void Insert(InputIterator first, InputIterator last)
	{
		t.InsertEqual(first, last);
	}
	// 其余与map相同
};
