/**************************************************************************************************************************
**********************************************��ϰ��************************************************************************
**************************************************************************************************************************/
#include"sort.h"
#include"../Chapter1/test.h"
//2.3.5:ֻ����������ֵ���������򣬿���ģ��3-way����
//�����ǵݹ���ȶ���һ�����
template<typename T>
void sort2distinct(std::vector<T>& a)
{
	int lt = 0, gt = a.size() - 1;
	//[0,lt-1],[lt,i-1][i,gt],[gt+1,hi]
	int i = lt+1;
	T v = a[0];
	while (i <= gt)
	{
		if (a[i] < v)
			std::swap(a[i++], a[lt++]);//ΪʲôҪ�����أ�����֮ǰ�Ķ��壬lt�ǵ���v�ģ����ԣ�Ҫ�ڳ��ռ�����v
		else if (v < a[i])
			std::swap(a[i], a[gt--]);
		else
			++i;
	}
	//��֮ǰ��quick sort�У�i��j����ʽ��ȣ����˺ܶ಻��Ҫ�Ľ�����������Ϊ�����޷�Ԥ�⣬������ĳ��ȣ�ֻ�����
	//�ɴˣ�����Ϊû���ظ���������£����Ƕ��ַ��ð�
}


//2.3.15:��˿����ñ
//���ݿ���������ȡ��˿KΪ�з�Ԫ�أ��з���ñ��ͬʱ�ҳ�����˿Kƥ�����ñK'��
//������ñK'�з���˿
//�ظ���������


/**
 *2.3.16:��������������ɶ���sort��˵��������ѵ�����
 *���ȿ����ܲ���ʹ�÷��η�
 *���ǰ��һ������õģ�����һ������õģ���ô�ܲ���->��������õģ�
 *��������ֳ���������õģ��ܲ���ÿ�����ֶ�����õģ�
 *���ε�����˼·
 */
void __best(std::vector<int>& a, int lo, int hi)
{
	if (hi <= lo) return;
	int mid = lo + (hi - lo)/2;//���Խ�����һ��Ϊ�����з�Ԫ��
	__best(a, lo, mid - 1);
	__best(a, mid + 1, hi);
	std::swap(a[lo], a[mid]);
}
std::vector<int> best(int N)
{
	std::vector<int> ret(N);
	for (int i = 0; i < N; ++i)
		ret[i] = i;
	__best(ret, 0, N - 1);
	return ret;
}


/**
 *2.3.17 �ڱ�������ȥ�߽���
 *����һ�²���������ڱ�������СԪ�طŵ���߽�
 *����Ҳ��ͨ���ڱ�Ԫ�������߽�
 *���߽����Ƕ���ģ������з�Ԫ�ر������һ���ڱ���v������С��a[lo]��
 *��������Ҳ��ڱ���
 */
/**
 *����Ψһ�ĸĶ����ǰ����Ԫ�ط��õ�������������Ҳ࣬�����Ϳ��Ա�֤i����Խ��
 *��ô�����������أ����ұߵ�������϶���û�������
 *���������Ҳ��ڱ���ʲô�أ�������ԭַ����,����֮ǰ��������Ļ����������˺ô�
 *���ǿ��Է��ʳ���������߽��Ԫ��
 *�����������飬������һ��Ԫ�ر�Ȼ�Ǵ������������е�����Ԫ�صģ����ǿ���ʹ����һ��������Ϊ�ڱ�
 *�����ǣ��������ڱ���ȷ��
 *1.�᲻��ı䡰�����߽���Ҳ��ڱ�����
 *��i==hi+1ʱ����Ȼ��i>=j,���Բ������
 *��i==hi+1��j==loʱ����Ȼ��Ҫ�˳���ѭ���ģ��������ǿ�������ѭ����ʹ��while (*++i < v)��������䣬��ΪֻҪi==lo�ˣ���Ȼ��������ѭ��
 *�����õ��ģ���������ѭ�����ٴ�����i
 */
template<typename It>
class __QuickWithSentry
{
public:
	void sort(It beg, It end)
	{
		static Random r;
		r.shuffle(beg, end);//����������������������

		//�����Ԫ���Ƶ����Ҳ࣬��Ϊ�ڱ�
		auto max = beg;
		for (auto it = beg + 1; it != end; ++it)
		{
			if (*max < *it)
				max = it;
		}
		std::swap(*max, *(end - 1));

		__sort(beg, end - 1);
	}
private:
	void __sort(It beg, It back)
	{
		if (back - beg <= 0) return;
		auto j = partition(beg, back);
		if (j != beg)
			__sort(beg, j - 1);
		if (j != back)
			__sort(j + 1, back);
	}


	It partition(It beg, It back)
	{
		It i = beg;
		It j = back + 1;
		auto v = *beg;

		while (true)
		{//ɨ��Ԫ��
			while (*++i < v);
			while (v < *--j);
			//��ָ��i��j����ʱ�˳���ѭ��
			if (i - j >= 0) break;

			std::swap(*i, *j);
		}
		//��*j������v����*i��С��v������184ҳͼ��
		std::swap(*beg, *j);
		return j;
	}

};

template<typename It>
void quickSortWithSentry(It beg, It end)
{
	static __QuickWithSentry<It> q;//static��Ϊ��random
	q.sort(beg, end);
}


/**
 *2.3.18 ��ȡ���з֣���ȡ���з�
 *֮����STLԴ����ʵ��
 *Ѱ�ҵ�K���Ԫ�أ����ÿ��Խ�����quicksort�㷨����Ϊ���㷨���ÿ���Ѱ�ҵ�K��Ԫ��
 */

/**
 *2.3.20���ǵݹ�Ŀ�������
 *�ұ������뷨�ǣ������ȱ���û����������䣿Ϊʲô�أ���һ��ջ��ִ�й���
 *��������и���
 *Ҳ����ʹ����һ�֣����Ϲ�Ϊ�������㷨ʵ��
 */
#include"../Chapter1/stack.h"
template<typename It>
class __QuickNonrecursive
{
public:
	void sort(It beg, It end)
	{
		static Random r;
		//r.shuffle(beg, end);//����������������������
		__sort(beg, end - 1);
	}
private:
	void __sort(It beg, It back)
	{
		Stack<It> stack;
		
		if (back - beg <= 0) return;
		stack.push(beg);
		stack.push(back);
		
		//��ʼִ�У�ջ�е����鶼û������
		while (!stack.isEmpty())
		{
			auto hi = stack.pop();
			auto lo = stack.pop();

			if (hi <= lo) continue;

			auto mid = partition(lo, hi);
			if (lo < mid - 1)
			{
				stack.push(lo);
				stack.push(mid - 1);
			}

			if (mid + 1 < hi)
			{
				stack.push(mid + 1);
				stack.push(hi);
			}
		}
	}


	It partition(It beg, It back)
	{
		It i = beg;
		It j = back + 1;//����ɨ��ָ��,ע�������ĵ����ݼ���ͳһ
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
			if (i - j >= 0) break;

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
void quickSortNonrecursive(It beg, It end)
{
	static __QuickNonrecursive<It> q;//static��Ϊ��random
	q.sort(beg, end);
}



#include<iostream>
double time2_3(const std::string& alg, std::vector<double>& v)
{
	Stopwatch timer;
	auto beg = v.begin();
	auto end = v.end();
	if (alg == "Merge") mergeSort(beg, end);
	else if (alg == "MergeX") mergeSortX(beg, end);
	else if (alg == "InsertionX") insertionSortX(beg, end);
	else if (alg == "Quick") quickSort(beg, end);
	else if (alg == "Quick3Way") quick3WaySort(beg, end);
	else if (alg == "QuickNon") quickSortNonrecursive(beg, end);
	else if (alg == "heap") heapSort(beg, end);
	
	

	return timer.elapsedTime();
}

//int main()
//{
//	int N = 10000;
//	int T = 100;
//	double t1 = timeRondomInput(time2_3, "MergeX", N, T);
//	double t2 = timeRondomInput(time2_3, "heap", N, T);
//	double t3 = timeRondomInput(time2_3, "Quick", N, T);
//	double t4 = timeRondomInput(time2_3, "Quick3Way", N, T);
//	double t5 = timeRondomInput(time2_3, "QuickNon", N, T);
//	std::cout << t1 << '\t' << t2 << '\t' << t1 / t2 << '\n';
//	std::cout << t2 << '\t' << t3 << '\t' << t2 / t3 << '\n';
//	std::cout << t2 << '\t' << t4 << '\t' << t2 / t4 << '\n';
//	std::cout << t2 << '\t' << t5 << '\t' << t2 / t5 << '\n';
//	system("pause");
//}
