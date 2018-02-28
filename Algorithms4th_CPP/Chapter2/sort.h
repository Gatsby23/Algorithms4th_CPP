#pragma once
#include<type_traits>
#include"../Chapter1/random.h"
//ʵ����õļ��������㷨��ʹ�õ�������Ϊ�ӿ�
using std::iterator_traits;
/**
 *2.1---ѡ������ԭַ���򡢲��ȶ���58528Ϊ����
 *
 *���������ڳ���ΪN�����飬ѡ��������Ҫ��ԼN^2/2�αȽϺ�N�ν���
 *����ͨ������켣��֤����һ��
 *
 *�ɴ����ǿ��Եõ����½��ۣ�
 *1.����ʱ���������޹�
 *2.���ݵ��ƶ����������ٵ�
 */

template<typename BidirectionalIt>
void selectionSort(BidirectionalIt first, BidirectionalIt last)
{
	typedef typename iterator_traits<BidirectionalIt>::value_type value_type;

	if (first == last) return;
	for (auto i = first; i != last; ++i)
	{//��a[i]��a[i+1..N)�е���СԪ�ؽ���
		auto min = i;
		for (auto j = i + 1; j != last; ++j)
		{
			if (*j < *min) min = j;
		}
		using std::swap;
		swap(*i, *min);
	}
}


/**
 *2.1----��������ԭַ�����ȶ���
 *
 *���������ڳ���ΪN�����飬ƽ������²���������Ҫ~N^2/4�αȽϺ�~N^2/4�ν�������������Ҫ~N^2/2�αȽϺ�~N^2/2�ν���
 *����������ҪN-1�αȽϺ�0�ν���
 *
 *����������Ҫ�Ľ�����������Ŀ�������е��õ���Ŀ��ͬ����Ҫ�ıȽ���Ŀ���ڵ�����Ŀ+��N-1��
 */
//ԭʼ����棺
template<typename BidirectionalIt>
void insertionSort(BidirectionalIt first, BidirectionalIt last)
{
	typedef typename iterator_traits<BidirectionalIt>::value_type value_type;

	if (first == last) return;
	for (auto i = first + 1; i != last; ++i)
	{//��a[i]���뵽a[0...i-1]֮��
		for (auto j = i; j > first && *j < *(j - 1); --j)
		{
			using std::swap;
			swap(*j, *(j - 1));
		}
	}
}

/**
 * ����2.1.24��������ڱ� & 2.1.25 ���ٽ���
 * �������������飬���insert���������
 *
 *�����ڱ�������СԪ�طŵ�����࣬���ܲ���select �ķ�������Ϊ��ᵼ���㷨���ȶ�
 */
template<typename BidirectionalIt>
void insertionSortX(BidirectionalIt first, BidirectionalIt last)
{
	typedef typename iterator_traits<BidirectionalIt>::value_type value_type;

	if (first == last) return;
	// put smallest element in position to serve as sentinel
	int exchanges = 0;
	for (auto i = last - 1; i != first; --i) 
	{
		if (*i < *(i-1))
		{
			using std::swap;
			std::swap(*i, *(i - 1));
			++exchanges;
		}
	}
	if (exchanges == 0) return;

	//����Ҫ�����Ĳ�������,����ֵ��������
	//insertion sort with half-exchanges
	for (auto i = (first + 1) + 1; i != last; ++i)
	{
		//��a[i]���뵽a[0...i-1]֮��
		auto t = *i;
		auto j = i;
		for (; t < *(j - 1); --j)
			*j = std::move(*(j - 1));

		*j = std::move(t);
	}
}


/**
 * 2.1---ϣ������
 * ϣ���������Ч��ԭ���ǣ���Ȩ����������Ĺ�ģ��������
 */
template<typename RandomAcessIt>
void shellSort(RandomAcessIt first, RandomAcessIt last)
{
	int N = (last - first);
	int h = 1;
	while (h < N / 3) h = 3 * h + 1;//1, 4, 13, 40, 121, 364, 1093, ...

	using std::swap;
	while (h >= 1)
	{ //��������h����
		for (int i = h; i < N; ++i)
		{//��a[i]���뵽a[i-h..i-2*h..]֮��
			auto t = *(first + i);
			auto j = first + i;
			for (; j>=(first+h) && t < *(j - h); j-=h)
				*j = std::move(*( j - h));
			*j = std::move(t);
		}
		h = h / 3;
	}
}

//�ж��Ƿ��Ѿ�����
template<typename It>
bool isSorted(It first, It last)
{
	auto prev = first;
	auto cur = first; ++cur;
	for (; cur != last; ++prev, ++cur)
	{
		if (*cur < *prev) return false;
	}
	return true;
}



//���ڲ��������㷨�����ܣ�
//���������㷨���ܲ���
template<typename Func>
double timeRondomInput(Func time, const std::string& alg, int N, int T)
{ //ʹ���㷨������ΪN����������T��
	double total = 0;
	std::vector<double> a(N);
	static Random r;
	for (int t = 0; t < T; ++t)
	{
		for (int i = 0; i < N; ++i)
			a[i] = r.uniform(0, 100);

		total += time(alg, a);
	}

	if (!isSorted(a.begin(), a.end()))
	{
		return 10000;
	}
	return total;
}

/**
 *2-2 �鲢����
 */

 /**
 *�鲢�������÷���˼�룬������ܽ���������������������ͨ���鲢��������������������������
 */

template<typename RandomAcessedIt>
class __Merge
{
	//typedef typename std::remove_reference<decltype(*beg)>::type value_type;
	typedef typename std::iterator_traits<RandomAcessedIt>::value_type value_type;

private:
	std::vector<value_type> aux;

public:
	__Merge(RandomAcessedIt first, RandomAcessedIt last) :aux(first, last)
	{
		sort(first, last - 1);
	}

private:
	void sort(RandomAcessedIt lo, RandomAcessedIt hi)
	{//������[lo..hi]����
		if (hi <= lo) return;
		auto mid = lo + (hi - lo) / 2;
		sort(lo, mid);
		sort(mid + 1, hi);
		merge(lo, mid, hi);
	}

	void merge(RandomAcessedIt lo , RandomAcessedIt mid, RandomAcessedIt hi)
	{//�鲢[lo mid] ��[mid+1 hi]
		auto i = lo, j = mid+1;
		for (int k = 0; k<=(hi-lo); ++k)
		{
			if (i > mid) aux[k] = std::move(*j++);
			else if (j > hi) aux[k] = std::move(*i++);
			else if (*i < *j) aux[k] = std::move(*i++);
			else aux[k] = std::move(*j++);
		}

	
		i = lo;
		for (int k = 0; k <= (hi - lo); ++k, ++i)
			*i = std::move(aux[k]);	
		_ASSERT(isSorted(lo, hi));
	}
};

template<typename RandomAcessedIt>
void mergeSort(RandomAcessedIt beg, RandomAcessedIt end)
{
	__Merge<RandomAcessedIt>(beg, end);
}

/**
*������
*���ڳ���ΪN���������飬�Զ����µĹ鲢������Ҫ1/2NlgN��NlgN�αȽ�
*�μ�����F���鲢����ΪN�����飬��ҪN/2��N�˱Ƚ�
*���ڳ���ΪN���������飬�Զ����µĹ鲢���������Ҫ6NlgN�η�������
*ÿ�ι鲢�������Ҫ��������6N�Σ�2N�������ƣ�2N������������õ�Ԫ���ƶ���ȥ��������N�αȽϣ�2N�η������飩��
*
*�������ֳ���lgN���������ۣ�Ҫһ����Ϊ����������
*/

/**
*�Ե����ϵĹ鲢����
*/
template<typename RandomAcessedIt>
class __MergeBU
{
	typedef typename std::iterator_traits<RandomAcessedIt>::value_type value_type;
private:
	std::vector<value_type> aux;

public:
	__MergeBU(RandomAcessedIt beg, RandomAcessedIt end) :aux(beg, end)
	{
		int N = end - beg;

		for (int sz = 1; sz < N; sz = sz + sz)//sz��������Ĵ�С
			for (int lo = 0; lo < N - sz; lo += sz + sz)
				merge(beg + lo, beg + lo + sz - 1, beg + std::min(lo + sz + sz - 1, N - 1));
	}

private:

	void merge(RandomAcessedIt lo, RandomAcessedIt mid, RandomAcessedIt hi);//ʹ��2.2.10�Ŀ��ٹ鲢
};

/**
*������
*���ڳ���ΪN���������飬�Ե����ϵĹ鲢������Ҫ1/2NlgN��NlgN�αȽϣ������Ҫ6NlgN���������
*������ĳ�����2����ʱ���Զ����º��Ե����ϵĹ鲢����Ƚϴ�����������ʴ���������ͬ��ֻ��˳��ͬ
*/
/**
*2.2.10:���ٹ鲢������ʡȥ��ѭ���м��ĳ����Ƿ��þ��Ĵ���
*���ڵڶ��ε��õĹ�ϵ���������ǲ��ȶ�����
*/
template<typename RandomAcessedIt>
void __MergeBU<RandomAcessedIt>::merge(RandomAcessedIt lo, RandomAcessedIt mid, RandomAcessedIt hi)
{
	int lsize = (mid - lo) + 1;
	int rsize = (hi - mid);

	for (int k = 0; k < lsize; ++k)
		aux[k] = std::move(*(lo + k));
	for (int k = 0; k < rsize; ++k)
		aux[lsize + rsize - 1 - k] = std::move(*(mid + k));

	auto i = aux.begin(), j = aux.begin() + (hi - lo);
	auto ret = lo;
	for (int k = 0; k < lsize + rsize; ++k, ++ret)
	{
		if (*i < *j) *ret = std::move(*i++);
		else *ret = std::move(*j--);
	}

}
/**
*2.2.8, 2.2.11, 2.2.10:�Ľ��鲢������㷨
*1.�ӿ�С����������ٶȣ�2.��������Ƿ��Ѿ�����3.ͨ���ڵݹ��н����������������鸴��
*����1��˵������CUTOFF=8��ʹ��mergesort������Ҫ���㣬ÿ����Ҫ6N��������ʣ��ܹ�18N����ʹ�ò���������ƽ��Ϊ1/4*��N/8)*8^2=N/2��Ч�ʸ���36��
*����2��˵���������ʱ����ģ���ôֻ��Ҫ����ʱ��
*����3��˵��ʡȥ���ƶ����ݵ�2N������Ч��������1/3
*/
template<typename RandomAcessedIt>
class __MergeX
{
	typedef typename std::iterator_traits<RandomAcessedIt>::value_type value_type;
private:
	std::vector<value_type> aux;
	int CUTOFF = 15;

public:
	__MergeX(RandomAcessedIt beg, RandomAcessedIt end) :aux(beg, end)
	{
		sort( aux.begin(), aux.end() - 1, beg, end - 1);
	}

private:
	void sort(RandomAcessedIt srclo, RandomAcessedIt srchi, RandomAcessedIt destlo, RandomAcessedIt desthi)
	{//������[srclo,srchi]����[destlo,desthi],���б��뿪ʼʱ�����������ӦԪ�����

	 //С�����ò�����������ٶ�
		if ((srchi - srclo) <= CUTOFF)
		{
			insertionSortX(destlo, desthi + 1);
			return;
		}

		//
		auto srcmid = srclo + (srchi - srclo) / 2;
		auto destmid = destlo + (desthi - destlo) / 2;

		sort(destlo, destmid, srclo, srcmid);
		sort(destmid + 1, desthi, srcmid + 1, srchi);

		//����Ƿ��Ѿ�����
		if (*srcmid <= *(srcmid + 1))
		{
			std::copy(srclo, srchi, destlo);
			return;
		}

		merge(srclo, srcmid, srchi, destlo, desthi);
	}

	void merge(RandomAcessedIt srclo, RandomAcessedIt srcmid, RandomAcessedIt srchi, RandomAcessedIt destlo, RandomAcessedIt desthi)
	{//��[srclo, srcmid,srchi]�鲢��[destlo,desthi],����source�Ѿ�����

		auto i = srclo, j = srcmid + 1;

		auto dest = destlo;
		for (int k = 0; k <= (srchi - srclo); ++k, ++dest)
		{
			if (i > srcmid) *dest = std::move(*j++);
			else if (j > srchi) *dest = std::move(*i++);
			else if (*i < *j) *dest = std::move(*i++);
			else *dest = std::move(*j++);
		}

	}
};

template<typename RandomAcessedIt>
void mergeSortX(RandomAcessedIt beg, RandomAcessedIt end)
{
	__MergeX<RandomAcessedIt>(beg, end);
}







/**
 *2.3 ��������
 *
 */

 ////�������� VS �鲢����
 ////���߻������鲢�����ǽ�����ֳ�����������ֱ����򣬲��������������鲢�Խ�������������->���鴦�����ڵݹ�֮��
 ////���������ǵ����������鶼�����ʱ������������Ҳ����Ȼ������->���鴦�����ڵݹ�֮ǰ
 template<typename It>
 class __Quick
 {
 
 public:
 	void sort(It beg, It end)
 	{
 		static Random r;
 		r.shuffle(beg, end);//����������������������
 		__sort(beg, end-1);
 	}
 private:
 	void __sort(It beg, It back)
 	{
 		if (back-beg <= 0) return;
 		auto j = partition(beg, back);
 		if (j != beg)
 			__sort(beg, j - 1);
 		if (j != back)
 			__sort(j + 1, back);
 	}
 
 	/**
 	 *ע��˼���
 	 *while(true){while(something){if(something) return/break;} 
 	 *ע��˫��Ƕ��ѭ������ֹ�����Ͱ���
 	 */
 
 	It partition(It beg, It back)
 	{
 		It i = beg;
 		It j = back+1;//����ɨ��ָ��,ע�������ĵ����ݼ���ͳһ
 		auto v = *beg;//�з�Ԫ��
 		
 		//ѭ������ʽ�ǣ�i����Ԫ�ض�������v��j�Ҳ��Ԫ�ض���С��v
 		while (true)
 		{//ɨ��Ԫ��
 			while (*++i < v)
 			{
 				if (i == back) break;
 			}
 			while (v < *--j)
 			{
 				if (j == beg) break;
 			}
 			//��ָ��i��j����ʱ�˳���ѭ��
 			if (i-j >= 0) break;
 			
			using std::swap;
 			swap(*i, *j);
 		}
 		//��*j������v����*i��С��v������184ҳͼ��
		using std::swap;
 		swap(*beg, *j);
 		return j;
 	}
 
 };
 
 template<typename It>
 void quickSort(It beg, It end)
 {
 	static __Quick<It> q;//static��Ϊ��random
 	q.sort(beg, end);
 }



 
 /**
  *���Խ��еĸĽ���
  *1.�ʹ󲿷ֵݹ����һ��������С���飬��������Ȳ�����������
  *2.��ȡ���з֣�ʹ���������һС����Ԫ�ص���λ�����з�����
  *3.�����ŵ��������з�
  */
 template<typename It>
 class __Quick3Way
 {
 public:
 	void sort(It beg, It end)
 	{
 		static Random r;
 		r.shuffle(beg, end);//����������������������
 		__sort(beg, end - 1);
 	}
 private:
 	void __sort(It beg, It back)
 	{
 		if (back <= beg) return;
 		It lt = beg, i = beg + 1, gt = back;
 		auto v = *beg;
 		//[beg,lt-1]��Ԫ��С��v,[lt..i-1]��Ԫ�ص���v��[gt+1,back]��Ԫ�ش���v������[i,gt]��Ԫ��
 		//ע������ѭ��������ʽ���������ֵĲ�ͬ
 		while (i <= gt)
 		{
			using std::swap;
 			if (*i < v) swap(*lt++, *i++);
 			else if (*i>v) swap(*i, *gt--);
 			else  ++i;
 		}
 
 		__sort(beg, lt - 1);
 		__sort(gt + 1, back);
 	}
 
 };
 
 template<typename It>
 void quick3WaySort(It beg, It end)
 {
 	static __Quick3Way<It> q;//static��Ϊ��random
 	q.sort(beg, end);
 }



/**
 *2.4 ������
 */

#include"pq.h"

 template<typename InputIt>
 void heapSort(InputIt beg, InputIt end)
 {
	 typedef typename std::iterator_traits<InputIt>::value_type value_type;
	 PQ<value_type, std::greater<value_type>> pq(beg,end);
	 auto i = beg;
	 while (!pq.isEmpty())
	 {
		 *i++ = pq.pop();
	 }
 }
 








