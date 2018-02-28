/*
 ѧϰ�����㷨��������ʵ�����壺
 1. �������㷨�ķ�����������ȫ����Ȿ���бȽ��㷨���ܵķ���
 2. ���Ƶļ���Ҳ����Ч����������͵�����
 3. �����㷨���������ǽ������ĵ�һ��

 ��Լ򵥵��㷨��ԭ�����ڣ�
 1. ��Щ�򵥵��㷨��ĳЩ����±�����֮�����۵ĸ����㷨����Ч
 2. �������������ǸĽ������㷨��Ч��
 */
#pragma once

#include"../Chapter1/test.h"
#include"../Chapter1/random.h"
#include<algorithm>
#include<vector>
#include<string>
#include<iostream>
#include"sort.h"

//�Ƚϸ��������㷨������
double time2_1(const std::string& alg, std::vector<double>& v)
{
	Stopwatch timer;
	auto beg = v.begin();
	auto end = v.end();
	if (alg == "Insertion") insertionSort(beg, end);
	else if (alg == "Selection") selectionSort(beg, end);
	else if (alg == "Shell") shellSort(beg, end);
	else if (alg == "InsertionX") insertionSortX(beg, end);

	return timer.elapsedTime();
}



//int main()
//{
//	int N = 10000;
//	int T = 10;
//	double t1 = timeRondomInput(time2_1, "Insertion", N, T);
//	double t2 = timeRondomInput(time2_1, "InsertionX", N, T);
//	double t3 = timeRondomInput(time2_1, "Shell", N, T);
//	std::cout << t1 << '\t' << t2 << '\t' << t1 / t2 << '\n';
//	std::cout << t2 << '\t' << t3 << '\t' << t2 / t3 << '\n';
//	system("pause");
//}

/*************************************************************************************************************************
********************************************��ϰ��*************************************************************************
*************************************************************************************************************************/

/**
 *2.1.14����������:���⿴�Ƹ����������йأ�ֽ����Ϸ����ȴֻ�ܿ��ʼ��������->���������ҳ����ֵ����
 *��������ѡ�������ҳ����ֵ��Ȼ��ŵ�ָ����λ��
 *��length�������������һ�������ֵ��Ȼ���Ƶ������
 *��length-1��ִ����������
 *ѭ������ʽ�����ڵ�i��ѭ��������µ�i-1z���ƣ��Ӵ�С���С�
 */

//����������1.����Ԫ���Ƶ���β��2.������ͷ������Ԫ��
//Ϊ�˷������������ʹ��˫�����
#include<deque>
using std::deque;
void moveHeadToTail(std::deque<int>& dq)
{
	auto item = dq.front();
	dq.pop_front();
	dq.push_back(item);
}

void changeTwoHeadItems(std::deque<int>& dq)
{
	auto first = dq.front();
	dq.pop_front();
	auto second = dq.front();
	dq.pop_front();

	dq.push_front(first);
	dq.push_front(second);
}

void dequeueSort(std::deque<int>& dq)
{
	//���ֵ�һ����������
	for (int length = dq.size(); length > 0; --length)
	{   //ǰlength��Ԫ�أ�Ѱ�����ֵ
		for (int i = 1; i < length; ++i)
		{
			if (dq[0] > dq[1])
			{
				changeTwoHeadItems(dq);
				moveHeadToTail(dq);
			}
			else {
				moveHeadToTail(dq);
			}
		}
		//��ʱ��ǰsize()-length+1��Ԫ�أ�Ϊ��С���������
		for (int i = 0; i < dq.size() - length + 1; ++i)
			moveHeadToTail(dq);
	}
}

///*2.1.15:��������̫����������ѡ��������Ϊ��������������*/
//
///**
// *2.1.27�����������㷨������������
// */
//double timeTria2_1_27(int N)
//{
//	static Random r;
//	int MAX = 1000000;
//	std::vector<int> v;
//	for (int i = 0; i < N; ++i)
//		v.push_back(r.uniformInt(-MAX, MAX));
//
//	Stopwatch timer;
//	shellSort(v.begin(), v.end());
//	return timer.elapsedTime();
//}
//
