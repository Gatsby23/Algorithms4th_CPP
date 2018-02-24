#include"random.h"
#include"test.h"
#include<iostream>
#include<vector>

/**
 *threeSum: count triples that sum to 0
 */
template<typename It>
size_t threeSum(It beg, It end)
{
	int N = (end - beg);
	size_t cnt = 0;
	for (int i = 0; i < N; ++i)
		for (int j = i + 1; j < N; ++j)
			for (int k = j + 1; k < N; ++k)
				if (*(beg+i) + *(beg+j) + *(beg+k) == 0)
					++cnt;
	return cnt;
}

double timeTrial(int N)
{
	static Random r;
	int MAX = 1000000;
	std::vector<int> v;
	for (int i = 0; i < N; ++i)
		v.push_back(r.uniformInt(-MAX, MAX));

	Stopwatch timer;
	size_t cnt = threeSum(v.begin(), v.end());
	return timer.elapsedTime();	
}


/**
 *���ֲ���,����key��rank
 *��ȻҪ�����������
 */
template<typename T,typename It>
size_t binarySearch(It beg, It end, T key) 
{
	int lo = 0;
	int hi = (end - beg) - 1;
	while (lo <= hi)
	{
		int mid = lo + (hi - lo) / 2;
		if (key < *(beg + mid)) hi = mid - 1;
		else if (key>*(beg + mid)) lo = mid + 1;
		else return mid;
	}

	//�Ҳ���������-1
	return -1;
}

/**
 *�����˶�2-sum:
 */
template<typename It>
size_t twoSumFast(It beg, It end)
{
	//������Ϊ��ʹ��binary search
	std::sort(beg, end);

	int N = (end - beg);
	int cnt = 0;
	/**
	 *����if������һ����ͣ��ȼ���binarySearch(-a[i],a)>i
	 *������ֲ����Ҳ����򷵻�-1.���ǲ������Ӽ�������ֵ
	 *������ֲ��ҷ��ص���j>i,��a[j]+a[i]=0�����Ӽ�������ֵ
	 *������ֲ����ҵ���j��0��i֮�䣬������ظ�����
	 */
	for (int i = 0; i < N; ++i)
		if (binarySearch(beg + i + 1, end,-*(beg+i)))
			++cnt;
	return cnt;
}

template<typename It>
size_t threeSumFast(It beg, It end)
{
	std::sort(beg, end);

	int N = (end - beg);
	int cnt = 0;
	for (int i = 0; i < N; ++i)
		for (int j = i + 1; j < N; ++j)
			if (binarySearch(beg + j + 1, end, -*(beg + i) - *(beg + j)))
				++cnt;
	return cnt;
}
/**************************************************************************************************************************
 ************************************************��ϰ��********************************************************************
 *************************************************************************************************************************/
//1.4.2:int�����������
//��x��y���Ǹ���������ǷǸ���������0������ô���Ǹ���������x��y���ǷǸ���������Ǹ�������ô�����������

//1.4.10
//���غͲ��ҵļ�ƥ���������С��Ԫ��
template<typename T, typename It>
size_t _binarySearch(It beg, It end, T key)
{
	int lo = 0;
	int hi = (end - beg) - 1;
	while (lo <= hi)
	{
		int mid = lo + (hi - lo) / 2;
		if (key < *(beg + mid)) hi = mid - 1;
		else if (key>*(beg + mid)) lo = mid + 1;
		else if (key == *(beg + mid) && mid != 0 && *(beg+mid-1)==key) hi = mid;//���û�����һ����������ô����������ͷ������ѭ�� 
		else return mid;
	}

	//�Ҳ���������-1
	return -1;
}

//1.4.12��Ӧ����Merge

/**
 *1.4.15��ʹ�����Լ�����㷨twoSum����������������к�Ϊ0��������
 *����ÿ������ֻ��ʹ��һ�Σ�-2,2,2ֻ��һ��������
 */
template<typename It>
size_t twoSumFaster(It beg, It end)
{
	//������

	int N = (end - beg);
	int cnt = 0;

	auto left = beg;
	auto right = end - 1;
	while (left < right)
	{
		if (*right>-*left) --right;
		else if (*right < -*left) ++left;
		else
		{
			++cnt;
			++left;
			--right;
		}
	}
	return cnt;
}

/**
 *1.4.16��1.4.17����ӽ���һ�Ե�ֵ����ңԶ��һ�Ե�ֵ
 */

std::pair<double, double> distance(std::vector<double>& v)
{
	//����
	std::sort(v.begin(), v.end());
	double min = INT_MAX, max = -INT_MAX;
	for (int i = 0; i < v.size() - 1; ++i)
	{
		if (v[i + 1] - v[i] < min)
			min = v[i + 1] - v[i];

		if (v[i + 1] - v[i] > max)
			max = v[i + 1] - v[i];
	}
	return{ min,max };
}

/**
 *1.4.18 & 1.4.19 :��������ľֲ���Сֵ
 *����һά���飬�����±Ƚϴ�����~2lgN
 *���������м�ֵa[N/2]�Լ����ڵ�Ԫ��a[N/2-1] & a[N/2+1]�����a[N/2]��һ���ֲ���Сֵ���㷨��ֹ�������ڽ�С���ڱ��м�������-->lgN
 *�����Ȼ�鲻�������ѯ������-->lgN
 *���ڶ�ά���飺�����£���N�����ȡ�
 *���ȵ�N/2���ϵ���Сֵa[N/2][j]���Ƚ� a[N/2-1][j] &a[N/2+1][j]. 
 *Recur in the half with the smaller neighbor. 
 *In that half, find the minimum entry in column N/2.
 *N+N/2+N/4...~N
 */


/**
 *1.4.20:˫�����ң���һ���������ֵ���ڶ����������ң����������Ҳ���ң��Ƚϴ���~3lgN
 */

template<typename T, typename It>
bool searchOfBitonic(It beg, It end, T key)
{
	//find max elements
	//�����Ȼ���ڣ����Բ��ÿ��Ǳ�Ե�ļ��
	int lo = 0;
	int hi = (end - beg) - 1;
	int mid = 0;
	while (lo <= hi)
	{
		mid = lo + (hi - lo) / 2;
		if (*(beg + mid) < *(beg + mid + 1)) lo = mid + 1;
		else if (*(beg + mid + 1) < *(beg + mid)) hi = mid - 1;
		else break;
	}

	int ret = -1;
	if ((ret = binarySearch(beg, beg + mid + 1, key)) != -1)
		return true;
	if ((ret = binarySearch(beg + mid + 1, end, key)) != -1)
		return true;

	return false;
}

/**
 *���üӼ�����ʵ�ֶ��ֲ��ң�Mihai Patrascu
 *ʹ�� Fibonacci numbers��������ַ�
 *��������û�кô��أ�ֻ�������üӼ����������˳���2�Ĳ������Ѱ�����ȫ������<<������ʵ�ֳ���2����
 *������ص��Ƿ���Fibnocci��power of 2�������ԱȽ��ϣ������������Fibnocci���ıȽ�
 *�ο���http://blog.csdn.net/zsw12013/article/details/50003505
 */
template<typename T, typename It>
int fibnocciSearch(It beg, It end, T key)
{
	//����
	std::vector<decltype(*It)> v(beg, end);
	std::sort(v.begin(), v.end());

	int N = v.size();
	//����ȷ����Ҫ�õ���Fibnocci number
	std::vector<int> f{ 1,1 };
	while (f.back() < N)
		f.push_back(f[f.size() - 1] + f[f.size() - 2]);

	//��������
	for (int i = v.size(); v < f.back(); ++i)
		v.push_back((T)INT_MAX);//���Ԫ��

	//��Ϊ����ʹ�õ������飬���Ա���ʱ�̼�ס�������Ĳ��
	//����ʹ��k������ǰҪ������Χ��Fibnocci ��������
	int k = f.size()-1;

	int lo = 0, hi = v.size() - 1;
	while (lo <= hi)
	{
		//��Ӧ��midԪ��:
		mid = lo + f[k - 1] -1;
		if (v[mid] < key)
		{
			lo = mid + 1;
			//��ʱ�Ĳ��ҳ���Ϊf[k-2]
			k -= 2;
		}
		else if (v[mid]>key)
		{
			hi = mid;
			//��ʱ�Ĳ��ҳ���Ϊf[k-1]
			k -= 1;
		}
		else
		{
			if (mid > N - 1) return -1;
			return mid;
		}
	}

	return -1;
}

/**
 *1.4.25:�Ӽ������⣺N���¥����༦��������F����һ���ٽ�㣬����ҵ�F��
 *lgN�εĻ��������ö��ַ����ҡ�
 *�ܲ��ܸĽ��أ�
 *����֪���������͵Ļ������������飬����1,2,4,8....�������ҵ�F������[c,2c]
 *���䳤��С��F���ڴ�������ֲ���F���ܹ�~2lgF�β���
 *
 *������������ֻ�������������������Ӧ�ڣ��������б�δ���еĳɱ�С�Ķ�����
 *��β��ң�
 *�϶���Ҫһ��һ���ʵ��Ĺ��̡�һ���鼦������С��Χ����һ��������һ������
 *~2sqrt��N)�Ļ���
 *To achieve 2 * sqrt(N), drop eggs at floors sqrt(N), 2 * sqrt(N), 3 * sqrt(N), ..., sqrt(N) * sqrt(N). 
 *(For simplicity, we assume here that sqrt(N) is an integer.) 
 *Let assume that the egg broke at level k * sqrt(N).
 *With the second egg you should then perform a linear search in the interval (k-1) * sqrt(N) to k * sqrt(N). 
 *In total you will be able to find the floor F in at most 2 * sqrt(N) trials.
 *���ʵ��~csqrt��F�����أ�
 *http://www.cnblogs.com/ltang/archive/2010/11/23/1885791.html
 */


