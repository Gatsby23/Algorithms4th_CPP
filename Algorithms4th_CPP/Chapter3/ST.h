#pragma once
/**
 *����ST�����ű���API
 *
 * ST() ����һ�ŷ��ű�
 * void put(Key key,Value val) ����ֵ�Է������
 * Value get(Key key)
 * void delete(Key key)
 * bool contains(Key key) ��key�Ƿ��ڱ����ж�Ӧ��ֵ
 * bool isEmpty() ���Ƿ�Ϊ��
 * int size() ���м�ֵ�Ե�����
 * 
 *������Ҫ���������⣺
 *1.�ظ�����ô����
 *  ÿ����ֻ��Ӧһ��ֵ��
 *  ����ֵ���Ѵ���ʱ�����µ�ֵ���滻�ɵ�ֵ
 *2.��δ���ɾ��������
 *  ��ʱɾ��
 *  ��ʱɾ��
 *3.��δ�����ĵȼ���
 */

/**
 *������˵���Ƿ���ST��API
 *�����ű������˼�����������ʵ��put��get
 *���������ԻἫ�����չST��API���������ʵ�õĲ���
 *
 *��չAPI
 *
 * Key min() ��С�ļ�
 * Key max() ���ļ�
 * Key floor(Key key) С�ڵ���key������
 * Key ceiling(Key key) ���ڵ���key����С��
 * int rank(Key key) С��key�ļ�������
 * Key select(int k) ����Ϊk�ļ�
 *
 * void deleteMin() ɾ����С�ļ�
 * void deleteMax() ɾ�����ļ�
 * int size(Key lo, Key hi) [lo..hi]֮���������
 *
 *���ϣ����ǿ��Կ��������ǹ��ڼ���λ�õ�һЩ�������ص���rank��������
 *i==rank(select(i)) & key==select(rank(key))
 */


/**
 *  The {@code SequentialSearchST} class represents an (unordered)
 *  symbol table of generic key-value pairs.
 *  
 *  The <em>put</em> and <em>delete</em> operations take linear time; the
 *  <em>get</em> and <em>contains</em> operations takes linear time in the worst case.
 *  The <em>size</em>, and <em>is-empty</em> operations take constant time.
 *  Construction takes constant time.
 */

//����C++��û��null��һ��ֵ������ʹ�ó����ټ�
#include<forward_list>
template<typename Key, typename Value>
class SequentialSearchST
{
public:
	struct Item
	{
		Key key;
		Value value;
	};

private:
	std::forward_list<Item> list;
	int N = 0;

public:
	typedef typename std::forward_list<Item>::iterator Iterator;

	int size() { return N; }
	bool isEmpty() { return N == 0; }
	bool contains(Key key) { return get(key) != Value(); }
	
	Value get(Key key)
	{
		auto prev = list.before_begin();
		for (auto it = list.begin(); it != list.end(); ++it,++prev)
			if (it->key == key)
			{//���в��ң�ʹ������֯����
				auto ret = it->value;
				list->erase_after(prev);
				list.push_front({ key,ret });
				return ret;
			}
		return Value(); //û���ҵ�������ʲô��...
	}

	void put(Key key, Value val)
	{
		for (auto it = list.begin(); it != list.end(); ++it)
			if (it->key == key)
			{
				it->value = val;
				return;
			}

		list.push_front({ key,val });
		++N;
		
	}

	void deleteKey(Key key)
	{
		auto prev = list.before_begin();
		for (auto it = list.begin(); it != list.end(); ++it,++prev)
			if (it->key == key)
			{
				list.erase_after(prev);
				--N;
				return;
			}
	}

	Iterator begin() { return list.begin(); }
	Iterator end() { return list.end(); }

};

/**
*  The {@code BST} class represents an ordered symbol table of generic
*  key-value pairs.
*  <p>
*  This implementation uses a sorted array. It requires that
*  The <em>put</em> and <em>remove</em> operations each take linear time in
*  the worst case; the <em>contains</em>, <em>ceiling</em>, <em>floor</em>,
*  and <em>rank</em> operations take logarithmic time; the <em>size</em>,
*  <em>is-empty</em>, <em>minimum</em>, <em>maximum</em>, and <em>select</em>
*  operations take constant time. Construction takes constant time.
*  <p>
*/
/**
 *����ST������ʵ�֣�rank����Ҫ����
 */
#include<vector>
template<typename Key, typename Value>
class BinarySearchST
{
public:
	struct Item
	{
		Key key;
		Value value;
	};

private:
	std::vector<Item> items;
	int N = 0;
	int cache = -1;//����

public:
	typedef typename std::vector<Item>::iterator Iterator;
	BinarySearchST()
		:BinarySearchST(2)
	{
	}
	BinarySearchST(int cap)
		:items(cap)
	{
	}

	int size() { return N; }
	bool isEmpty() { return N == 0; }
	bool contains(Key key) { return get(key) != Value(); }

	Value get(Key key)
	{
		if (isEmpty())
			return Value();

		int i = rank(key);
		if (i < N && items[i].key == key)
		{//�ӻ���
			cache = i;
			return items[i].value;
		}
			
		return Value();
	}

	int rank(Key key)
	{
		if (cache >= 0)
		{//���漼��
			if (items[cache].key == key)
				return cache;
		}

		int lo = 0, hi = N - 1;
		while (lo <= hi)
		{
			int mid = lo + (hi - lo) / 2;
			if (items[mid].key < key) lo = mid + 1;
			else if (items[mid].key == key) return mid;
			else hi = mid - 1;
		}
		return lo;
	}

	void put(Key key, Value val)
	{	
		
		if (N == items.capacity()) resize(2 * N);


		if (N>0 && key > max())
		{//3.1.28 �������
			items[N++] = { key,val };
			return;
		}
		
		int i = rank(key);

		for (int j = size(); j > i; --j)
			items[j] = items[j - 1];
		items[i] = { key,val };
		++N;
	}

	void deleteKey(Key key)
	{
		if (isEmpty()) return;

		int i = rank(key);

		if (i == size() || items[i].key != key) return;

		for (int j = i; j < size() - 1; ++j)
			items[j] = items[j + 1];
		--N;
	}

	void deleteMin() { deleteKey(min()); }
	void deleteMax() { deleteKey(max()); }
	Key min() { return items[0].key; }
	Key max() { return items[N - 1].key; }

	Key select(int k) { return items[k].key; }
	Key floor(Key key)
	{
		int i = rank(key);
		if (i < N && items[i].key == key) return items[i].key;
		if (i == 0) return Key();
		else return items[i - 1].key;
	}
	Key ceiling(Key key)
	{
		inr i = rank(key);
		if (i == n) return Key();
		else return items[i].key;
	}

	int size(Key lo, Key hi)
	{
		if (hi < lo) return 0;
		if (contains(hi)) return rank(hi) - rank(lo) + 1;
		else return rank(hi) - rank(lo);

	}

	Iterator begin() { return items.begin(); }
	Iterator end() { return items.end(); }

private:
	void resize(int cap)
	{
		std::vector<Item> temp(cap);

		for (int i = 0; i < items.size(); ++i)
			temp[i] = std::move(items[i]);

		items = std::move(temp);
	}
};


/**
 *������Ҫͬʱ�ܹ�֧�ָ�Ч�Ĳ��ҺͲ������ֲ����ķ��ű�ʵ��
 *
 *Ҫ֧�ָ�Ч�Ĳ�������������ƺ���Ҫһ����ʽ�ṹ
 *���ǵ����ӵ��������޷�ʹ�ö��ֲ��ҷ��ģ���Ϊ���ֲ��ҵĸ�Ч��Դ���ܹ�����ͨ������ȡ���κ���������м�Ԫ��
 *Ϊ�˽����ֲ��ҵ�Ч�ʺ����������Խ��������������Ҫ���Ӹ��ӵ����ݽṹ���ܹ�ͬʱӵ�����ߵľ��Ƕ��������
 */

/**
 *���������
 *�����Ķ����������ʵ�ֳ����Ƿǵݹ��
 *����������Ϳ������򼸺����ǡ�˫��̥�������ĸ������ǿ��������еĵ�һ���з�Ԫ�أ�����������е�����ͬ�����ã���Ϳ��������ж���
 *������ĵݹ�������ȫ��Ӧ��
 *
 *������Ա��ƫ�����ڶ���������ķ��ű��ԭ��������֧�ָ�Ч��rank��select��delete�Լ���Χ���ҵȲ���
 *
 */
#include<allocators>
#include<algorithm>
template<typename Key, typename Value>
class BST
{
private:
	struct Node
	{//Ĭ�ϰ�ֵ��������
		Key key;
		Value value;
		Node* left;
		Node* right;
		int size;
	};

	Node *root = nullptr;
	Node *cache = nullptr;//����
public:
	BST()
	{
	}
	//�������죬��ֵ����
	BST(const BST& rhs)
	{
		constructor(rhs.root);
	}
	BST& operator=(const BST& rhs)
	{
		BST tmp(rhs);
		destructor(root);
		root = tmp.root;
		cache = nullptr;
		return *this;
	}
	BST(BST &&rhs) noexcept
	{
		root = rhs.root;
		cache = rhs.cache;
		rhs.root = nullptr;
		rhs.cache = nullptr;
	}
	BST& operator=(BST&& rhs) noexcept
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
	~BST()
	{
		destructor(root);
	}


	bool isEmpty() { return root == nullptr; }
	bool contains(Key key) { return get(key) != Value(); }
	//ע��ݹ��㷨��ݹ�Ľӿ�
	Value get(Key key) 
	{
		//�ӻ���
		if (cache && cache->key == key) return cache->value;
		return get(root, key); 
	}
	/**
	 *ע�����ּ���
	 *���ص����Բ���Ϊ�������������ĸ���㣬������������ȫ�֣�����Ҫ���游�ڵ�
	 *�Ӷ����븸�ڵ��йصĴ���ת�޵�������
	 */
	void put(Key key, Value val) { root = put(root, key, val); }


	void deleteMin() { root = deleteMin(root); }
	void deleteMax() { root = deleteMax(root); }
	void deleteKey(Key key) { root = deleteKey(root, key); }


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

	//���������йصķ���
#ifndef NORDERMETHOD
	int size() { return size(root); }
	Key select(int k) 
	{
		if (cache && size(cache->left) == k) return cache->key;
		return select(root, k)->key; 
	}
	int rank(Key key) 
	{ 
		if (cache && cache->key == key) return size(cache->left);
		return rank(root, key); 
	}
#endif

	//Ϊ�˷������
	struct Item
	{
		Key key;
		Value value;
	};
	std::vector<Item> toArray() { return toArray(min(), max()); }
	std::vector<Item> toArray(const Key lo, const Key hi)
	{
		std::vector<Item> ret;
		toArray(root, ret,lo,hi);
		return ret;
	}

#ifndef NORDERMETHOD
private:
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
		else
		{
			cache = x;
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

private:

	Value get(Node* x, Key key)
	{
		if (x == nullptr) return Value();
		if ( key < x->key) return get(x->left, key);
		else if (x->key < key) return get(x->right, key);
		else
		{
			cache = x;
			return x->value;
		}
	}

	Node* put(Node* x, Key key, Value val)
	{
		if (x == nullptr) return createNode(key, val, 1);
		if (key < x->key) x->left = put(x->left, key, val);
		else if (x->key < key) x->right = put(x->right, key, val);
		else x->value = val;
#ifndef NORDERMETHOD
		x->size = size(x->left) + size(x->right) + 1;
#endif
		return x;
	}



	Node* min(Node* x)
	{
		if (x->left == nullptr) return x;
		else return min(x->left);
	}
	Node* max(Node* x)
	{
		if (x->right == nullptr) return x;
		else return max(x->right);
	}
	Node* floor(Node* x, Key key)
	{
		if (x == nullptr) return nullptr;
		if (key < x->key) return floor(x->left);
		else if (x->key == key) return x;
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
		if (x->key == key) return x;
		else if (x->key < key) return ceiling(x->right, key);

		Node *t = ceiling(x->left, key);
		if (t == nullptr) return x;
		else return t;
	}



	Node* deleteMin(Node *x)
	{	
		if (x->left == nullptr)
		{
			Node* ret = x->right;
			deleteNode(x);
			return ret;//ɾ��x
		}

		x->left = deleteMin(x->left);
#ifndef NORDERMETHOD
		x->size = size(x->left) + size(x->right) + 1;
#endif
		return x;
	}

	Node* deleteMax(Node *x)
	{
		if (x->right == nullptr)
		{
			Node* ret = x->left;
			deleteNode(x);
			return ret;//ɾ��x
		}

		x->right = deleteMax(x->right);
#ifndef NORDERMETHOD
		x->size = size(x->left) + size(x->right) + 1;
#endif
		return x;
	}

	Node* deleteKey(Node *x, Key key)
	{
		if (x == nullptr) return nullptr;

		if (key < x->key) x->left = deleteKey(x->left, key);
		else if (x->key < key) x->right = deleteKey(x->right, key);
		else
		{//ɾ��x��������������ֻ��һ���ӽ�㣬��ôֱ�����ӽ�����x
			//���ӵ�������ӽ�㣬��ô��x�ĺ��������x��x�ĺ����z�����������ұ�Ȼֻ��С�ڵ���һ���ӽ��
			if (x->right == nullptr)
			{
				Node *ret = x->left;
				deleteNode(x);
				return ret;//ɾ��x
			}
			if (x->left == nullptr)
			{
				Node *ret = x->right;
				deleteNode(x);
				return ret;
			}

			Node *t = x;
			x = copyNode(min(t->right));
			x->right = deleteMin(t->right);
			x->left = t->left;
			deleteNode(t);
		}
#ifndef NORDERMETHOD
		x->size = size(x->left) + size(x->right) + 1;
#endif
		return x;
	}

	//����������copy node
	Node* copyNode(Node *x)
	{
		auto ret = createNode(x->key, x->value, x->size);
		ret->left = x->left;
		ret->right = x->right;
		return ret;
	}



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



	void toArray(Node *x, std::vector<Item> &q, const Key lo, const Key hi)
	{
		if (x == nullptr)return;
		if (x->key > lo) toArray(x->left, q, lo, hi);
		if (x->key >= lo && x->key <= hi) q.push_back({ x->key,x->value });
		if (x->key < hi) toArray(x->right, q, lo, hi);

	}





private:
	static std::allocator<Node> alloc;
	Node* createNode(const Key &key, const Value &val, const int n)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&(p->key), key);//ע��һ����ȡ��ַ
		alloc.construct(&(p->value), val);		
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
		alloc.construct(&(p->size), n);

		return p;
	}
	Node* createNode(Key &&key, Value &&val, const int n)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&(p->key), std::move(key));
		alloc.construct(&(p->value), std::move(val));
		alloc.construct(&(p->left), nullptr);
		alloc.construct(&(p->right), nullptr);
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

template<typename Key, typename Value> std::allocator<typename BST<Key, Value>::Node> BST<Key,Value>::alloc;



/**
 *1.������������˵�һ��������ʵ������ӵĴ��������put��delete����������������е�����������Ȼ���Լ���ʹ�ã����ں��˶��������
 *  ��Ч���Ĳ��ҷ�����2-3���и�Ч��ƽ������㷨��
 *2.�˴��������2-3����һ��ʵ�ַ�ʽ��1�������Ӿ�Ϊ�����ӣ�2��û���κν��ͬʱ������������������3��������ɫƽ��
 *  �ı�˴�������Ķ��壬Ҳ������Ϊ2-3-4����һ��ʵ�ַ�ʽ�������㷨���ۣ�
 *3.ʹ�õݹ����ʵ�֣�������⴦�����ڵݹ�֮ǰ����Ϊ�������µĴ�����������ڵݹ�֮����Ϊ�������ϵĴ����ο���������͹鲢����
 *4.������xΪ�����������µ����������ַ�ʽ�ǵݹ�ʵ�ֶ���������ĸ��ַ�����ԭ��
 */


template<typename Key, typename Value>
class RedBlackBST
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
	Node *cache = nullptr;

	Key NULLKEY;
	Value NULLVALUE;


public:
	
	RedBlackBST(Key nullkey=Key(), Value nullvalue=Value())
		:NULLKEY(nullkey),NULLVALUE(nullvalue)
	{

	}



//------------------------------------��������---------------------------------------------------------------------
	//�������죬��ֵ����
	RedBlackBST(const RedBlackBST& rhs)
	{
		root = constructor(rhs.root);
		NULLKEY = rhs.NULLKEY;
		NULLVALUE = rhs.NULLVALUE;
	}

	RedBlackBST& operator=(const RedBlackBST& rhs)
	{
		RedBlackBST tmp(rhs);
		destructor(root);
		root = tmp.root;
		cache = nullptr;
		return *this;
	}

	RedBlackBST(RedBlackBST &&rhs) noexcept
	{
		root = rhs.root;
		cache = rhs.cache;
		NULLKEY = rhs.NULLKEY;
		NULLVALUE = rhs.NULLVALUE;
		rhs.root = nullptr;
		rhs.cache = nullptr;
	}

	RedBlackBST& operator=(RedBlackBST&& rhs) noexcept
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

	~RedBlackBST()
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

	Value get(Node* x, Key key)
	{
		if (x == nullptr) return NULLVALUE;
		if (key < x->key) return get(x->left, key);
		else if (x->key < key) return get(x->right, key);
		else
		{
			cache = x;
			return x->value;
		}
	}

	/**
	 *put�������õ������¶��ϵķ���������2-3����һ��������ʱ��4-��㣬������ת���ֽ⡢�ϴ�
	 */
	Node* put(Node* h, Key key, Value val)
	{
		if (h == nullptr) return createNode(key, val, 1, RED);

		if (key < h->key) h->left = put(h->left, key, val);
		else if (h->key < key) h->right = put(h->right, key, val);
		else h->value = val;

		if (isRed(h->right) && !isRed(h->left)) h = rotateLeft(h);
		if (isRed(h->left) && isRed(h->left->left))h = rotateRight(h);//��Ȼ������Ϊh�϶�����Ҷ�ӣ�Ҷ���ڵ�һ�оͷ�����
		if (isRed(h->left) && isRed(h->right)) flipColors(h);

		h->size = size(h->left) + size(h->right) + 1;
		return h;
	}

	/**
	 *1. deleteMin��������һЩ���϶��µĲ�����ʹ�õ�ǰ����Ҷ��㲻��2-��㣬ͨ���ݹ�ɾ��֮���ٲ���һЩ���¶��ϵĲ������ֽ��������4-���
	 *2. �ڴ˲����У�4-�����������ڵģ���ͬ��put��һ������4-���ͷֽ⣩���ȵ�ɾ��������ɺ��ٽ�����·�������е�4-���ֽ�
	 */



	Node* deleteMin(Node *h)
	{//��֤h����2-��㣬��Ϊ���ǵĴ��������ʹ�����ӽ�㲻��2-��㣬�����������̺����ױ�֤��
		//����������Ҫ����root�����root��2-��㣬��ôֻ��Ҫ�����ɺ�ɫ����
		

		if (h->left == nullptr)
		{
			deleteNode(h);
			return nullptr;//ɾ��
		}
		//h��Ȼ����2-��㣨������3-��㣬Ҳ������4-��㣩
		//h�������Ȼ�ǣ�1. isRed(h.left),isBlack(h) [isRed(h.right)--��Ӧ4-��㣬isBlack(h.right)--��Ӧ3-���]
		//             2. isRed(h),isBlack(h.parent)
		//Ҳ�������h�Ǻ�ɫ�Ļ�����ô���3-��㣨��4-��㣩�տ�ʼ�����h�Ǻ�ɫ�Ļ�����ô���3-��㣨��4-��㣩Ҫ����
		
		//���ԣ�
		//case1��h������3-/4-���Ŀ�ʼԪ�أ�Ҳ�����ǽ���Ԫ��
		//if (!isRed(h)) h->left = deleteMin(h->left);
		//���ԣ�����ִ�е�������ǣ�h is red����ʱ��h.left��Ȼ��black

		//case2�����h�����ӽ�㲻��2-���,��ô��ֱ�ӽ����ӽ��
		//if(isRed(h->left->left)) h->left=deleteMin(h->left);

		//case3:
		//case3.1: ���h�����ӽ����2-��㣬���ֵܽ�㲻��2-���
		//case3.2�����h�����ӽ����2-��㣬���ֵܽ����2-���
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
		//����deleteMin���������ϵĹ����У����ܴ��ڣ�2-��㣬3-��㣬4-��㣨����moveRedLeft��֪��4-���ֻ��һ����̬��
		//����ɾ�����������ܻ����4-���ɾ������������
		//����Ϊֻ��Ҫ���´��뼴��
		//if (h->left==nullptr && isRed(h->right)) h = rotateLeft(h);
		//if (isRed(h->left) && isRed(h->right)) flipColors(h);



		//��Щ�������deleteMax������ͦ���ʵ�
		//����deleteMax�����ܴ��ڣ�2-��㣬3-��㣨right���ӣ���4-��㣬����ɾ�����������ܴ���4-���ɾ�����ҽ������
		//����Ҫ���Ĺ����ǣ��ָ�3-���Ϊleft���ӣ����4����
		//����Ϊֻ��Ҫ���´��룺
		//if (isRed(h->right)) h = rotateLeft(h);
		//if (isRed(h->left) && isRed(h->right)) flipColors(h);

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
				Node *x = min(h->right);
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

	bool contains(Key key) { return get(key) != NULLVALUE; }

	Value get(Key key)
	{	//�ӻ���
		if (cache && cache->key == key) return cache->value;
		return get(root, key);
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
		root = deleteKey(root,key);
		if (!isEmpty()) root->color = BLACK;
	}

	Key min() { return min(root)->key; }

	Key max() { return max(root)->key; }

	Key floor(Key key)
	{
		Node* x = floor(root, key);
		if (x == nullptr) return NULLKEY;
		return x->key;
	}

	Key ceiling(Key key)
	{
		Node *x = ceiling(root, key);
		if (x == nullptr) return NULLKEY;
		return x->key;
	}



//--------------------------------��size�йصķ���----------------------------------------------------------
#ifndef NORDERMETHOD
public:

	int size() { return size(root); }

	Key select(int k)
	{
		if (cache && size(cache->left) == k) return cache->key;
		return select(root, k)->key;
	}

	int rank(Key key)
	{
		if (cache && cache->key == key) return size(cache->left);
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
			cache = x;
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
		if (x->left == nullptr) return x;
		else return min(x->left);
	}

	Node* max(Node* x)
	{
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
		if (x == nullptr) return nullptr;

		auto left = constructor(x->left);
		auto right = constructor(x->right);
		auto ret = createNode(x->key, x->value, x->size, x->color);
		ret->left = left;
		ret->right = right;
		return ret;
	}



//--------------------------------------�ڴ�����й�-------------------------------------------------------------------------
private:
	static std::allocator<Node> alloc;
	Node* createNode(const Key &key, const Value &val, const int n,bool col)
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

template<typename Key, typename Value> 
std::allocator<typename RedBlackBST<Key, Value>::Node> RedBlackBST<Key, Value>::alloc;



template<typename Key,typename Value,typename hashFunc=std::hash<Key>>
class SeparateChainingHashST
{
private:
	static const int INIT_CAPACITY = 4;
	int N = 0;//��ֵ������
	int M;//ɢ�б��С
	std::vector<SequentialSearchST<Key, Value>> st;//������������

public:
	SeparateChainingHashST() :SeparateChainingHashST(INIT_CAPACITY) {}

	SeparateChainingHashST(int cap)
		:M(cap)
	{
		for (int i = 0; i < cap; ++i)
			st.push_back(SequentialSearchST<Key,Value>());	
	}

	int size() { return N; }
	bool isEmpty() { return N == 0; }
	
	bool contains(Key key) { return get(Key) != Value(); }

	Value get(Key key)
	{
		int i = hash(key);
		return st[i].get(key);
	}

	void put(Key key, Value val)
	{
		if (N >= 10 * M) resize(2 * M);

		int i = hash(key);
		if (!st[i].contains(key)) ++N;
		st[i].put(key, val);
	}

	void deleteKey(Key key)
	{
		int i = hash(key);
		if (st[i].contains(key)) --N;
		st[i].deleteKey(key);

		if (M > INIT_CAPACITY&& N <= 2 * M) resize(M / 2);
	}

	std::vector<typename SequentialSearchST<Key, Value>::Item> toArray()
	{
		std::vector<typename SequentialSearchST<Key, Value>::Item> ret;

		for (int i = 0; i < M; ++i)
			for (auto item : st[i])
				ret.push_back(item);
		
		return ret;	
	}

private:

	void resize(int cap)
	{
		SeparateChainingHashST temp(cap);
		for (int i = 0; i < M; ++i)
			for (auto item : st[i])
				temp.put(item.key, item.value);

		M = temp.M;
		N = temp.N;
		st = std::move(temp.st);
	}

	int hash(Key key)
	{
		return (hashFunc(key) & 0x7fffffff) % M;
	}

};

template<typename Key,typename Value,typename hashFunc=std::hash<Key>>
class LinearProbingHashST
{

public:
	struct Item
	{
		Key key;
		Value value;
	};

private:
	static const int INIT_CAPACITY = 4;

	int N = 0;
	int M;
	std::vector<Item> st;

public:
	LinearProbingHashST() :LinearProbingHashST(INIT_CAPACITY) {}

	LinearProbingHashST(int m) :M(m), st(M) {}

	int size() { return N; }
	bool isEmpty() { return N == 0; }

	bool contains(Key key)
	{
		return get(key) != Value();
	}

	void put(Key key, Value val)
	{
		if (n >= m / 2) resize(2 * m);

		int i;
		for (i = hash(key); st[i].key != Key();i=(i+1)%M)
			if (st[i].key == key)
			{
				st[i].value = val;
				return;
			}
		st[i] = { key,val };
		++N;
	}

	Value get(Key key)
	{
		for (int i = hash(key); st[i].key != Key(); i = (i + 1) % M)
			if (st[i].key == key)
				return st[i].value;

		return Value();
	}

	void deleteKey(Key key)
	{
		if (!contains(key)) return;

		int i = hash(key);
		while (st[i].key != key)
			i = (i + 1) % M;
		st[i] = { Key(),Value() };

		i = (i + 1) % M;
		while (st[i].key != Key())
		{
			auto item = st[i];
			st[i] = { Key(),Value() };
			--N;
			put(item.key, item.value);
			i = (i + 1) % M;
		}

		--N;
		if (N > 0 && N <= M / 8)resize(M / 2);
	}

	std::vector<typename LinearProbingHashST::Item> toArray()
	{
		std::vector<typename LinearProbingHashST::Item> ret;
		for (int i = 0; i < M; ++i)
			if (st[i].key != Key())
				ret.push_back(st[i]);
		return ret;
	}

private:

	int hash(Key key)
	{
		return (hashFunc(key) & 0x7fffffff) % M;
	}

	void resize(int m)
	{
		LinearProbingHashST temp(m);

		for (int i = 0; i < m; ++i)
			if (st[i].key != Key())
				temp.put(st[i].key, st[i].value);

		M = temp.M;
		st = std::move(temp.st);
	}

};