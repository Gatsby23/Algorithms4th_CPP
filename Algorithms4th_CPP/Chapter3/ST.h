#pragma once
//����listʵ�ֹ��̣�����������Ҫ�����ڴ������
//�ڶ��������ṹ��������Ҫʵ�ֵ�����
#include<memory>
#include<algorithm>
//1. treenode�ķ�����
template<typename Key, typename Value>
struct TreeNode
{
	std::pair<Key, Value> item;
	TreeNode* left;
	TreeNode* right;
	TreeNode* parent; //���������ڵ�����
	int size;
private:
	static std::allocator<TreeNode> alloc;
public:
	//create Node�ִ�����ֵ����ֵ
	static TreeNode* createNode()
	{
		auto p = alloc.allocate(1);
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
		alloc.construct(&(p->parent), nullptr);
		return p;
	}
	TreeNode* createNode(const Key &key, const Value &val, int n=1)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&((p->item).first), key);//ע��һ����ȡ��ַ
		alloc.construct(&((p->item).second), val);
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
		alloc.construct(&(p->parent), nullptr);
		alloc.construct(&(p->size), n);

		return p;
	}
	TreeNode* createNode(Key &&key, Value &&val, int n=1)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&((p->item).first), std::move(key));//ע��һ����ȡ��ַ
		alloc.construct(&((p->item).second), std::move(val));
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
		alloc.construct(&(p->parent), nullptr);
		alloc.construct(&(p->size), n);
		return p;
	}
	TreeNode* cloneNode()
	{
		auto ret = createNode((this->item).first, (this->item).second, this->size);
		ret->left = this->left;
		ret->right = this->right;
		return ret;
	}
	void deleteNode()
	{
		alloc.destroy(&(this->item));
		alloc.deallocate(this, 1);
	}
};
template<typename Key, typename Value> std::allocator<TreeNode<Key,Value>> TreeNode<Key,Value>::alloc;

template<typename Key, typename Value>
struct RedBlackTreeNode 
{
	static const bool RED = true;
	static const bool BLACK = false;
	bool color;

	std::pair<Key, Value> item;
	RedBlackTreeNode* left;
	RedBlackTreeNode* right;
	RedBlackTreeNode* parent; //���������ڵ�����
	int size;
private:
	static std::allocator<RedBlackTreeNode> alloc;
public:
	//create Node�ִ�����ֵ����ֵ
	static RedBlackTreeNode* createNode()
	{
		auto p = alloc.allocate(1);
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
		alloc.construct(&(p->parent), nullptr);
		return p;
	}

	RedBlackTreeNode* createNode(const Key &key, const Value &val, const int n = 1, bool col = RED)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&((p->item).first), key);//ע��һ����ȡ��ַ
		alloc.construct(&((p->item).second), val);
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
		alloc.construct(&(p->parent), nullptr);
		alloc.construct(&(p->size), n);
		p->color = col;
		return p;
	}
	RedBlackTreeNode* createNode(Key &&key, Value &&val, const int n = 1, bool col = RED)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&((p->item).first), std::move(key));//ע��һ����ȡ��ַ
		alloc.construct(&((p->item).second), std::move(val));
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
		alloc.construct(&(p->parent), nullptr);
		alloc.construct(&(p->size), n);
		p->color = col;
		return p;
	}

	RedBlackTreeNode* cloneNode()
	{
		auto ret = createNode((this->item).first, (this->item).second, this->size, this->color);
		ret->left = this->left;
		ret->right = this->right;
		return ret;
	}

	void deleteNode()
	{
		alloc.destroy(&(this->item));
		alloc.deallocate(this, 1);
	}
};

template<typename Key, typename Value> std::allocator<RedBlackTreeNode<Key, Value>> RedBlackTreeNode<Key, Value>::alloc;

//2.���ṹ�ĵ�������Ϊ���ܹ���һ���ڵ㿪ʼ�������������parent������׷��
template<typename TreeNode>
struct TreeIterator
{

	TreeNode* node;
	typedef typename std::remove_reference<decltype(node->item)>::type value_type;
	TreeIterator(TreeNode* s) :node(s) {}
	bool operator==(const TreeIterator &rhs) const { return node == rhs.node; }
	bool operator!=(const TreeIterator &rhs) const { return !(*this == rhs); }

	value_type& operator*() const { return node->item; }
	value_type* operator->() const { return &(node->item); }
	TreeIterator& operator++() { increment(); return *this; }
	TreeIterator& operator++(int) { TreeIterator ret = *this; increment(); return ret; }
	TreeIterator& operator--() { decrement(); return *this; }
	TreeIterator& operator--(int) { TreeIterator ret = *this; decrement(); return ret; }
private:
	void increment()
	{
		if (node->right != nullptr) //���node����������
		{
			node = node->right;
			while (node->left != 0)
				node = node->left;

		}
		else {
			auto parent = node->parent;//�ҳ����ڵ�
			while (node == parent->right)//�����ǰnode�Ǹ��ڵ�����ӽڵ�
			{
				node = parent;
				parent = node->parent;
			}//����������Ϊ���ӽڵ㡱Ϊֹ��
			if (node->right != parent)
				node = parent;
			//���node->right == parent����ʱ���������������������Ѱ�Ҹ��ڵ����һ���ڵ㣬��ǡ�ɸ��ڵ�û�����ӽڵ�
			//����μ���STLԴ��������P216
		}
	}

	void decrement()
	{
		if (node->left != nullptr)//��������������������������ֵ
		{
			auto y = node->left;
			while (y->right != nullptr)
				y = y->right;
			node = y;
		}
		else {//���û��������
			auto y = node->parent;
			while (node == y->left)
			{
				node = y;
				y = node->parent;
			}
			if (node->left != y)
				node = y;
		}
	}
};


template<typename Key, typename Value, typename Node = TreeNode<Key, Value>>
class BST
{
	typedef typename TreeIterator<Node> Iterator;
protected:
	Node* dummy;//������singlelist�е�dummy
	
//�������ƺ���
public:
	BST();
	BST(const BST& rhs);
	BST& operator=(const BST& rhs);
	BST(BST &&rhs) noexcept;
	BST& operator=(BST&& rhs) noexcept;
	~BST();
protected:
	void init();
	void destructor(Node *x);
	Node* constructor(Node* x);

//�ӿں���
public:
	bool isEmpty() { return root() == nullptr; }
	bool contains(const Key &key) { return get(key) != end(); }
	//ע��ݹ��㷨��ݹ�Ľӿ�
	Iterator get(const Key &key) { return get(root(), key); }
	/**
	*ע�����ּ���
	*���ص����Բ���Ϊ�������������ĸ���㣬������������ȫ�֣�����Ҫ���游�ڵ�
	*�Ӷ����븸�ڵ��йصĴ���ת�޵�������
	*/
	void put(const Key &key, const Value &val) 
	{
		auto r = put(root(), key, val);
		r->parent = dummy;
		dummy->parent = r;
		dummy->left = min(r);
		dummy->right = max(r);
	}
	virtual void deleteMin() 
	{ 
		auto r = deleteMin(root()); 
		r->parent = dummy;
		dummy->parent = r;
		dummy->left = min(r);
		dummy->right = max(r);
	}
	virtual void deleteMax() 
	{ 
		auto r = deleteMax(root()); 
		r->parent = dummy;
		dummy->parent = r;
		dummy->left = min(r);
		dummy->right = max(r);
	}
	virtual void deleteKey(const Key &key) 
	{ 
		auto r = deleteKey(root(), key); 
		r->parent = dummy;
		dummy->parent = r;
		dummy->left = min(r);
		dummy->right = max(r);
	}
	Iterator min() { return min(root()); }
	Iterator max() { return max(root()); }
	Iterator floor(const Key &key)
	{
		Node* x = floor(root(), key);
		if (x == nullptr) return end();
		return x;
	}
	Iterator ceiling(const Key &key)
	{
		Node *x = ceiling(root(), key);
		if (x == nullptr) return end();
		return x;
	}
	int size() { return size(root()); }
	Iterator select(int k) 
	{ 
		Node *x = select(root(), k);
		if (x == nullptr) return end();
		return x;
	}
	int rank(const Key &key) { return rank(root(), key); }

//�ӿں����ĸ�������
protected:
	Node*& root() const { return dummy->parent; }
public:
	Iterator begin() { return min(); }
	Iterator end() { return dummy; }

//�ӿں����ĸ�������2
protected:
	//size�����������ӵ�ֵ����0����ô 
	//size(x)=size(x->left)+size(x->right)+1 ���ǳ���
	int size(Node* x)
	{
		if (x == nullptr) return 0;
		else return x->size;
	}

	Node* select(Node *x, int k)
	{
		if (x == nullptr) return nullptr;
		int t = size(x->left);
		if (t > k) return select(x->left, k);
		else if (t < k) return select(x->right, k - t - 1);
		else return x;
	}
	int rank(Node *x, const Key &key)
	{
		if (x == nullptr) return 0;
		if (key < x->item.first) return rank(x->left, key);
		else if (x->item.first < key) return 1 + size(x->left) + rank(x->right, key);
		else return size(x->left);
	}

	Node* floor(Node* x, const Key &key)
	{
		if (x == nullptr) return nullptr;
		if (key < x->item.first) return floor(x->left, key);
		else if (x->item.first == key) return x;
		//���key���ڸ����ļ�ֵ����ôf
		//���������д���С��key�Ľ�㣬��ôfloor����������
		//����floor���Ǹ����
		Node *t = floor(x->right, key);
		if (t != nullptr) return t;
		else return x;
	}
	Node* ceiling(Node *x, Key key)
	{
		if (x == nullptr) return nullptr;
		if (x->item.first == key) return x;
		else if (x->item.first < key) return ceiling(x->right, key);

		Node *t = ceiling(x->left, key);
		if (t == nullptr) return x;
		else return t;
	}

protected://
	Node* get(Node* x, const Key &key)
	{
		if (x == nullptr) return dummy;
		if (key < x->item.first) return get(x->left, key);
		else if (x->item.first < key) return get(x->right, key);
		else return x;
	}

	virtual Node* put(Node* x, const Key &key, const Value &val)
	{//
		if (x == nullptr) return dummy->createNode(key, val, 1);
		if (key < x->item.first)
		{
			x->left = put(x->left, key, val);
			x->left->parent = x;
		}
		else if (x->item.first < key) {
			x->right = put(x->right, key, val);
			x->right->parent = x;
		}
		else x->item.second = val;
		x->size = size(x->left) + size(x->right) + 1;
		return x;
	}

	Node* min(Node* x)
	{
		if (x == nullptr) return x;
		if (x->left == nullptr) return x;
		else return min(x->left);
	}
	Node* max(Node* x)
	{
		if (x == nullptr) return x;
		if (x->right == nullptr) return x;
		else return max(x->right);
	}

	virtual Node* deleteMin(Node *x)
	{
		if (x == nullptr) return nullptr;
		if (x->left == nullptr)
		{
			Node* ret = x->right;
			x->deleteNode();
			return ret;//ɾ��x
		}

		x->left = deleteMin(x->left);
		x->left->parent = x;
		x->size = size(x->left) + size(x->right) + 1;
		return x;
	}

	virtual Node* deleteMax(Node *x)
	{
		if (x == nullptr) return nullptr;
		if (x->right == nullptr)
		{
			Node* ret = x->left;
			x->deleteNode();
			return ret;//ɾ��x
		}
		x->right = deleteMax(x->right);
		x->right->parent = x;
		x->size = size(x->left) + size(x->right) + 1;
		return x;
	}

	virtual Node* deleteKey(Node *x, const Key &key)
	{
		if (x == nullptr) return nullptr;

		if (key < x->item.first)
		{
			x->left = deleteKey(x->left, key);
			x->left->parent = x;
		}
		else if (x->item.first < key) {
			x->right = deleteKey(x->right, key);
			x->right->parent = x;
		}
		else
		{//ɾ��x��������������ֻ��һ���ӽ�㣬��ôֱ�����ӽ�����x
		 //���ӵ�������ӽ�㣬��ô��x�ĺ��������x��x�ĺ����z�����������ұ�Ȼֻ��С�ڵ���һ���ӽ��
			if (x->right == nullptr)
			{
				Node *ret = x->left;
				x->deleteNode();
				return ret;//ɾ��x
			}
			if (x->left == nullptr)
			{
				Node *ret = x->right;
				x->deleteNode();
				return ret;
			}

			Node *t = x;
			x = min(t->right)->cloneNode();
			x->right = deleteMin(t->right);
			x->right->parent = x;
			x->left = t->left;
			x->left->parent = x;
			t->deleteNode();
		}
		x->size = size(x->left) + size(x->right) + 1;
		return x;
	}
};



//--------------------------------�������ƺ���-------------------------------------------------------//
template<typename Key, typename Value, typename Node> 
void BST<Key, Value, Node>::destructor(Node *x)
{
	if (x == nullptr) return;
	destructor(x->left);
	destructor(x->right);
	x->deleteNode();
}

template<typename Key, typename Value, typename Node>
Node* BST<Key, Value, Node>::constructor(Node* x)
{
	if (x == nullptr) return nullptr;

	auto left = constructor(x->left);
	auto right = constructor(x->right);
	auto ret = x->cloneNode();
	left->parent = ret;
	right->parent = ret;
	ret->left = left;
	ret->right = right;
	return ret;
}

template<typename Key, typename Value, typename Node>
void BST<Key, Value, Node>::init()
{	
	dummy = Node::createNode();
	dummy->parent = nullptr; //root
	dummy->left = dummy; //left most
	dummy->right = dummy; // right most
}

template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>::BST()
{
	init();
}
template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>::BST(const BST& rhs)
{
	
	init();
	if (rhs.size() > 0)
	{
		auto x = constructor(rhs.root()); //x is root
		x->parent = dummy;
		dummy->left = min(x);
		dummy->right = max(x);
		dummy->parent = x;
	}
}

template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>& BST<Key, Value, Node>::operator=(const BST& rhs)
{
	if (*this != rhs)
	{
		BST tmp(rhs);
		*this = std::move(tmp);
		return *this;
	}

}
template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>::BST(BST &&rhs) noexcept
{
	dummy = rhs.dummy;
	rhs.init();
}

template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>& BST<Key, Value, Node>::operator=(BST&& rhs) noexcept
{
	if (this != &rhs)
	{
		destructor(root());
		dummy = rhs.dummy;
		rhs.init();
	}
	return *this;
}
template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>::~BST()
{
	destructor(root());
}

//RedBlackTree




//����������������Ĳ�ͬ�����ڣ�put��delete����
template<typename Key, typename Value, typename Node = RedBlackTreeNode<Key, Value>>
class RedBlackBST :public BST<Key, Value, Node>
{
public:
	//��Ҫ����
	void put(const Key &key, const Value &val)
	{
		BST<Key, Value, Node>::put(key, val);
		root()->color = BLACK;
	}
	bool is23Tree() const { return is23Tree(root()); }



private:
	static const bool RED = true;
	static const bool BLACK = false;
	bool isRed(Node *x) const
	{
		if (x == nullptr) return false;//������Ϊ��
		return x->color == RED;
	}
	Node* rotateLeft(Node *h);
	Node* rotateRight(Node *h);
	void flipColors(Node *h);

private:
	virtual Node* put(Node* h, const Key &key, const Value &val)
	{
		if (h == nullptr) return dummy->createNode(key, val, 1, RED);

		if (key < h->item.first)
		{
			h->left = put(h->left, key, val);
			h->left->parent = h;
		}
		else if (h->item.first < key) {
			h->right = put(h->right, key, val);
			h->right->parent = h;
		}
		else h->item.second = val;

		if (isRed(h->right) && !isRed(h->left)) h = rotateLeft(h);
		if (isRed(h->left) && isRed(h->left->left))h = rotateRight(h);
		if (isRed(h->left) && isRed(h->right)) flipColors(h);

		h->size = size(h->left) + size(h->right) + 1;
		return h;
	}

	bool is23Tree(Node *h) const
	{
		if (h == nullptr) return true;
		if (isRed(h->right)) return false;
		if (h != root() &&isRed(h) && isRed(h->left))
			return false;
		return is23Tree(h->left) && is23Tree(h->right);
	}

};

//���������������:
template<typename Key, typename Value, typename Node>
Node* RedBlackBST<Key,Value,Node>::rotateLeft(Node *h)
{
	_ASSERT(isRed(h->right));

	Node *x = h->right;
	h->right = x->left; 
	if (h->right != nullptr) h->right->parent = h;
	x->left = h; x->left->parent = x;
	x->color = h->color;
	h->color = RED;
	x->size = h->size;
	h->size = 1 + size(h->left) + size(h->right);

	return x;
}

template<typename Key, typename Value, typename Node>
Node* RedBlackBST<Key, Value, Node>::rotateRight(Node *h)
{
	_ASSERT(isRed(h->left));

	Node *x = h->left;
	h->left = x->right; 
	if (h->left != nullptr)h->left->parent = h;
	x->right = h; x->right->parent = x;
	x->color = h->color;
	h->color = RED;
	x->size = h->size;
	h->size = 1 + size(h->left) + size(h->right);

	return x;
}
template<typename Key, typename Value, typename Node>
void RedBlackBST<Key, Value, Node>::flipColors(Node *h)
{
	h->color = !h->color;
	h->left->color = !h->left->color;
	h->right->color = !h->right->color;
}






