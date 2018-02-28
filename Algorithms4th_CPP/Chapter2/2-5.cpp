
/**
 *֮ǰһֱ��˵�ģ����ÿ�������Ѱ�ҵ�kС��Ԫ�صľ���ʵ������
 *��ƽ��ʱ�临�Ӷ�Ϊ���ԣ�����֤����221ҳ
 */
template<typename It>
class Select
{
public:
	Select(It beg, It end, int k)
	{
		//�������������
		int lo = 0, hi = (end - beg) - 1;
		while (hi > lo)
		{
			It j_it = partition(beg + lo, beg + hi);
			int j = j_it - beg;
			if (j == k) return *(beg + j);
			else if (j > k) hi = j - 1;
			else if (j < k) lo = j + 1;
		}

		//return *(beg + k);
	}

private:

	It partition(It beg, It back)
	{
		It i = beg;
		It j = back + 1;//����ɨ��ָ��,ע�������ĵ����ݼ���ͳһ
		auto v = *beg;//�з�Ԫ��


		while (true)
		{
			while (*++i < v)
			{
				if (i == back) break;
			}
			while (v < *--j)
			{
				if (j == beg) break;
			}
			if (i - j >= 0) break;

			std::swap(*i, *j);
		}
		std::swap(*beg, *j);
		return j;
	}

};


/**
 *���µ���Ŀ��ֵ��һ������Ҫ����2.5.12-2.5.30
 *2.5.32��A*�㷨����Ҫѧϰһ��
 *�ص���2.5.19 Kendall tau�������⣬֮ǰ�ü��ζ�û��������
 *��ȡ���������
 */
/**
 * A,B֮��ľ��룬��������ã����A����ȫ��ȷ��˳����ôB�����A��˵��������
 * A = 0 3 1 6 2 5 4; B = 1 0 3 6 4 2 5
 * ����2.5.26��˼·���������� order =��0316254��
 * index of B[i] = order.indexOf(b.charAt(i));
 * �ɴ˿��Եó�һ��A��˳���ǣ�0123456;B�������һ��˳����xxxxxxx��Ȼ�����������
 *
 * �����һ�������ǣ�A���������������������ܻ�������ʵ�ַ�ʽ������IndexMinPQ�ľ��飩
 * A[0,1...6]={0,3,1,6,2,5,4}����Ӧ��order0=1��order1=3....
 * ����ͨ��order��������֣�����������Ҫ��ȴ��ͨ����������ȡorder
 * AInv[0,3,1,6,2,5,4]={0,1...6}
 * �ɴ�index of B[i] AInv[B[i]]
 */

#include<vector>
int kendallTau(const std::vector<int>& a, const std::vector<int>& b)
{
	std::vector<int> ainv(a.size());

	for (int i = 0; i < a.size(); ++i)
		ainv[a[i]] = i;

	std::vector<int> bmaped(b.size());

	for (int i = 0; i < b.size(); ++i)
		bmaped[i] = ainv[b[i]];

	//__Inversion<decltype(bmaped.begin()),int> count(bmaped.begin(), bmaped.end());
	//return count.count(bmaped.begin(), bmaped.end()-1);
	return 0;
}
