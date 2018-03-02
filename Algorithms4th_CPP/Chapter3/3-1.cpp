#include"ST2017.h"
#include<iostream>
#include<string>

/**
 *ͳ��ÿ�����ʳ��ֵĴ���
 */

void countFrequency(std::istream& in, int minLen)
{

	BST<std::string, int> st;
	std::string word;
	while (in >> word)
	{
		if (word.length() < minLen) continue;//���Խ϶̵ĵ���
		if (!st.contains(word)) st.put(word, 1);
		else st.put(word, st.get(word) + 1);
	}

	auto q = st.toArray();
	word = q.begin()->key;
	int max = q.begin()->value;

	for(auto item:q)
		if (item.value > max)
		{
			word = item.key;
			max = item.value;
		}

	std::cout << word << '\t' << max << std::endl;
	

			
}
//
///**
// *3.1.25:����
// *if (!st.contains(word)) st.put(word, 1);
//	else st.put(word, st.get(word) + 1);
// *���ִ��������ѭ���н�ͬһ��������������
// *Ϊ������������������Ч�ʣ����ǿ���ʹ��һ�ֽ�������ļ�����������������Ƶ���ļ���λ�ñ�����һ�������С�
// *
// *����SequentialSearchST�����ǿ���ʹ��3.1.22����֯���ң�����һ�ֽ�����Ԫ����������ʹ�ñ�����Ƶ�ʽϸߵ�Ԫ�ظ����ױ��ҵ��Ĳ����㷨
// *�����Ļ�����ÿ�β�������ʱ�������ҵ��ļ�ֵ���ƶ�������Ŀ�ͷ����
// *
// *����BinarySearchST����������
// */
//
///**
// *3.1.28 ������룬�޸�BinarySearchST��ʹ�ò���һ���ȵ�ǰ���м�����ļ�ֻ��Ҫ����ʱ��
// *��ˣ�����������ű�ʱ��ֻ��Ҫ����ʱ��
// */
//	//std::ifstream in("E:\\tale.txt");
//
//	//countFrequency(in, 8);
//	
//	/*
//	std::vector<int> a{ 6, 3, 8, 1, 2, 4, 5, 7, 9, 0 };
//	BST<int, int> st;
//	for (int i = 0; i < a.size(); ++i)
//		st.put(a[i], a[i]);
//
//	st.deleteMin();
//	st.deleteMax();
//	st.deleteKey(6);
//	auto v = st.toArray();
//
//	for (auto item : v)
//		std::cout << item.key << '\t' << item.value << std::endl;
//		*/
