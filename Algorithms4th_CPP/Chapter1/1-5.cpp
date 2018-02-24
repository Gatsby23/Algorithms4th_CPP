/**
Ϊ�˴ﵽ������Ч����������Ҫ���һ�����ݽṹ�����������֪�����������Ե��㹻��Ϣ�����������ж�һ���¶����Ƿ��������ġ�
���ǽ�������������̬��ͨ������
*/

/*������㷨��ʱ����Եĵ�һ��������Ǿ�ȷ�ض�������*/
/** API 
 * UF(int N) ��������ʶ��0��N-1����ʼ��N������
 * void union(int p, int q) ��p��q֮�����һ������
 * int find(int p) p���ڷ����ı�ʶ����0��N-1��
 * bool connected(int p, int q) ���p��q������ͬһ���������򷵻�true
 * int count() ��ͨ�����ĸ���
 */
#include<vector>
class UF1
{
private:
	std::vector<int> id; //����id
	int cnt;//��������

public:
	UF1(int N) :cnt(N)
	{//��ʼ������id����
		for (int i = 0; i < N; ++i)
			id.push_back(i);
	}

	int count() const { return cnt; }

	bool connected(int p, int q) { return find(p) == find(q); }

	int find(int p);
	void unions(int p, int q);
	int find2(int p);
	void unions2(int p, int q);
};

/**
 *ģ��1��quick findģ��
 *�����е���ͨ��ֵ��ͬһ��id
 */

int UF1::find(int p)
{
	return id[p];
}

void UF1::unions(int p, int q)
{
	int pID = find(p);
	int qID = find(q);

	if (pID == qID) return;

	for (int i = 0; i < id.size(); ++i)
		if (id[i] == pID) id[i] = qID;
	--cnt;
}

/**
 *����������������ֻ�õ�һ����ͨ��������ô����Ҫ����N-1��unions������quick-find������ƽ������
 *���Ǳ���Ҫ�Ľ�
 */

/**
 *ģ�Ͷ���quick-union
 *������״�ṹ����֯id��ֻ��Ҫ��һ��������
 */

int UF1::find2(int p)
{
	while (p != id[p]) p = id[p];
	return p;
}

void UF1::unions2(int p, int q)
{
	int pRoot = find2(p);
	int qRoot = find2(q);
	if (pRoot == qRoot) return;

	id[pRoot] = qRoot;
	--cnt;
}

/**
 *�������������Ķ��壺find��������Ĵ���Ϊ1+�ڵ����ȵ�������unions��connected��������Ĵ���Ϊ����find����
 *�����õ�һ����ͨ����������£������£���ƽ������ġ�
 *�Ƿ��а취���������ĳ��֣��������������
 */

/**
 *ģ��3����Ȩ��quick-union
 *��������Ľ�һ�������ӵ���һ�������������ڻ��¼ÿһ�����Ĵ�С��������С�������ӵ��ϴ������
 */
/*
class UF
{
private:
	std::vector<int> id; 
	std::vector<int> sz;
	int cnt;

public:
	UF(int N) :cnt(N)
	{
		for (int i = 0; i < N; ++i)
		{
			id.push_back(i);
			sz.push_back(1);
		}
	}

	int count() const { return cnt; }
	bool connected(int p, int q) { return find(p) == find(q); }
	int find(int p)
	{
		while (p != id[p]) p = id[p];
		return p;
	}

	void unions(int p, int q)
	{
		int pRoot = find(p);
		int qRoot = find(q);
		if (pRoot == qRoot) return;

		if (sz[pRoot] < sz[qRoot])
		{
			id[pRoot] = qRoot;
			sz[qRoot] += sz[pRoot];
		}
		else
		{
			id[qRoot] = pRoot;
			sz[pRoot] += sz[qRoot];
		}
		--cnt;
	}
};
*/
/**
 *����������N���㣬��Ȩquick-union�㷨�����ɭ���е�����ڵ��������ΪlgN������ѧ���ɷ����������i+j=k, ��i<=j,��С�������нڵ���������
 *��1,1+lgi=lg(i+i)<=lg(i+j)=lgk
 *���ԣ��������£�find������������ΪlogN
 *�ɴ˿ɼ�����Ȩquick-union�㷨�������㷨��Ψһ�������ڴ���ʵ��������㷨
 */


/****************************************************************************************************************************
 ************************************************��ϰ��***********************************************************************
 ****************************************************************************************************************************
 */

/**1.5.8,����ʵ��Ϊʲô���д��󣿣�
void union(int p, int q) {
if (connected(p, q)) return;
for (int i = 0; i < id.length; i++)
if (id[i] == id[p]) id[i] = id[q];
count--;
}
 *����ϸ�����о�ʱû��ʲô����ģ�
 *�������ڣ�������forѭ���и�����id[p],�����򰵺��ļٶ��ǣ�ѭ����id[p]�������ı�
 */

//���ǿ��Կ������������ʹ��find��������ô·��ѹ�����Լ������߲���Ч��
//1.5.13 :ʹ��·��ѹ���ļ�Ȩquick-union
class PathCompressionUF
{
private:
	std::vector<int> id;
	std::vector<int> sz;
	int cnt;

public:
	PathCompressionUF(int N) :cnt(N)
	{
		for (int i = 0; i < N; ++i)
		{
			id.push_back(i);
			sz.push_back(1);
		}
	}

	int count() const { return cnt; }
	bool connected(int p, int q) { return find(p) == find(q); }
	int find(int p)
	{
		int root = p;
		while (root != id[root]) root = id[root];

		//·��ѹ��
		while (p != root)
		{
			int father = id[p];
			id[p] = root;
			sz[father] -= sz[p];
			p = father;
		}
		return root;
	}

	void unions(int p, int q)
	{
		int pRoot = find(p);
		int qRoot = find(q);
		if (pRoot == qRoot) return;

		if (sz[pRoot] < sz[qRoot])
		{
			id[pRoot] = qRoot;
			sz[qRoot] += sz[pRoot];
		}
		else
		{
			id[qRoot] = pRoot;
			sz[pRoot] += sz[qRoot];
		}
		--cnt;
	}

};

//����֮ǰ���㷨�����ǿ���֪����ֻ�и��ڵ��size�����ã��������ǻ���Ҫά���м�ڵ��size��������������Ѷ�
//�������ݽṹ����Ϊid[root]=root;�������ǿ�����id[root]������-size

/**
 *1.5.14��Ҳ���Ը������ĸ߶�����Ȩ�����ǽ����������ӵ��ߵ�����
 *�������ϰ�⽫ʵ�����������뷨
 */
class WeightedUFByHeight
{
private:
	std::vector<int> id;//id[root]�洢�������ĸ߶�
	int cnt;

public:
	WeightedUFByHeight(int N) :cnt(N)
	{
		for (int i = 0; i < N; ++i)
			id.push_back(-1);//-1������Ǹ߶�0
	}

	int count() const { return cnt; }

	bool connected(int p, int q) { return find(p) == find(q); }

	int find(int p)
	{
		int root = p;
		while (id[root] > 0) root = id[root];

		//·��ѹ��,���Ѳ�׽�߶ȵı仯�����Բ�ʵ�֣�����ʹ��һ������������height
		return root;

	}

	void unions(int p, int q)
	{
		int pRoot = find(p);
		int qRoot = find(q);

		if (pRoot == qRoot) return;

		if (-id[pRoot] > -id[qRoot])
		{
			//p���Ƚϸ�
			id[qRoot] = pRoot;
			//q�����ļ��벢���ܸı�p�����ĸ߶�
		}
		else if (-id[pRoot] < -id[qRoot])
		{
			id[pRoot] = qRoot;
		}
		else
		{
			id[qRoot] = pRoot;
			--id[pRoot];
		}
	}
};

