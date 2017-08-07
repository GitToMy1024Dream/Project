#pragma once

struct InputIteratorTag
{};
struct OutputIteratorTag
{};
struct ForwardIteratorTag :public InputIteratorTag
{};
struct BidirectionalIteratorTag :public ForwardIteratorTag
{};
struct RandomAccessIteratorTag :public BidirectionalIteratorTag
{};

//迭代器的萃取是为了弥补T*不符合迭代器的标准
//内嵌定义的5种类型

template<class Iterator>
struct IteratorTraits
{
	typename typedef Iterator::IteratorCategory IteratorCategory;
	typename typedef Iterator::ValueType ValueType;
	typename typedef Iterator::Pointer   Pointer;
	typename typedef Iterator::Reference Reference;
	typename typedef Iterator::DifferenceType   DiffenceType;
};

//原生态的指针
template<class T>
struct IteratorTraits<T*>
{
	typename RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef T* Pointer;
	typedef int DiffenceType;
};

//原生态的指针
template<class T>
struct IteratorTraits<const T*>
{
	typedef RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef const T* Poniter;
	typedef const T& Reference;
	typedef int DiffenceType;
};

//随机迭代器
template<class Iterator>
typename IteratorTraits<Iterator>::DiffenceType _Distance(Iterator first, Iterator last, RandomAccessIteratorTag)
{
	return last - first;
}

template<class Iterator>
typename IteratorTraits<Iterator>::DiffenceType _Distance(Iterator first, Iterator last, InputIteratorTag)
{
	IteratorTraits<Iterator>::DiffenceType count = 0;
	while (first != last)
	{
		++count;
		++first;
	}
	return count;
}

template<class Iterator>
typename IteratorTraits<Iterator>::DiffenceType Distance(Iterator first, Iterator last)
{
	return _Distance(first, last, IteratorTraits<Iterator>::IteratorCategory());
}

//反向迭代器
template<class Iterator>
class RerveseIterator
{
public:
	//typename typedef IteratorTraits<Iterator>::IteratorCategory IteratorCategory;
	typename typedef IteratorTraits<Iterator>::ValueType        ValueType;
	typename typedef IteratorTraits<Iterator>::Pointer          Pointer;
	typename typedef IteratorTraits<Iterator>::Reference        Reference;
	typename typedef IteratorTraits<Iterator>::DiffenceType     DiffenceType;
	
	typedef RerveseIterator<Iterator> Self;
public:
	RerveseIterator()
		:_cur()
	{}
	
	RerveseIterator(Iterator cur)
		:_cur(cur)
	{}

	RerveseIterator(const Self& s)
		:_cur(s._cur)
	{}

	//前置--
	Self& operator++()
	{
		--_cur;
		return *this;
	}

	//后置--
	Self operator++(int)
	{
		Self tmp(*this);
		--_cur;
		return tmp;
	}

	//前置++
	Self& operator--()
	{
		++_cur;
		return *this;
	}

	//后置++
	Self operator--(int)
	{
		Self tmp(*this);
		++_cur;
		return tmp;
	}

	Reference operator*()
	{
		Iterator tmp(_cur);
		return *(--tmp);
	}

	Pointer operator->()
	{
		return &(*_cur);
	}

	bool operator!=(const Self& s)
	{
		return _cur != s._cur;
	}

	bool operator==(const Self& s)
	{
		return _cur == s._cur;
	}

private:
	Iterator _cur;
};