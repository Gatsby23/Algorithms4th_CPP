#include<iostream>
#include"pq.h"
#include"../Chapter1/stack.h"

/**
*һ������TopM����������N�����Էǳ��󣩣��ҳ�����M����
*��������е�Nһ�������룬���������㷨��������quick sort�������ƽ������£�ֻ��Ҫ2N�αȽϾͿ��ԣ���������Ϊ�μ�Ѱ�ҵ�M��Ԫ�أ�
*�������ֲ���һ��������ģ�����ʹ�����ȶ���
*/

void topM()
{
	int M = 10;
	PQ<int> pq;
	while (std::cin)
	{
		int tmp;
		std::cin >> tmp;
		pq.push(tmp);
		if (pq.size() > M)
		pq.pop();
	}

	Stack<int> stack;
	while (!pq.isEmpty())
	stack.push(pq.pop());

	for (auto item : stack)
	std::cout << item << std::endl;
}

////����PQ��
//
//int main()
//{
//	std::vector<int> vec{ 10,9,8,7,6,5,4,3,2,1 };
//	PQ<int> pq(vec.begin(), vec.end());
//	//PQ<int,std::greater<int>> pq(vec.begin(), vec.end());
//
//	while (!pq.isEmpty())
//	{
//		std::cout << pq.pop() << std::endl;
//	}
//
//	system("pause");
//
//}


//
///**
// *2.4.26���ο����������2.1.25���ܹ�����һ��Ľ�������
// */

/**
 *2.4.30:��̬��λ�����ҡ�Ҫ������λ������Ȼ���Բ��ò��ҵ�M��Ԫ�صķ��������ǣ��ص����ڶ�̬����
 *ʹ����������������Ǳ���С����λ����������ȶ��У��ұ��Ǳ��ִ��ڵ�����λ������С���ȶ���
 *ֻ�������������num(left)=num(right),����num(left)+1=num(right)
 */

template<typename T>
class MedianFinding
{
public:
	MedianFinding() = default;
	template<typename It> MedianFinding(It beg, It end)
	{
		for (auto it = beg; it != end; ++it)
			push(*it);
	}

	void push(T t)
	{
		if (right.isEmpty())//û��Ԫ��
		{
			right.push(t);
			med = t;
			return;
		}
		
		//��t�����ʵ���λ��
		if (t < med)
			left.push(t);
		else
			right.push(t);

		//����������״̬�����뵽left-> num(left)=num(right)+1; num(left)=num(right);
		//���뵽right-> num(left)+1= num(right); num(left)+2 =num(right)
		//��ʱ״̬���ܻ�ı�Ϊ��num(left)=num(right)+1; num(right)=num(left)+2,������������������
		//���Կ���{1,2,2,2,2,2,3}���ֺ����ظ�Ԫ�ص����

		if (left.size() == right.size() + 1)//��Ȼ֮ǰ��num(left)==num(right),��ʱ��Ȼ��������Ԫ��
			right.push(left.pop());
		else if(right.size()==left.size()+2)//֮ǰ��Ȼ��num(right)==num(left)+1,��ʱ��Ȼ��ż����Ԫ��
			left.push(right.pop());
			

		if (left.size() == right.size())
			med = (left.top() + right.top()) / 2;
		else// if(left.size()+1==right.size())
			med = right.top();
	}

	T median() { return med; }
private:
	PQ<T,std::less<T>> left;
	double med;
	PQ<T, std::greater<T>> right;
};

//int main()
//{
//	std::vector<int> vec{ 10,9,8,7,6,5,4,3,2,1 };
//	MedianFinding<double> pq(vec.begin(), vec.end());
//	//PQ<int,std::greater<int>> pq(vec.begin(), vec.end());
//	std::cout << pq.median() << std::endl;
//
//	system("pause");
//}

///**
//*����������Ҫͨ��index������key
//*һ��ʵ�ַ�ʽ�ǣ�pq[loc]�������<index,key>,aux[index]=loc
//*�����Ļ������ǿ���ͨ��pq[aux[index]].key��ʵ�ַ��ʡ�
//*��ô�����ʵ�����ȶ����أ�
//*���ǿ���pq��key�������ȶ�������ÿ������pqʱ��ͬʱ����aux���ɡ�
//*aux����ͨ��map��ʵ��
//*
//*���ڵĺô���index�����������index��0��N��Χ�Ļ������ǾͿ���ʹ�����飬������map��ʵ��aux
//*���ǻ�����ʵ�ֵ�һ��Ľ��ǣ����������ƶ�<index,key>,ֻ�ƶ�index����������
//*��ʱqp[index]=loc, pq[loc]=index, ���keys[index]=key����ô�Ͳ���Ҫ�ƶ�key��ֻ��Ҫ��ϵqp��pq����
//*����֮ǰ�ķ��������ǽ������ȶ��в�������pq���飬���ı�locʱ��ͬʱ����qp����һ�㲻��Ū����
//*ץס����һ�㣬���ǾͿ���д��IndexMinPQ�Ĵ�����
//*/
//template<typename T, typename Compare = std::less<T>>
//class IndexPQ
//{
//private:
//	int N = 0;
//	std::vector<int> pq; //���������
//	std::vector<int> qp; //����qp[pq[i]]=pq[qp[i]]=i
//	std::vector<T> keys;
//	//��ע��qp[indx]��λ�á�keys[qp[index]]=key, pq[qp[index]]=index
//	Compare comp;
//public:
//	explicit IndexPQ(int maxN)
//		:pq(maxN + 1), qp(maxN + 1), keys(maxN + 1)
//	{
//		for (int i = 0; i < qp.size(); ++i)
//			qp[i] = -1;
//	}
//	void push(int i, T k)
//	{
//		if (contains(i))
//			return;
//		++N;
//		qp[i] = N;
//		keys[i] = k;
//		pq[N] = i;
//		swim(N);
//	}
//	int indexOfTop()
//	{
//		return pq[1];
//	}
//	T top()
//	{
//		return keys[pq[1]];
//	}
//
//	int pop()
//	{//ɾ��keyֵ��С����������Ӧ������
//		int min = pq[1];
//		exch(1, N--);
//		sink(1);
//
//		qp[min] = -1;
//		pq[N + 1] = -1;
//		return min;
//	}
//
//	T keyOf(int i)
//	{//������������Ӧ�ļ�ֵ
//		_ASSERT(contains(i));
//		return keys[i];
//	}
//
//	void changeKey(int i, T k)
//	{
//		if (greater(k, keys[i]))
//		{
//			keys[i] = k;
//			sink(qp[i]);
//		}
//		else
//		{
//			keys[i] = k;
//			swim(qp[i]);
//		}
//	}
//
//	bool contains(int i) { return qp[i] != -1; }
//	void deleteT(int i)
//	{
//		_ASSERT(contains(i));
//		int loc = qp[i];
//		exch(loc, N--);
//		swim(loc);
//		swim(loc);
//		qp[i] = -1;
//	}
//
//	bool isEmpty() { return N == 0; }
//	int size() { return N; }
//
//private:
//	bool compare(int i, int j)
//	{
//		return comp(keys[pq[i]], keys[pq[j]]);
//	}
//
//	void exch(int i, int j)
//	{
//		//����λ�õ�index����
//		std::swap(pq[i], pq[j]);
//		std::swap(qp[pq[i]], qp[pq[j]]);
//	}
//
//	void swim(int k)
//	{
//		while (k > 1 && compare(k >> 1, k))
//		{
//			exch(k, k >> 1);
//			k >>= 1;
//		}
//	}
//
//	void sink(int k)
//	{
//		while ((k << 1) <= N)
//		{
//			int j = k << 1;
//			if (j < N && compare(j, j + 1)) ++j;
//			if (!compare(k, j))break;
//			exch(k, j);
//			k = j;
//		}
//	}
//};

//int main()
//{
//	std::vector<int> vec{ 10,9,8,7,6,5,4,3,2,1 };
//	IndexPQ<double, std::greater<double>> pq(10);
//	pq.push(1, 3.2);
//	pq.push(2, 2.2);
//
//	while (!pq.isEmpty())
//	{
//		std::cout <<pq.top() <<  std::endl;
//		pq.pop();
//	}
//	system("pause");
//}



/**
*֮ǰ�Ķ�·�鲢��������˼·�ǣ�����ÿһ·����һ����ͷ��ָ�룬Ȼ���ҳ����ֵ�������Ӷ���M
*������Ҫ����������Ϣ����һ·����һ·���ĸ�Ԫ�أ�
*��������ȶ��п���ʹ�ø��Ӷȱ�ΪlogM
*/
template<typename T>
std::vector<T> MultiWayMerge(std::vector<std::vector<T>>& a)
{
	std::vector<T> ret;

	int M = a.size();
	std::vector<int> points;
	for (int i = 0; i < M; ++i)
		points.push_back(0);
	IndexPQ<T,std::greater<T>> pq(M);
	for (int i = 0; i < M; ++i)
		if (points[i] != a[i].size())
			pq.push(i, a[i][points[i]++]);

	while (!pq.isEmpty())
	{
		ret.push_back(pq.top());
		int i = pq.pop();
		if (points[i] != a[i].size())
			pq.push(i, a[i][points[i]++]);
	}

	return ret;
}

//int main()
//{
//	std::vector<std::vector<int>> a;
//	a.push_back({ 1,3,5,7,9 });
//	a.push_back({ 2,4,6,8,10 });
//
//	auto ret = MultiWayMerge(a);
//
//	for (auto item : ret)
//		std::cout << item << std::endl;
//	system("pause");
//}

/**
*һ��˵�����������ȶ��е��ô���ʲô�أ�
*�����ڶ�·�鲢ʱһ�����
*����������Ӧ�û���Ҫ���������̽��
*����������ݽṹ��һ���Ϻ÷����ǽ�������һ���ܹ����ٷ���������СԪ�ص�����
*/




