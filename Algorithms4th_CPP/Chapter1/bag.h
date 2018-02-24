#pragma once
#include<forward_list>
#include<string>
/**
 * Bag�಻֧��ɾ��Ԫ�أ�֧�ֲ����Լ�������
 * ʵ�����ֹ���������ݽṹ��C++��vector
 * ������Ϊ����Ҫ������ʣ�����ListҲ����ʹ��
 * ��Ϊʵ��һ������Ҫ���ǵ�ϸ���Ѿ���singleList�������ʵ���ˣ���������ֻ��Ҫ��װһ�¾ͺ�
 *
 * API
 * Bag()				����һ���ձ���
 * void add(Item item)	���һ��Ԫ��
 *
 * bool isEmpty()		�жϱ����Ƿ�Ϊ��
 * int size()			�����е�����
 *
 * Iter begin();		����
 * Iter end();
 *
 * string toString()	����bag���ַ�����ʾ
 */


template<typename Item>
class Bag
{
public:
	typedef std::forward_list<Item> List;
	typedef typename List::iterator Iterator;
	typedef typename List::const_iterator CIterator;
private:
	List list;
	int N;
public:
	Bag() :list(List()), N(0) {}//ʹ���Դ��Ŀ������ƺ���
	bool isEmpty() const{ return list.empty(); }
	int size() const{ return N; }

	void add(const Item &item)
	{
		list.push_front(item);
		++N;
	}
	void add(Item &&item)
	{
		list.push_front(std::forward<Item>(item));
		++N;
	}


	Iterator begin() { return list.begin(); }
	CIterator begin() const { return list.begin(); }

	Iterator end() { return list.end(); }
	CIterator end() const { return list.end(); }

	/**
	 *return the string representation of this bag
	 *����ʵ�֣�Item������ת����string��ͳһAPI,java����toString(),C++û��
	 */

};


