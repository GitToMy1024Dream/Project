#pragma once
#include <iostream>
#include "heap.h"
using namespace std;
template<class T>
struct HuffmanTreeNode
{
	T _weight;
	HuffmanTreeNode<T>* _pLeft;
	HuffmanTreeNode<T>* _pRight;
	HuffmanTreeNode<T>* _pParent;
	HuffmanTreeNode(const T& weight)
		:_weight(weight)
		, _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
	{}
};
template<class T/*,class Compare = Less<T>*/>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_pRoot(NULL)
	{}
	//构造函数
	HuffmanTree(T _array[], size_t size, const T& invaild)
	{
		_Create(_array, size, invaild);//构造堆
	}
	Node* Root()
	{
		return _pRoot;
	}
	//析构函数
	~HuffmanTree()
	{
		Destroy(_pRoot);//销毁堆
	}
protected:
	void _Create(T *_array, size_t size, const T& invalid)
	{
		struct Compare
		{
			bool operator()(const Node* left, const Node* right)
			{
				return left->_weight < right->_weight;//最小堆
			}
		};
		Myheap<Node*,Compare> hp;
		for (size_t idx = 0; idx < size; ++idx)
			{
			//hp.PushHeap(new Node(_array[idx]));
				if (_array[idx] != invalid)
				{
					Node* node = new Node(_array[idx]);
					hp.PushHeap(node);
				}
			}
		while (hp.Size() > 1)
		{
			Node* pLeft = hp.Top();
			hp.PopHeap();
			Node* pRight = hp.Top();
			hp.PopHeap();
			Node* parent = new Node(pLeft->_weight + pRight->_weight);
			parent->_pLeft = pLeft;
			parent->_pRight = pRight;
			pLeft->_pParent = parent;
			pRight->_pParent = parent;
			hp.PushHeap(parent);
		}
		_pRoot = hp.Top();
	}
	void Destroy(Node* root)
	{
		if (root)
		{
			Destroy(root->_pLeft);
			Destroy(root->_pRight);
			delete root;
			root = NULL;
		}
	}
protected:
	Node* _pRoot;
};


