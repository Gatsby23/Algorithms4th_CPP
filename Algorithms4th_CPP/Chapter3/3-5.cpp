#include"ST.h"
#pragma once
/**
 *Set
 *
 *API
 * SET() ����һ���ռ���
 * void add(Key key) ����key���뼯��
 * void delete(Key key) �Ӽ�����ɾ����key
 * bool contains(Key key) ��key�Ƿ��ڼ�����
 * bool isEmpty()
 * int size()
 */

template<typename Key>
class SET
{
private:
	RedBlackBST<Key, int> st;

public:
	SET(Key nullkey=Key()) :st(nullkey,-10){}

	void add(Key key)
	{
		st.put(key, 1);
	}

	bool contains(Key key) { return st.contains(key); }

	void deleteKey(Key key) { return st.deleteKey(key); }

	int size() { return st.size(); }
	
	bool isEmpty() { return st.isEmpty(); }

	std::vector<Key> toArray()
	{
		auto v = st.toArray();
		
		std::vector<Key> ret;

		for (auto item : v)
			ret.push_back(item.key);

		return ret;
	}

public:

	SET unions(const SET& that)
	{
		SET<Key> ret;

		auto v = toArray();
		for (auto item : v)
			ret.add(item);

		v = that.toArray();
		for (auto item : v)
			ret.add(item);

		return ret;
	}

	SET intersects(const SET &that)
	{
		SET ret;
		if (size() < that.size())
		{
			auto v = toArray();
			for (auto x : v)
				if (that.contains(x))
					ret.add(x);
		}
		else
		{
			auto v = that.toArray();
			for (auto x : v)
				if (contains(x))
					ret.add(x);
		}

		return ret;
	}

	SET complement(const SET &all)
	{
		SET ret;

		auto v = all.toArray();

		for (auto x : v)
			if (!contains(x))
				ret.add(x);

		return ret;
	}
};

/**
 *3.5.9 3.5.10 ����BST/RedBlackST�������ظ���
 *
 *ֻ��Ҫ�޸�put��delete����
 *ֻҪ���ǰ����еļ�����Ϊ��̼��ɣ�������ͣ��ɾ���Ϳ�����
 */

template<typename Key, typename Value>
class RedBlackMultiBST
{
private:
	static const bool RED = true;
	static const bool BLACK = false;
	struct Node
	{//Ĭ�ϰ�ֵ��������
		Key key;
		Value value;
		Node* left;
		Node* right;
		bool color;
		int size;
	};

	Node *root = nullptr;

public:

	RedBlackMultiBST()
	{
	}



	//------------------------------------��������---------------------------------------------------------------------
	//�������죬��ֵ����
	RedBlackMultiBST(const RedBlackMultiBST& rhs)
	{
		constructor(rhs.root);
	}

	RedBlackMultiBST& operator=(const RedBlackMultiBST& rhs)
	{
		RedBlackMultiBST tmp(rhs);
		destructor(root);
		root = tmp.root;
		cache = nullptr;
		return *this;
	}

	RedBlackMultiBST(RedBlackMultiBST &&rhs) noexcept
	{
		root = rhs.root;
		cache = rhs.cache;
		rhs.root = nullptr;
		rhs.cache = nullptr;
	}

	RedBlackMultiBST& operator=(RedBlackMultiBST&& rhs) noexcept
	{
		if (this != &rhs)
		{
			destructor(root);
			root = rhs.root;
			cache = rhs.cache;
			rhs.root = nullptr;
			rhs.cache = nullptr;
		}
		return *this;
	}

	~RedBlackMultiBST()
	{
		destructor(root);
	}



	//-----------------------------------����������������Ĳ�ͬ�����ڣ�put��delete����--------------------------------------------
private:
	//--------------------������������--------------------------------------
	bool isRed(Node *x)
	{
		if (x == nullptr) return false;//������Ϊ��
		return x->color == RED;
	}

	//����ɫ������תΪ��ɫ������
	Node* rotateLeft(Node *h)
	{
		_ASSERT(isRed(h->right));

		Node *x = h->right;
		h->right = x->left;
		x->left = h;
		x->color = h->color;
		h->color = RED;
		x->size = h->size;
		h->size = 1 + size(h->left) + size(h->right);

		return x;
	}

	//����ɫ������ת��Ϊ��ɫ������
	Node* rotateRight(Node *h)
	{
		_ASSERT(isRed(h->left));

		Node *x = h->left;
		h->left = x->right;
		x->right = h;
		x->color = h->color;
		h->color = RED;
		x->size = h->size;
		h->size = 1 + size(h->left) + size(h->right);

		return x;
	}

	void flipColors(Node *h)
	{
		h->color = !h->color;
		h->left->color = !h->left->color;
		h->right->color = !h->right->color;
	}
	//---------------------End-----------------------------------

	void get(Node* x, std::vector<Value> &v, Key key)
	{
		if (x == nullptr) return;

		if (key < x->key) get(x->left, v, key);
		else if (x->key < key) get(x->right, v, key);
		else
		{
			get(x->left, v, key);
			v.push_back(x->value);
			get(x->right, v, key);
		}
	}


	/**
	*put�������õ������¶��ϵķ���������2-3����һ��������ʱ��4-��㣬������ת���ֽ⡢�ϴ�
	*/
	Node* put(Node* h, Key key, Value val)
	{
		if (h == nullptr) return createNode(key, val, 1, RED);

		if (key < h->key) h->left = put(h->left, key, val);
		else if (h->key <= key) h->right = put(h->right, key, val);


		if (isRed(h->right) && !isRed(h->left)) h = rotateLeft(h);
		if (isRed(h->left) && isRed(h->left->left))h = rotateRight(h);//��Ȼ������Ϊh�϶�����Ҷ�ӣ�Ҷ���ڵ�һ�оͷ�����
		if (isRed(h->left) && isRed(h->right)) flipColors(h);

		h->size = size(h->left) + size(h->right) + 1;
		return h;
	}




	Node* deleteMin(Node *h)
	{
		if (h->left == nullptr)
		{
			deleteNode(h);
			return nullptr;//ɾ��
		}

		if (!isRed(h->left) && !isRed(h->left->left))
			h = moveRedLeft(h);

		h->left = deleteMin(h->left);
		return balance(h);
	}

	Node* moveRedLeft(Node *h)
	{
		//h��Ȼ����2-���
		//����case3�����������ʹ��h�����ӽ�㲻��2-���

		//����case3.2��ֻ��Ҫ��h�����3-/4-������СԪ�أ��˴�����h�������ӽ�������ֵܣ��˴���h.right)�����ӽ��ϲ�Ϊ4-��㼴��
		flipColors(h);

		//����case3.1��
		if (isRed(h->right->left))//��Ȼ��������moveRedLeft�ĵ���������h.right��Ȼ����
		{//��ִ��flipColors(h)
			h->right = rotateRight(h->right);
			h = rotateLeft(h);
			flipColors(h);
		}

		return h;
	}

	Node* balance(Node *h)
	{

		if (isRed(h->right)) h = rotateLeft(h);
		if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
		if (isRed(h->left) && isRed(h->right)) flipColors(h);

		h->size = size(h->left) + size(h->right) + 1;
		return h;
	}

	Node* deleteMax(Node *h)
	{//ɾ�����ֵ����Ϊ�����Ӷ��������ӣ��������������������У���Ҫ�Ѻ�����ת��Ϊ������
	 //�����������ɾ����Сֵ�ӽ�
		if (isRed(h->left))
			h = rotateRight(h);

		if (h->right == nullptr)
		{
			deleteNode(h);
			return nullptr;
		}
		if (!isRed(h->right) && !isRed(h->right->left))
			h = moveRedRight(h);

		h->right = deleteMax(h->right);
		return balance(h);
	}

	Node* moveRedRight(Node *h)
	{
		flipColors(h);

		if (isRed(h->left->left))
		{
			h = rotateRight(h);
			flipColors(h);
		}
		return h;
	}

	Node* deleteKey(Node *h, Key key)
	{//��ǰ��㲻Ϊ2-���
	 //����������������ôȷ����ǰ�������ӽ�㲻Ϊ2-���
	 //������ұ���������ô���ȸ��ĵ�ǰ���Ϊ�����ӣ�Ȼ��ȷ����ǰ�������ӽ�㲻Ϊ2-���
	 //���ɾ����ǰ��㣺��ôҲ��ͬ���ģ�ȷ�����ӽ�㲻Ϊ2-��㣬�Դ�Ϊ��ʼ������deleteMin��h.right��
		if (key < h->key)
		{
			if (!isRed(h->left) && !isRed(h->left->left))
				h = moveRedLeft(h);
			h->left = deleteKey(h->left, key);
		}
		else
		{
			if (isRed(h->left))
				h = rotateRight(h);
			if (key == h->key &&h->right == nullptr)
			{
				deleteNode(h);
				return nullptr;
			}

			if (!isRed(h->right) && !isRed(h->right->left))
				h = moveRedRight(h);

			if (key == h->key)
			{
				//��ɾ��ǰ������ɾ�����
				Node *x = max(h->left);
				while (x != nullptr && x->key == key)
				{
					h->left = deleteMax(h->left);
					x = max(h->left);
				}

				x = min(h->right);
				while (x != nullptr && x->key == key)
				{
					h->right = deleteMin(h->right);
					x = min(h->right);
				}

					
				x = min(h->right);
				h->key = x->key;
				h->value = x->value;
				h->right = deleteMin(h->right);
			}
			else h->right = deleteKey(h->right, key);
		}

		return balance(h);
	}
	//-------------------------------�ӿں���-----------------------------------------------------------------------------
public:
	bool isEmpty() { return root == nullptr; }

	bool contains(Key key) { return get(key) != Value(); }

	std::vector<Value> get(Key key)
	{
		std::vector<Value> ret;
		get(root, ret,key);
		return ret;
	}

	void put(Key key, Value val)
	{
		root = put(root, key, val);
		root->color = BLACK;
	}

	void deleteMin()
	{
		if (!isRed(root->left) && !isRed(root->right)) root->color = RED;//��֤��ǰ��㲻��2-���

		root = deleteMin(root);
		if (!isEmpty()) root->color = BLACK;
	}

	void deleteMax()
	{
		if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
		root = deleteMax(root);
		if (!isEmpty()) root->color = BLACK;
	}

	void deleteKey(Key key)
	{
		if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
		root = deleteKey(root, key);
		if (!isEmpty()) root->color = BLACK;
	}

	Key min() { return min(root)->key; }

	Key max() { return max(root)->key; }

	Key floor(Key key)
	{
		Node* x = floor(root, key);
		if (x == nullptr) return Value();
		return x->key;
	}

	Key ceiling(Key key)
	{
		Node *x = ceiling(root, key);
		if (x == nullptr) return Value();
		return x->key;
	}



	//--------------------------------��size�йصķ���----------------------------------------------------------
#ifndef NORDERMETHOD
public:

	int size() { return size(root); }

	Key select(int k)
	{
		return select(root, k)->key;
	}

	int rank(Key key)
	{
		return rank(root, key);
	}
#endif



	//-------------------------------������й�---------------------------------------------------------------------------------
public:

	struct Item
	{
		Key key;
		Value value;
	};

	std::vector<Item> toArray() { return toArray(min(), max()); }

	std::vector<Item> toArray(const Key lo, const Key hi)
	{
		std::vector<Item> ret;
		toArray(root, ret, lo, hi);
		return ret;
	}


	//--------------------------------������й�-----------------------------------------------------------------------------
public:
	bool is23Tree() { return is23Tree(root); }

	bool isBalanced()
	{
		int black = 0;//number of black links on path from root to min
		Node *x = root;
		while (x != nullptr)
		{
			if (!isRed(x)) ++black;
			x = x->left;
		}
		return isBalanced(root, black);
	}

private:

	bool is23Tree(Node *h)
	{
		if (h == nullptr) return true;
		if (isRed(h->right)) return false;
		if (h != root &&isRed(h) && isRed(h->left))
			return false;
		return is23Tree(h->left) && is23Tree(h->right);
	}

	bool isBalanced(Node *x, int black)
	{
		if (x == nullptr) return black == 0;
		if (!isRed(x)) --black;
		return isBalanced(x->left, black) && isBalanced(x->right, black);
	}

	//--------------------------------�ӿں�����ʵ��1-------------------------------------------------------------------------
#ifndef NORDERMETHOD
private:
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
		else
		{
			return x;
		}
	}

	int rank(Node *x, Key key)
	{
		if (x == nullptr) return 0;
		if (key < x->key) return rank(x->left, key);
		else if (x->key < key) return 1 + size(x->left) + rank(x->right, key);
		else return size(x->left);
	}
#endif



	//------------------------------��ӿں����йص�ʵ��2-------------------------------------------------------------------------
private:

	Node* min(Node* x)
	{
		if (x == nullptr) return nullptr;
		if (x->left == nullptr) return x;
		else return min(x->left);
	}

	Node* max(Node* x)
	{
		if (x == nullptr) return nullptr;
		if (x->right == nullptr) return x;
		else return max(x->right);
	}

	Node* floor(Node* x, Key key)
	{
		if (x == nullptr) return nullptr;
		if (key < x->key) return floor(x->left);
		else if (x->key == key) return x;

		Node *t = floor(x->right, key);
		if (t != nullptr) return t;
		else return x;
	}

	Node* ceiling(Node *x, Key key)
	{
		if (x == nullptr) return nullptr;
		if (x->key == key) return x;
		else if (x->key < key) return ceiling(x->right, key);

		Node *t = ceiling(x->left, key);
		if (t == nullptr) return x;
		else return t;
	}

	void toArray(Node *x, std::vector<Item> &q, const Key lo, const Key hi)
	{
		if (x == nullptr)return;
		if (x->key > lo) toArray(x->left, q, lo, hi);
		if (x->key >= lo && x->key <= hi) q.push_back({ x->key,x->value });
		if (x->key < hi) toArray(x->right, q, lo, hi);

	}



	//----------------------------------��������-----------------------------------------------------------------------------

	void destructor(Node *x)
	{
		if (x == nullptr) return;
		destructor(x->left);
		destructor(x->right);
		deleteNode(x);
	}

	Node* constructor(Node* x)
	{
		if (x == nullptr) return;

		auto left = constructor(x->left);
		auto right = constructor(x->right);
		auto ret = copyNode(x);
		ret->left = left;
		ret->right = right;
		return ret;
	}



	//--------------------------------------�ڴ�����й�-------------------------------------------------------------------------
private:
	static std::allocator<Node> alloc;
	Node* createNode(const Key &key, const Value &val, const int n, bool col)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&(p->key), key);//ע��һ����ȡ��ַ
		alloc.construct(&(p->value), val);
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
		alloc.construct(&(p->color), col);
		alloc.construct(&(p->size), n);

		return p;
	}
	Node* createNode(Key &&key, Value &&val, const int n, bool col)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&(p->key), std::move(key));
		alloc.construct(&(p->value), std::move(val));
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
		alloc.construct(&(p->color), col);
		alloc.construct(&(p->size), n);
		return p;
	}
	void deleteNode(Node* node)
	{
		alloc.destroy(&(node->key));
		alloc.destroy(&(node->value));
		alloc.deallocate(node, 1);
	}

};

template<typename Key, typename Value> std::allocator<typename RedBlackMultiBST<Key, Value>::Node> RedBlackMultiBST<Key, Value>::alloc;

/**
 *3.5.8 & 3.5.19
 *hashTable MultiST
 */
#include<unordered_map>
template<typename Key, typename Value, typename hashFunc = std::hash<Key>>
class SeparateChainingHashMultiST
{
public:
	struct Item
	{
		Key key;
		Value value;
	};

private:
	static const int INIT_CAPACITY = 4;
	int N = 0;//��ֵ������
	int M;//ɢ�б��С
	std::vector<std::forward_list<Item>> st;



public:
	SeparateChainingHashMultiST() :SeparateChainingHashMultiST(997) {}

	SeparateChainingHashMultiST(int m) :M(m), st(m) {}

	int size() { return N; }

	bool isEmpty() { return N == 0; }

	bool contains(Key key) { return get(key) != Value(); }

	Value get(Key key)
	{
		size_t i = hash(key);
		for (auto x = st[i].begin(); x != st[i].end(); ++x)
			if (x->key == key) return x->value;

		return Value();
	}

	std::vector<Value> getAll(Key key)
	{
		std::vector<Value> ret;
		size_t i = hash(key);
		for (auto x = st[i].begin(); x != st[i].end(); ++x)
			if (x->key == key) ret.push_back(x->value);
		return ret;
	}

	void put(Key key, Value val)
	{
		if (N >= 10 * M) resize(2 * M);
		size_t i = hash(key);
		++N;

		st[i].push_front({ key,val });
	}

	void deleteKey(Key key)
	{
		size_t i = hash(key);

		auto begin = st[i].begin();
		auto prev = st[i].before_begin();
		for (auto it = begin; it != st[i].end(); ++it)
		{
			if (it->key == key)
			{
				st[i].erase_after(prev);
				it = prev;
			}
			prev = it;
		}

		if (M > INIT_CAPACITY&& N <= 2 * M) resize(M / 2);
	}

	std::vector<Item> toArray()
	{
		std::vector<Item> ret;
		for (int i = 0; i < M; ++i)
			for (auto x = st[i].begin(); x != st[i].end(); ++x)
				ret.push_back({ x->key,x->value });
		return ret;
	}

private:
	void resize(int cap)
	{
		SeparateChainingHashMultiST temp(cap);
		for (int i = 0; i < M; ++i)
			for (auto x = st[i].begin(); x != st[i].end(); ++x)
				temp.put(x->key, x->value);


		M = cap;
		st = std::move(temp.st);
	}

	size_t hash(Key key)
	{
		hashFunc h;
		return (h(key)&0x7fffffff) % M;
	}


};

/**
 *3.5.24 ���ص����������
 *ֻҪ���¶���һ���������ͼ���
 */
template<typename T>
class Interval
{
private:
	T lo;
	T hi;

public:
	Interval(T t1 = T(), T t2 = T()) :lo(t1), hi(t2) {}

	Interval(T t) :lo(t), hi(t) {}

	bool operator<(const Interval& rhs)
	{
		return hi < rhs.lo;
	}
	bool operator<=(const Interval& rhs)
	{
		return (*this<rhs)||(*this==rhs);
	}
	bool operator>(const Interval &rhs)
	{
		return lo > rhs.hi;
	}
	bool operator>=(const Interval &rhs)
	{
		return (*this>rhs) || (*this == rhs);
	}
	bool operator==(const Interval &rhs)
	{
		return (lo <= rhs.lo) && (rhs.hi <= hi);
	}
	bool operator!=(const Interval &rhs)
	{
		return !(*this == rhs);
	}

};
#include<iostream>
/*
int main()
{
	RedBlackBST<Interval<int>, int> st;
	st.put(Interval<int>(1643, 2033),1);
	st.put(Interval<int>(5532, 7643), 2);
	st.put(Interval<int>(8999, 10332), 3);
	st.put(Interval<int>(5666653, 5669321), 4);

	std::cout << st.get(Interval<int>(9122,9122));




	system("pause");

}*/

