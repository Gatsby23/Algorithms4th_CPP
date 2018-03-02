#pragma once


/**
 *  The {@code PQ} class represents a priority queue of generic keys.
 *  It supports the usual <em>insert</em> and <em>delete-the-maximum</em>
 *  operations, along with methods for peeking at the maximum key,
 *  testing if the priority queue is empty, and iterating through
 *  the keys.
 *  <p>
 *  This implementation uses a binary heap.
 *  The <em>insert</em> and <em>delete-the-maximum</em> operations take
 *  logarithmic amortized time.
 *  The <em>max</em>, <em>size</em>, and <em>is-empty</em> operations take constant time.
 *  Construction takes time proportional to the specified capacity or the number of
 *  items used to initialize the data structure.
 *  <p>
 * API
 * PQ() ����һ�����ȶ���
 * PQ(int max) ����һ���������Ϊmax�����ȶ���
 * PQ(It beg, It end) ��a[]��Ԫ�ش���һ�����ȶ���
 * void push(Key v) �����ȶ��в���һ��Ԫ��
 * Key top() �������/��СԪ��
 * Key pop() ɾ�����������/��СԪ��
 * bool isEmpty() ���ض����Ƿ�Ϊ��
 * int size() �������ȶ����е�Ԫ�ظ���
 */

/**
 *���ȶ��е�ʵ�ַ�ʽ���ֶ���������ײ�ʵ�ֶ��ԣ����Ի��������������
 *��������һ�֣����Ի���stack�ı�ʽ��ʵ�֣�
 *����ʵ�֣�����Ԫ��->push();delMax�����߽�Ԫ�������Ԫ�ػ�����Ȼ��pop����
 *����ʵ�֣�����Ԫ��->��������->pop()��delMax��ֱ��pop����
 *
 *Ȼ��������Ҫѧ�Ķ����������������ļ�ʵ�֣����Ƕ����ʵ��
 *��һ�ö�������ÿ����㶼���ڵ������������ӽ��ʱ��������Ϊ������
 *�������һ���ܹ��ö��������ȫ�����������Ԫ�أ����������а��ղ㼶�洢�����Բ�ʹ������ĵ�һ��λ�ã�
 */

//MaxPQ -> Compare = less<T>
//MinPQ -> Compare = greater<T>
#include<vector>
#include<functional>
template<typename T, typename Compare= std::less<T>>
class PQ
{
public:
	explicit PQ(const Compare &x = Compare()) :comp(x) {}
	template<typename InputIt> PQ(InputIt first, InputIt last, const Compare &x = Compare())
		:pq(first, last), comp(x)
	{
		int N = pq.size();
		for (int k = (N-1) / 2; k >= 0; --k)
			sink(k);
	}

	void push(const T &t)
	{
		pq.push_back(t);
		int N = pq.size();
		swim(N - 1);
	}

	T pop()
	{
		T ret = pq[0];
		int N = pq.size();
		pq[0] = pq[N-1];
		pq.pop_back();
		if(!pq.empty())sink(0);
		return ret;
	}
	
	T top() const { return pq[0]; }
	bool isEmpty() { return pq.empty(); }
	int size() { return pq.size(); }

private:
	void swim(int k)
	{
		auto key = pq[k];
		while (k > 0 && comp(pq[(k-1) >> 1], key))
		{
			//exch((k-1) >> 1, k);
			//k = (k - 1) >> 1;
			//ʹ��2.4.26 ����������򣬼��ٽ�������
			pq[k] = std::move(pq[(k - 1) >> 1]);
			k = (k - 1) >> 1;
		}
		pq[k] = std::move(key);
	}

	void sink(int k)
	{
		auto key = pq[k];
		int N = pq.size();

		while ((k<<1) +1 < N)
		{
			int j = (k << 1) + 1;
			if (j < N-1 && compare(j, j + 1)) ++j;
			if (!comp(key, pq[j])) break;
			//exch(k, j);
			pq[k] = std::move(pq[j]);
			k = j;
		}
		pq[k] = std::move(key);
	}

	bool compare(int i, int j)
	{
		return comp(pq[i], pq[j]);
	}

	void exch(int i, int j)
	{
		using std::swap;
		swap(pq[i], pq[j]);
	}

private:
	std::vector<T> pq;
	Compare comp;
};


/**
*����������Ҫͨ��index������key
*һ��ʵ�ַ�ʽ�ǣ�pq[loc]�������<index,key>,aux[index]=loc
*�����Ļ������ǿ���ͨ��pq[aux[index]].key��ʵ�ַ��ʡ�
*��ô�����ʵ�����ȶ����أ�
*���ǿ���pq��key�������ȶ�������ÿ������pqʱ��ͬʱ����aux���ɡ�
*aux����ͨ��map��ʵ��
*
*���ڵĺô���index�����������index��0��N��Χ�Ļ������ǾͿ���ʹ�����飬������map��ʵ��aux
*���ǻ�����ʵ�ֵ�һ��Ľ��ǣ����������ƶ�<index,key>,ֻ�ƶ�index����������
*��ʱqp[index]=loc, pq[loc]=index, ���keys[index]=key����ô�Ͳ���Ҫ�ƶ�key��ֻ��Ҫ��ϵqp��pq����
*����֮ǰ�ķ��������ǽ������ȶ��в�������pq���飬���ı�locʱ��ͬʱ����qp����һ�㲻��Ū����
*ץס����һ�㣬���ǾͿ���д��IndexMinPQ�Ĵ�����
*/
template<typename T, typename Compare = std::less<T>>
class IndexPQ
{
private:
	int N = 0;
	std::vector<int> pq; //���������
	std::vector<int> qp; //����qp[pq[i]]=pq[qp[i]]=i
	std::vector<T> keys;
	//��ע��qp[indx]��λ�á�keys[qp[index]]=key, pq[qp[index]]=index
	Compare comp;
public:
	explicit IndexPQ(int maxN)
		:pq(maxN + 1), qp(maxN + 1), keys(maxN + 1)
	{
		for (int i = 0; i < qp.size(); ++i)
			qp[i] = -1;
	}
	void push(int i, T k)
	{
		if (contains(i))
			return;
		++N;
		qp[i] = N;
		keys[i] = k;
		pq[N] = i;
		swim(N);
	}
	int indexOfTop()
	{
		return pq[1];
	}
	T top()
	{
		return keys[pq[1]];
	}

	int pop()
	{//ɾ��keyֵ��С����������Ӧ������
		int min = pq[1];
		exch(1, N--);
		sink(1);

		qp[min] = -1;
		pq[N + 1] = -1;
		return min;
	}

	T keyOf(int i)
	{//������������Ӧ�ļ�ֵ
		_ASSERT(contains(i));
		return keys[i];
	}

	void changeKey(int i, T k)
	{
		if (greater(k, keys[i]))
		{
			keys[i] = k;
			sink(qp[i]);
		}
		else
		{
			keys[i] = k;
			swim(qp[i]);
		}
	}

	bool contains(int i) { return qp[i] != -1; }
	void deleteT(int i)
	{
		_ASSERT(contains(i));
		int loc = qp[i];
		exch(loc, N--);
		swim(loc);
		swim(loc);
		qp[i] = -1;
	}

	bool isEmpty() { return N == 0; }
	int size() { return N; }

private:
	bool compare(int i, int j)
	{
		return comp(keys[pq[i]], keys[pq[j]]);
	}

	void exch(int i, int j)
	{
		//����λ�õ�index����
		std::swap(pq[i], pq[j]);
		std::swap(qp[pq[i]], qp[pq[j]]);
	}

	void swim(int k)
	{
		while (k > 1 && compare(k >> 1, k))
		{
			exch(k, k >> 1);
			k >>= 1;
		}
	}

	void sink(int k)
	{
		while ((k << 1) <= N)
		{
			int j = k << 1;
			if (j < N && compare(j, j + 1)) ++j;
			if (!compare(k, j))break;
			exch(k, j);
			k = j;
		}
	}
};