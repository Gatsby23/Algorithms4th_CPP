/**
* ʵ�ֵ�������Ҫ���Ǽ������⣺
* 1. ��ν����ڴ�Ķ�̬���䣿����ʹ��Ĭ�ϵĿռ�������allocate
* 2. ���ʵ�ֵ�����������ʹ�ýڵ��ָ����Ϊ����������ôend��ʲô�أ�����������Ҫ��List���б���һ��end�ı�־
* ����ʹ��dummy�ڵ㣬Ҳ����ָ������ʼ�ڵ�Ľڵ㣨before first node��
* dummy-->first node-->....-->last node--|
* ��--------------------------------------|
*/
#include<memory>

template<typename Item>
class SingleList
{
	class Node;//������
	class Iterator;

private://���ݳ�Ա
	Node* dummy;//equals to off last��
	size_t N; //the numbers of list

public: //��������
	SingleList() :N(0)
	{
		dummy = alloc.allocate(1);
		dummy->next = dummy;
	}
	bool isEmpty() const { return N == 0; }
	int size() const { return N; }

public://���Ԫ�أ�ɾ��Ԫ��
	void popBack();                                            //ɾ�����һ��Ԫ�أ�ϰ��1.3.19
	void erase(int k);                                         //ɾ����k��Ԫ�أ�ϰ��1.3.20
	Iterator removeAfter(Iterator it);                         //ɾ��it����Ľڵ㣬ϰ��1.3.24
	int remove(const Item &key);                               //ɾ��key��ϰ��1.3.26


	Iterator insertAfter(Iterator it, const Item &item);       //��it�����Ԫ�أ�ϰ��1.3.25
	Iterator pushFront(const Item &s);                         //��������ǰ�����Ԫ��

	bool find(const Item &key);                                //find key��ϰ��1.3.21
	Iterator max() const;                                          //ϰ��1.3.27
	Iterator reverse(Iterator beg);                            //��ת����ϰ��1.3.30

public: //��������
	Iterator beforeBegin() const { return dummy; }
	Iterator begin() const { return dummy->next; }
	Iterator end() const { return dummy; }

public: //��������
	SingleList(const SingleList &rhs);
	SingleList(SingleList &&rhs) noexcept;
	SingleList& operator=(const SingleList &rhs);
	SingleList& operator=(SingleList &&rhs) noexcept;
	~SingleList();
private:
	void free();//�������Ƶĸ������������ͷ��������нڵ�


private: //�ڲ���
	struct Node
	{
		Node *next;
		Item item;
	};
	struct Iterator
	{
		Node* node;
		Iterator(Node* s) :node(s) {}
		bool operator==(const Iterator &rhs) const{ return node == rhs.node; }
		bool operator!=(const Iterator &rhs) const{ return !(*this == rhs); }
		Item& operator*() const { return node->item; }
		Item* operator->() const { return &(node->item); }
		Iterator& operator++(){ node = node->next; return *this; }
		Iterator& operator++(int){ Iterator ret = *this; ++*this; return ret; }
	};


private: //��̬�ڴ����
	static std::allocator<Node> alloc;
	//create Node�ִ�����ֵ����ֵ
	Node* createNode(const Item &s)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&(p->item), s);//ע��һ����ȡ��ַ
		return p;
	}
	Node* createNode(Item &&s)
	{
		auto p = alloc.allocate(1);
		alloc.construct(&(p->item), std::move(s));
		return p;
	}
	void deleteNode(Node* node)
	{
		alloc.destroy(&(node->item));
		alloc.deallocate(node, 1);
	}
};

template<typename Item> std::allocator<typename SingleList<Item>::Node> SingleList<Item>::alloc;



//�������ƺ���
template<typename Item>
SingleList<Item>::SingleList(const SingleList &rhs) 
	:SingleList()
{
	for (auto item : rhs) 
		pushFront(item);

	reverse(begin());
	N = rhs.N;

}

template<typename Item>
SingleList<Item>::SingleList(SingleList &&rhs) noexcept
	:dummy(rhs.dummy), N(rhs.N)
{
	rhs.dummy = nullptr;
	rhs.N = 0;
}

template<typename Item> 
SingleList<Item>& SingleList<Item>::operator=(const SingleList &rhs)
{
	if (*this != rhs)
	{
		free();
		for (auto item : rhs)
			pushFront(item);

		reverse(begin());
		N = rhs.N;
	}
}
template<typename Item>
SingleList<Item>& SingleList<Item>::operator=(SingleList &&rhs) noexcept
{
	if (*this != rhs)
	{
		free();
		dummy = rhs.dummy;
		N = rhs.N;
		rhs.dummy = nullptr;
		rhs.N = 0;
	}
}

template<typename Item>
SingleList<Item>::~SingleList()
{
	free();
}

template<typename Item>
void SingleList<Item>::free()
{
	while (!isEmpty())
	{
		erase(1);
	}
	N = 0;
	dummy = nullptr;
}
//-------�������ƺ�������-----------------------------------------//

template<typename Item>
void  SingleList<Item>::popBack()
{
	erase(N);
}

template<typename Item>
void  SingleList<Item>::erase(int k)
{
	_ASSERT(k <= N);
	auto cur = dummy;
	auto prev = cur;
	for (int i = 0; i < k; ++i)
	{
		prev = cur;
		cur = cur->next;
	}

	prev->next = cur->next;
	deleteNode(cur);
	--N;
}

template<typename Item>
bool SingleList<Item>::find(const Item &s)
{
	for (auto c : *this)
		if (c == s) return true;
	return false;
}

template<typename Item> typename SingleList<Item>::Iterator
SingleList<Item>::removeAfter(Iterator it)
{
	auto it_copy = it;
	if (it_copy == end() || ++it_copy == end())
		return it;

	(it.node)->next = (it_copy.node)->next;
	deleteNode(it_copy.node);
	--N;
	return ++it;
}

template<typename Item> typename SingleList<Item>::Iterator
SingleList<Item>::insertAfter(Iterator it, const Item &item)
{
	auto newNode = createNode(item);
	newNode->next = (it.node)->next;
	(it.node)->next = newNode;
	++N;
	return Iterator(newNode);
}

template<typename Item> typename SingleList<Item>::Iterator
SingleList<Item>::pushFront(const Item &s)
{
	return insertAfter(beforeBegin(), s);
}

template<typename Item>
int SingleList<Item>::remove(const Item &key)
{
	int count = 0;
	auto prev = dummy;
	auto cur = prev;
	for (int i = 0; i < N; ++i)
	{
		prev = cur;
		cur = cur->next;
		if (cur->item == key)
		{
			prev->next = cur->next;
			deleteNode(cur);
			++count;
			--N;
			cur = prev;//��������forѭ����ͬ��
		}
	}
	return count;
}

template<typename Item> typename SingleList<Item>::Iterator
SingleList<Item>::max() const
{
	if (isEmpty()) return end();
	auto ret = begin();
	for (auto it = begin(); it != end(); ++it)
		if (*ret < *it) ret = it;
	return ret;
}


//����ʵ�ֵĻ���Ҫ����prev��cur��next�����ڵ㣬������õݹ�ʵ��
template<typename Item> typename SingleList<Item>::Iterator
SingleList<Item>::reverse(Iterator beg)
{
	//auto beg = begin();
	if (beg.node == dummy) return end();
	if ((beg.node)->next == dummy) return beg;

	Iterator second((beg.node)->next);
	auto rest = reverse(second);
	(second.node)->next = (beg.node);
	(beg.node)->next = dummy;
	dummy->next = rest.node;
	return rest;
}

//���Դ���
//#include<iostream>
//int main()
//{
//�������Ʋ��ԣ�
//SingleList<int> s1;
//s1.pushFront(1);
//s1.pushFront(2);
//s1.pushFront(3);
//
//SingleList<int> s2(s1);
//SingleList<int> s3(std::move(s1));
//SingleList<int> s4 = s2;
//SingleList<int> s5 = std::move(s4);
// ���ܲ��ԣ�
//	SingleList<int> list;
//	list.pushFront(1);
//	list.pushFront(2);
//	list.pushFront(3);
//
//	for (auto item : list) std::cout << item << ' ';
//	std::cout << std::endl;
//
//	list.erase(2);
//	for (auto item : list) std::cout << item << ' ';
//	std::cout << std::endl;
//
//	if (list.find(5)) std::cout << "Yes, found 5";
//	else std::cout << "No, Not found 5";
//	std::cout << std::endl;
//
//	list.pushFront(4);
//	list.pushFront(5);
//	if (list.find(5)) std::cout << "Yes, found 5";
//	else std::cout << "No, Not found 5";
//	std::cout << std::endl;
//
//	list.removeAfter(list.begin());
//	for (auto item : list) std::cout << item << ' ';
//	std::cout << std::endl;
//
//	list.remove(3);
//	for (auto item : list) std::cout << item << ' ';
//	std::cout << std::endl;
//
//
//	std::cout << *list.max() << std::endl;
//
//	list.pushFront(6);
//	list.pushFront(7);
//	list.reverse(list.begin());
//	for (auto item : list) std::cout << item << ' ';
//	std::cout << std::endl;
//
//	system("pause");
//
//}


