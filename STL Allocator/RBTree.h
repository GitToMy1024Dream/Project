#pragma once

#include "Alloc.h"
#include "IteratorTraits.h"
#include "TypeTraits.h"

//节点颜色
//红色为0，黑色为1
typedef bool RBTreeColorType;
const RBTreeColorType RED = false;
const RBTreeColorType BLACK = true;

template<class K,class V>
struct RBTreeNode
{	
	typedef RBTreeColorType ColorType; // 颜色类型
	typedef RBTreeNode* base_ptr;

	base_ptr _pLeft;   //指向左节点
	base_ptr _pRight;  //指向右结点
	base_ptr _pParent; //指向父节点
	base_ptr color;

	//最小值
	static base_ptr minimum(base_ptr x)
	{
		while (x->_pLeft != NULL) //一直往做走，就会找到最小值
			x = x->_pLeft;        //这是二叉搜索树的特性
		return x;       
	}

	//最大值
	static base_ptr maximum(base_ptr x)
	{
		while (x->_pRight != NULL)  //一直往右走，就会直到最大值
			x = x->_pRight;         //这是二叉搜索树的特性
		return x;
	}
};

//红黑树的迭代器
template<class K,class V,class Ref,class Ptr>
class RBTreeIterator
{
public:
	typedef RBTreeIterator<K, V, Ref, Ptr> Self;
	typedef RBTreeNode<K, V> Node;

public:
	RBTreeIterator()
		:_pNode(NULL)
	{}

	RBTreeIterator(Node* pNode)
		:_pNode(pNode)
	{}

	RBTreeIterator(const Self& it)
		:_pNode(it._pNode)
	{}

	Self& operator++()
	{
		_Increment();
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		_Increment();
		return temp;
	}
	
	Self& operator--()
	{
		_Decrement();
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		_Decrement();
		return temp;
	}

	Ref operator*()
	{
		return _pNode->_key;
	}

	Ptr operator->()
	{
		return &(_pNode->_key);
	}

	bool operator!= (const Self& s)
	{
		return _pNode != s._pNode;
	}

	bool operator==(const Self& s)
	{
		return _pNode == s._pNode;
	}

private:
	/* 求下一个节点(比当前节点大的最小结点) */
	void _Increment()
	{
		//若右子树存在，则一定是右子树最左边的结点
		if (_pNode->_pRight != NULL)
		{
			_pNode = _pNode->_pRight;
			while (_pNode->_pLeft != NULL)
				_pNode = _pNode->_pLeft;
		}
		//右子树不存在
		else
		{
			Node* pParent = _pNode->_pParent;
		
			//若pNode就是右结点，则一直上溯，直到不为右结点为止
			//此时父节点一定是下一个节点
			while (_pNode == pParent->_pRight)
			{
				_pNode = pParent;
				pParent = pParent->_pParent;
			}
			//防止根节点与头结点的特殊情况出现
			if (_pNode->_pRight != pParent)
			{
				_pNode = pParent;
			}
		}
	}

	/* 求上一个节点(比当前节点小的最大结点) */
	void _Decrement()
	{
		//若当前节点为红色，且父节点的父节点等于自己，则一定是右子节点
		//这属于一种特殊请求，即pNode为头结点
		if (_pNode->_color == RED && _pNode->_pParent->_pParent == _pNode)
			_pNode = _pNode->_pRight;

		//若左结点存在，在左子树中一直往最右边走，即为上一个节点
		else if (_pNode->_pLeft)
		{
			_pNode = _pNode->_pLeft;
			while (_pNode->_pRight)
				_pNode = _pNode->_pRight;
		}

		//非根节点，如果pNode为左节点，一直交替往上走，直到
		//pNode不为左节点，此时父节点一定是上一个节点
		else
		{
			Node* pParent = _pNode->_pParent;
			while (_pNode == pParent->_pLeft)
			{
				_pNode = pParent;
				pParent = pParent->_pParent;
			}
			_pNode = pParent;
		}
	}

private:
	Node* _pNode;
};


//RBTree的数据结构
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc = alloc>
class RBTree
{
protected:
	typedef void* voidPointer;  //空指针
	typedef RBTreeNode* basePtr; //基层节点的指针
	typedef SimpleAlloc<RBTreeNode, alloc> RBTreeNodeAllocator;//节点的空间配置器
	typedef RBTreeColorType ColorType; //节点颜色
public:
	typedef Key KeyType;					//键
	typedef Value ValueType;			    //值
	typedef ValueType* Pointer;				//值指针
	typedef const ValueType* ConstPointer;  //const值指针
	typedef ValueType& Reference;			//值引用
	typedef const ValueType& ConstReference;//const值引用
	typedef RBTreeNode* LinkType;           //节点指针
	typedef size_t SizeType;                //size_t正整数类型

protected:
	SizeType NodeCount;       //红黑树的结点数目 
	LinkType Header;		  //头结点，与根节点互为双亲
	Compare KeyCompare;		  //比较键值大小的函数

public:
	typedef RBTreeIterator<ValueType, Reference, Pointer> Iterator; //定义红黑树的迭代器
protected:

	/* 调用空间配置器申请一个节点 */
	LinkType GetNode()
	{
		return RBTreeNodeAllocator::Allocate();
	}
	/* 调用空间配置器释还一个节点 */
	void PutNode(LinkType p)
	{
		RBTreeNodeAllocator::Deallocate(p);
	}
	/* 申请并初始化节点 */
	LinkType CreateNode(const ValueType& x)
	{
		LinkType tmp = GetNode();   //申请一个节点
		Construct(&tmp->_value, x); //调用构造函数初始化节点
		return tmp;
	}

	/* 克隆节点 */
	LinkType CloneNode(LinkType x)
	{
		LinkType tmp = CreateNode(x->_value); //申请并初始化节点
		tmp->color = x->color;
		tmp->_pLeft = 0;
		tmp->_pRight = 0;
		return tmp;
	}

	/* 释还节点 */
	void DestroyNode(LinkType p)
	{
		Destroy(&p->_value);  //调用析构函数销毁节点
		PutNode(p);           //释放内存
	}

protected:
	//以下列三个函数方便取得header的成员

	/* 获得根节点 */
	LinkType& root()const
	{
		return (LinkType&)Header->_pParent;
	}

	/* 获得整棵树最左边的结点 */
	LinkType& leftmost()const
	{
		return (LinkType)Header->_pLeft;
	}

	/* 获得整棵树最右边的结点 */
	LinkType& rightmost()const
	{
		return (LinkType)Header->_pRight;
	}

	//以下六个函数用来方便取得节点x的成员

	/* 返回节点的左子节点 */
	static LinkType& left(LinkType x)
	{
		return (LinkType&)(x->_pLeft);
	}

	/* 返回节点的右右子节点 */
	static LinkType& right(LinkType x)
	{
		return (LinkType&)(x->_pRight);
	}

	/* 返回节点的父节点 */
	static LinkType& parent(LinkType x)
	{
		return (LinkType&)(x->_pParent);
	}

	/* 返回节点的值 */
	static Reference value(LinkType x)
	{
		return x->value;
	}

	/* 返回节点的颜色 */
	static ColorType& color(LinkType x)
	{
		return (ColorType)(x->color);
	}

	/* 返回最小值节点 */
	static LinkType minimum(LinkType x)
	{
		return (LinkType)RBTreeNode::minimum(x);
	}

	/* 返回最大值节点 */
	static LinkType maximum(LinkType x)
	{
		return (LinkType)RBTreeNode::maximum(x);
	}
private:
	/* 初始化红黑树 */
	void Init()
	{
		Header = GetNode(); //初始化Header节点，header节点与root节点互为父节点
		color(Header) = RED; //设置Header为红色节点
		root() = 0;   //root()获得红黑树的根节点
		leftmost() = Header; //设置header节点的左子树指向自己
		rightmost() = Header; //设置header节点的右子树指向自己
	}
public:
	/* 构造函数 */
	RBTree(const Compare& comp = Compare())
		:NodeCount(0)
		, KeyCompare(comp)
	{
		Init();
	}

	/* 析构函数 */
	~RBTree()
	{
	    //Clear();
		PutNode(Header);
	}

public:
	/* 获取根节点的值,与header互为父节点 */
	ValueType rootValue()
	{
		return Value((LinkType)Header->_pParent);
	}

	/* 返回比较大小的函数 */
	Compare keyComp()const
	{
		return keyComp;
	}

	/* 返回一个迭代器，指向红黑树最左边的结点 */
	Iterator begin()
	{
		return leftmost;
	}

	/* 返回一个迭代器，指向红黑树最右边的结点 */
	Iterator end() 
	{
		return Header;
	}

	/* 判断红黑树是否为空 */
	bool empty()const
	{
		return NodeCount == 0;
	}

	/* 判断红黑树大小 */
	SizeType size()const
	{
		return NodeCount;
	}

	/* 红黑树的最大结点数 */
	SizeType maxSize()const
	{
		return SizeType(-1);
	}

public:
	//红黑树的基本操作

	/* 插入新值：节点键值不允许重复，若重复则插入无效
	   返回值是pair，第一个元素是RBTree的迭代器，指向
	   新增节点，第二个元素表示插入成功与否 */
	std::pair<Iterator,bool> InsertUnique(const Value& v)
	{
		LinkType y = Header; //令y是头结点
		LinkType x = root(); //x是红黑树的根节点，默认初始值为null
		bool Comp = true; //比较大小的布尔值
		//x!=null，说明我们找到了插入新节点的位置
		while (x != 0)
		{
			y = x; //y保存着x节点的父节点
			
			Comp = KeyCompare(KeyOfValue()(v), Key(x));
			//如果Comp为true，说明待插入值小于节点x的值，我们沿左走，令x为x的左子树
			//反之，说明待插入值大于节点x的值，我们沿右走，令x为x的右子树
			x = Comp ? left(x) : right(x);
		}
		//此时y指向插入点的父节点,必为叶子结点
		Iterator j = Iterator(y); 

		//如果插入的值比父节点的值小，插入在左子节点
		if (Comp)
		{
			//begin()调用leftmost()，获得整棵树最左边的结点
			if (begin() == j)
			{
				//x是插入点，y是插入点的父节点，v是插入的值
				return std::pair<Iterator, bool>(_Insert(x, y, v), true);
			}
			else
			{
				j--; //调整j
			}
		}
		//新值不与既有结点值重复，可以插入
		if (keyComp(Key(j.node), KeyOfValue()(v)))
		{
			return std::pair<Iterator, bool>(Insert(x, y, v), true);
		}
		//如果到这一步，说明新值一定与树中键值重复，不能插入
		return std::pair<Iterator, bool>(j, false);
	}

	/* 插入新值，结点键值允许重复
	   返回红黑树的迭代器，该迭代器指向新节点 */
	Iterator InsertEqual(const ValueType& v)
	{
		LinkType y = Header;
		LinkType x = root();//从根节点开始
		while (x != 0) //从根节点向下寻找合适的插入点
		{
			y = x;
			//遇大往右，遇小往左
			x = keyComp(KeyOfValue()(v), Key(x)) ? left(x) :right(x);
		}
		return Insert(x, y, v);//x为插入点，y为x的父节点，v为插入的键值
	}

	/* 寻找红黑树中是否有键值为k的结点 */
	Iterator Find(const ValueType& x)
	{
		LinkType y = Header; //令y为头结点
		LinkType x = root(); //令x为根节点
		while (x != 0)
		{
			//KeyCompare()是比较大小的函数
			if (!KeyCompare(Key(x), k))//x > k
			{
				y = x;
				x = left(x); //遇到大值向左走
			}
			else  //x < k
			{
				x = right(x); //遇到小值向右走
			}
		}
		Iterator j = Iterator(y);
		return (j == end() || KeyCompare(k, Key(j.node))) ? end() : j;
	}
	
private:
	//真正的插入执行程序

	/*  _x:待插入结点
		_y:插入节点的父节点
		_v:插入节点的键值 */
	Iterator Insert(basePtr _x, basePtr _y, const ValueType& _v)
	{
		LinkType x = (LinkType)_x; //x指向插入点
		LinkType y = (LinkType)_y; //y指向插入点的父节点
		LinkType z;
		
		/* 1.y == Header：插入点的父节点为header（注意Header是哨兵，
		     header不属于树的一部分，但是header的parent指向根节点）。
			 y == Header说明插入点为根节点  
		   2.x == 0：说明插入点在叶子节点下方（叶子节点的左子树和
		     右子树均为null），也就是在叶子节点下挂新的节点；x != 0
			 说明插入点在树的内部某个节点上  
		   3.KeyCompare(KeyOfValue()(v), Key(y))：带插入节点的值
		     要比父节点的值小（意味着我们要插入到父节点的左子树上） */

		if (y == Header || x != 0 || KeyCompare(KeyOfValue()(v), Key(y)))
		{
			z = CreateNode(v); //创建新节点
			left(y) = z; //令父节点的左子树为z
			
			// y == Header:插入点的父节点为header，说明根节点还没有初始化
			if (y == Header)
			{
				root() = z; //z成为了根节点
				rightmost() = z; //令z是整棵树最右边的结点
			}
			//如果父节点是整棵树最左边的结点
			else if (y == leftmost)
			{
				leftmost() = z;//则将新节点作为整棵树左左边的结点
			}
		}
		else
		{
			z = CreateNode(v); //创建一个新节点
			right(y) = z; //插入节点到父节点的右边
			
			//如果父节点是整棵树最右边的结点
			if (y == rightmost)
			{
				rightmost() = z; //将新节点作为树中最右边的结点
			}
		}

		parent(z) = y; //令新节点的父节点为y
		left(z) = 0;   //令新节点的左右节点为null
		right(z) = 0;

		RBTreeRebalance(z, Header->parent); //调整树结构满足红黑树
		++NodeCount; //增加节点的个数
		return Iterator(z);
	}


	/* 重新平衡红黑树(改变颜色及旋转树形) 
	   x: 新增节点
	   y: 叔叔节点
	   root: 根节点 */
	inline void RBTreeRebalance(RBTreeNode* x, RBTreeNode*& root)
	{
		x->color = RED; //新插入的结点是红色的

		//新插入的结点不是根节点并且父节点为红色
		while (x != root && x->parent->color == RED)
		{
			//父节点为祖父节点的左子节点
			if (x->parent == x->parent->parent->left)
			{
				RBTreeNode* y = x->parent->parent->right; //令y为叔叔节点
				//1.叔叔节点存在且为红色
				if (y && y->color == RED)
				{
					x->parent->color = BLACK; //更改父节点为黑色
					y->color = bool;          //更改叔叔节点为黑色
					x->parent->parent->color = RED; //更改祖父节点为红色
					x = x->parent->parent;
				}
				//2.无叔叔节点，或者叔叔节点为黑色
				else
				{
					//3.若新节点为父节点的右子节点
					if (x == x->parent->right)
					{
						x = x->parent;
						// 整理一下从while开始的条件判断分支，我们可以得出做左旋转的条件：  
						// 1.新增节点不是根节点  
						// 2.新增节点的父节点是红色  
						// 3.父节点是祖父节点的左子节点  
						// 4.伯父节点不存在，或者伯父节点为黑  
						// 5.新增节点为父节点的右子节点 
						RBTreeRotateLeft(x, root);  //以新插入节点为轴左旋转
					}

					x->parent->color = BLACK; //修改父节点的颜色为黑色
					x->parent->parent->color = RED; //修改祖父节点的颜色为红色
					RBTreeRotateRight(x->parent->parent, root); //以新插入节点的祖父节点为轴右旋转
				}
			}
			//父节点为祖父节点的右子节点
			else
			{
				RBTreeNode* y = x->parent->parent = > left; //令y为叔叔节点
				//4.叔叔存在且为红色
				if (y && y->color == RED)
				{
					x->parent->color = BLACK;      //更改父节点的颜色为黑色
					y->color = BLACK;              //更改叔叔节点的颜色为黑色 
					x->parent->parent->color = RED;//更改祖父节点的颜色为红色
					x = x->parent->parent; //继续向上检查
				}
				//5.无叔叔节点，或叔叔节点为黑色
				else
				{
					//6.如果新节点为父节点的左子节点
					if (x == x->parent->left)
					{
						x = x->parent;

						// 整理一下从while开始的条件判断分支，我们可以得出做右旋转的条件：  
						// 1.新增节点不是根节点  
						// 2.新增节点的父节点是红色  
						// 3.父节点是祖父节点的右子节点  
						// 4.伯父节点不存在，或者伯父节点为黑  
						// 5.新增节点为父节点的左子节点  

						RBTreeRotateRight(x, root); //以新插入节点为轴右旋转
					} 
					x->parent->color = BLACK;       //将新插入节点的父节点调整为黑色
					x->parent->parent->color = RED; //将新插入节点的祖父节点调整为黑色
					RBTreeRotateLeft(x->parent->parent, root);//以新插入节点的祖父节点为轴进行左旋转
				}
			}
		}
		root->color = BLACK; //根节点永远为黑色
	}

	/* 左旋转
	   新节点必为红色，如果插入处的父节点为红色，就
	   违反红黑树的规则，此时需要坐旋转 */

	inline void RBTreeRotateLeft(RBTreeNode* x, RBTreeNode*& root)
	{
		RBTreeNode* y = x->right; // 令y为旋转点的右子节点
		x->right = y->left;  //父子对换位置
		if (y->left != 0)
		{
			y->left->parent = x; //设定父节点
			y->parent = x->parent;
		}
		//让y替代x的作用
		if (x == root)
			root = y;  //令y为根节点
		else if (x == x->parent->left)  //x为其父节点的左子节点
			x->parent->left = y;        //令旋转点的父节点的左子节点为y
		else 
			x->parent->right = y;       //令旋转点的父节点的右子节点为y
		y->left = x;                    //令右子节点的左子树为x
		x->parent = y;                  //令右子节点为旋转点的父节点
	}

	/* 右旋转
	   新节点必为红节点，如果插入处父节点为红色，则违反
	   了红黑树的规则，此时必须进行右旋转 */

	inline void RBTreeRotateRight(RBTreeNode* x, RBTreeNode*& root)
	{
		//x为旋转点
		RBTreeNode* y = x->left; //y为旋转点的左子节点
		x->left = y->right;
		if (y->right != 0)
			y->right->parent = x;  //设定父节点
		y->parent = x->parent;

		//令y完全取代x的地位(必须将x对其父节点的关系完全接受过来)
		if (x == root)      //x为根节点
			root = y;       //让y取代根节点
		else if (x == x->parent->right)   //x为其父节点的右子节点
			x->parent->right = y;
		else                              //x为其父节点的左子节点
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}

};

