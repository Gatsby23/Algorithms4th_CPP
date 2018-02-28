#include<vector>
#include<algorithm>
#include<type_traits>
#include<iostream>
#include"sort.h"
#include"../Chapter1/test.h"


/***************************************************************************************************************************
**************************************************��ϰ��********************************************************************
**************************************************************************************************************************/

/**
 *2.2.12 �����ԵĶ���ռ䣺
 *���Բμ���ݡ�STLԴ����������merge���ڿռ�Ĵ���
 *http://blog.csdn.net/shoulinjun/article/details/20702681
 */

/**
 *2.2.14 & 2.2.15 �Ե����ϵ������������
 */
#include"../Chapter1/queue.h"
template<typename It, typename T>
class __MergeSortQueue
{
private:
	Queue<Queue<T>> queues;
public:
	__MergeSortQueue(It beg, It end)
	{
		for (auto it = beg; it != end; ++it)
		{
			Queue<T> q;
			q.enqueue(*it);
			queues.enqueue(std::move(q));
		}

		while (queues.size() > 1)
		{//�ö�����ʵ���Ե����ϵ��߼�
			auto first = queues.dequeue();
			auto second = queues.dequeue();
			queues.enqueue(merge(first, second));
		}
		auto q = queues.dequeue();//Ӧ�����ƶ�����

		for (auto it = beg; it != end; ++it)
			*it = q.dequeue();
	}

private:

	Queue<T> merge(Queue<T>& first, Queue<T>& second)
	{
		Queue<T> ret;
		auto left = first.dequeue();
		auto right = second.dequeue();
		while(ret.size()!=first.size()+second.size())
		{
			if (first.isEmpty()) ret.enqueue(second.dequeue());
			else if (second.isEmpty()) ret.enqueue(first.dequeue());
			else if (left <= right)
			{//<=Ϊ�ȶ�����
				ret.enqueue(left);
				left = first.dequeue();
			}
			else
			{
				ret.enqueue(right);
				right = second.dequeue();
			}
		}
		return ret;
	}	
};


/**
 *2.2.16 ��Ȼ�Ĺ鲢����------�������򷽷�
 */
template<typename It, typename T>
class __MergeNature
{
private:
	std::vector<T> aux;
public:
	__MergeNature(It beg, It end) :aux(beg, end)
	{
		int N = end - beg;

		while (true)//��ֹ������ֻʣһ����������:��ֹ�����Ƶ�����ѭ����
		{		
			int firstlo = 0, firsthi = 0;
			int secondhi = -1;
			int tmphi = 0;

			while (tmphi<N-1)
			{
				//Ѱ�ҵ�һ����������
				tmphi = firstlo = secondhi + 1;
				while ((tmphi!=N-1)&&*(beg + tmphi) <= *(beg + tmphi + 1)) //�б߽���
					++tmphi;
				firsthi = tmphi;
				//���
				if (firsthi - firstlo + 1 == N) return;
				if (firsthi == N - 1) break;

				//Ѱ�ҵڶ�����������
				tmphi = firsthi + 1;
				while ((tmphi != N - 1) && *(beg + tmphi) <= *(beg + tmphi + 1)) //�б߽���
					++tmphi;
				secondhi = tmphi;

				merge(beg+firstlo, beg+firsthi, beg+secondhi);
			}
		}
	}

private:

	void merge(It beg, It mid, It back)
	{
		int lsize = (mid - beg) + 1;
		int rsize = (back - mid);

		for (int k = 0; k < lsize; ++k)
			aux[k] = *(beg + k);
		for (int k = 0; k < rsize; ++k)
			aux[lsize + rsize - 1 - k] = *(mid +1 + k);

		int i = 0, j = (back - beg);
		for (int k = 0; k < lsize + rsize; ++k)
		{
			if (aux[i] <= aux[j]) 
				*(beg + k) = aux[i++];
			else 
				*(beg + k) = aux[j--];
		}
	}

};

template<typename It>
void mergeSortNature(It beg, It end)
{
	__MergeNature<It, typename std::remove_reference<decltype(*beg)>::type>(beg, end);
}

/**
 *2.2.17������������Ȼ�鲢�������������ѷ���
 *������2.2.16������ʵ��
 */

/**
 *2.2.18:�ص�����Merge�����У�next=random��first��second��
 *����ע���ǡ����ҡ����Σ�һ��Ҫע���߼��ϵ�����
 */

/**
 *2.2.19�����ã�
 *�ܷ���Σ����ҷֿ�֮��ϲ�����һ���ı����ԣ����Բŷ���
 *�ı���ԭʼ��mergesort����
 */
template<typename It, typename T>
class __Inversion
{
private:
	std::vector<T> aux;

public:
	__Inversion(It beg, It end) :aux(beg, end)
	{
	}

	int count(It beg, It back)
	{
		if (back <= beg)return 0;
		int ret = 0;
		It mid = beg + (back - beg) / 2;
		ret += count(beg, mid);
		ret += count(mid + 1, back);
		ret += merge(beg, mid, back);
		return ret;
	}

private:

	int merge(It beg, It mid, It back)
	{
		int ret = 0;

		for (int k = 0; k <= (back - beg); ++k)
			aux[k] = *(beg + k);
		
		int i = 0, j = (mid - beg) + 1;
		for (int k = 0; k <= (back - beg); ++k)
		{
			if (i > (mid - beg)) *(beg + k) = aux[j++];
			else if (j > (back - beg)) *(beg + k) = aux[i++];
			else if (aux[i] <= aux[j]) *(beg + k) = aux[i++];
			else {
				*(beg + k) = aux[j++];
				ret += (mid -beg- i + 1);
			}
		}

		return ret;
	}
};


/**
 *2.2.20:�������
 *���������ԭ����һ�£�ֻ��Ҫ�ڷ������ݵ�ʱ�򣬽����ü���
 */
template<typename It, typename T>
class __IndexMerge
{
private:
	std::vector<int> aux;
	std::vector<int> index;
	std::vector<T> data;
public:
	__IndexMerge(It beg, It end) :data(beg, end)
	{
		for (int i = 0; i < data.size(); ++i)
		{
			index.push_back(i);
			aux.push_back(i);
		}
		sort(0, index.size()-1);//����index����
	}

private:
	void sort(int lo, int hi)
	{
		if (hi <= lo) return;
		int mid = lo + (hi - lo) / 2;
		sort(lo, mid);
		sort(mid + 1, hi);
		merge(lo, mid, hi);
	}

	void merge(int lo, int mid, int hi)
	{
		for (int k = lo; k <= hi; ++k)
			aux[k] = index[k];

		int i = lo, j = mid+1;
		for (int k = lo; k <= hi; ++k)
		{
			if (i > mid) index[k] = aux[j++];
			else if (j > hi) index[k] = aux[i++];
			else if (data[aux[i]] <= data[aux[j]]) index[k] = aux[i++];
			else index[k] = aux[j++];
		}
	}
};

/**
 *2.2.22:��·�鲢������K·�鲢���򣬳��˷���֮��û��ʲôֵ����ȥʵ�ֵĵط�
 *Merge������Ҫע��ĵط��ǣ�
 *���ǵ���ѭ���ǣ�for (int k = lo; k <= hi; ++k){}��
 *Զ��while(i<=mid && j<=hi) + while(i<=mid) +while(j<=hi)���������ŵö�
 *��Ϊǰһ����ѭ���Ǵ�������ǣ�������ô������������N��Ԫ�أ�ֻѭ��N�ξͺ�
 *��һ���Ǵ����뿼�ǡ�
 *һ��Ҫ��ϸ���������ֱ�ʾ����
 */
template<typename It, typename T>
class __KMerge
{
private:
	std::vector<T> aux;
	std::vector<T> data;
	int K;
public:
	__KMerge(It beg, It end, int k) :data(beg, end), aux(beg, end), K(k)
	{
		sort(0, data.size() - 1);

		auto it2 = data.begin();
		for (auto it = beg; it != end; ++it, ++it2)
			*it = *it2;
	}

private:
	void sort(int lo, int hi)
	{
		if (hi <= lo + K)
		{
			insertionSortHalfChange(lo, hi);
			return;
		}

		std::vector<int> cutpoints(K + 1);
		//�ֳ�k��,��k+1�����ʾ[lo,mid1,mid2...hi]
		for (int i = 0; i < K; ++i)
		{
			cutpoints[i] = lo + i*(hi - lo) / K;
		}
		cutpoints[K] = hi;

		//�ֶ�sort
		sort(lo, cutpoints[1]);//[lo,mid]
		for (int i = 1; i < K; ++i)
		{
			sort(cutpoints[i] + 1, cutpoints[i + 1]);
		}
		//merge
		merge(cutpoints);
	}

	void merge(std::vector<int>& points)
	{
		//int lo = points[0];
		//int hi = points.back();
		for (int k = points.front(); k <= points.back(); ++k)
			aux[k] = data[k];

		std::vector<int> cur(K);
		cur[0] = points[0];
		for (int i = 1; i < K; ++i)
			cur[i] = points[i] + 1;

		//
		for (int k = points.front(); k <= points.back(); ++k)
		{
			//ѡȡһ���Σ����䣩t��cur[t]Ϊ�鲢ʱ��СԪ��
			int t=-1;//��tΪ��ʱ����ʾδ��ʼ��
			for (int i = 0; i < K; ++i)
			{
				if (cur[i] > points[i + 1])//�������Ѿ�ȡ��
					continue;
				else if (t == -1)
					t = i;
				else if (aux[cur[i]] < aux[cur[t]])
					t = i;
			}
			data[k] = aux[cur[t]];
			++cur[t];
		}

	}

	void insertionSortHalfChange(int lo, int hi)
	{


		//����Ҫ�����Ĳ�������,����ֵ��������
		for (int i = lo+1; i <= hi; ++i)
		{ //��a[i]���뵽a[0...i-1]֮��
			auto t = data[i];
			int j = i;
			for (; (j>lo)&&t < data[j-1]; --j)
				data[j] = data[j-1];

			data[j] = t;
		}
	}
};
template<typename It>
void KMergeSort(It beg, It end, int k)
{
	__KMerge<It, typename std::remove_reference<decltype(*beg)>::type>(beg, end,k);
}


/**
 *2.2.23 �Ƚ�ԭʼ�㷨��Ľ��㷨������ 
 */

double time2_2(const std::string& alg, std::vector<double>& v)
{
	Stopwatch timer;
	auto beg = v.begin();
	auto end = v.end();
	if (alg == "Merge") mergeSort(beg, end);
	else if (alg == "MergeX") mergeSortX(beg, end);
	else if (alg == "2Merge") KMergeSort(beg, end, 2);
	else if (alg == "3Merge") KMergeSort(beg, end, 3);
	else if (alg == "4Merge") KMergeSort(beg, end, 4);
	else if (alg == "5Merge") KMergeSort(beg, end, 5);
	else if (alg == "6Merge") KMergeSort(beg, end, 6);
	else if (alg == "7Merge") KMergeSort(beg, end, 7);
	else if (alg == "8Merge") KMergeSort(beg, end, 8);
	else if (alg == "Insertion") insertionSort(beg, end);
	else if (alg == "Selection") selectionSort(beg, end);
	else if (alg == "Shell") shellSort(beg, end);
	else if (alg == "InsertionX") insertionSortX(beg, end);

	return timer.elapsedTime();
}

//int main()
//{
//	int N = 1000;
//	int T = 100;
//	double t1 = timeRondomInput(time2_2, "Merge", N, T);
//	double t2 = timeRondomInput(time2_2, "MergeX", N, T);
//	double t3 = timeRondomInput(time2_2, "InsertionX", N, T);
//	double t4 = timeRondomInput(time2_2, "Shell", N, T);
//	std::cout << t1 << '\t' << t2 << '\t' << t1 / t2 << '\n';
//	std::cout << t2 << '\t' << t3 << '\t' << t2 / t3 << '\n';
//	std::cout << t2 << '\t' << t4 << '\t' << t2 / t4 << '\n';
//	system("pause");
//}



/**
 *2.2.25 ȷ�����Kֵ
 *�˵��ԵĲ��Խ���ǣ�3��4,5Ч���Ϻ�
 */
