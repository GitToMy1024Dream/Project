#pragma once

#include "Alloc.h"
#include "RBTree.h"
#include "Construct.h"
#include "IteratorTraits.h"

template<class Key,class Compare = less<Key>,class Alloc = alloc>
class multiset
{
public:
	//multiset一定是要insert_equal()，而不使用insert_unique()

	template <class inputiterator>
	multiset(InputIterator first, InputIterator last)
		: t(Compare()) 
	{
		t.InsertEqual(first, last);
	}
	template <class inputiterator>
	multiset(InputIterator first, InputIterator last, const Compare& comp)
		: t(comp) 
	{
		t.InsertEqual(first, last);
	}

	// 插入元素, 注意, 插入的元素key允许重复
	Iterator Insert(const value_type& x)
	{
		return t.InsertEqual(x);
	}

	// 在position处插入元素, 但是position仅仅是个提示, 如果给出的位置不能进行插入,
	// STL会进行查找, 这会导致很差的效率
	Iterator Insert(Iterator position, const ValueType& x)
	{
		typedef typename repType::Iterator rep_iterator;
		return t.InsertEqual((rep_iterator&)position, x);
	}

	//其余与set相同
};