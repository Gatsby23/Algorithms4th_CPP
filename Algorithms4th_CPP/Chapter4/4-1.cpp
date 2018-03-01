#include"graph.h"
#include<fstream>
#include<iostream>

/**
 *4.1.35:�ߵ���ͨ�ԣ������
 *���������������
 *ʲô���ţ��ŵ�������ͨ��㶼����������֮ǰ�ĵ㡢
 *������һ�������ʵ�֣���һ����������¼����˳��
 *��ô��֮���������ͨ��㶼����������֮ǰ����ͨ��㣬��������ʵ�֣�
 *������Ҫһ���ֶ�����¼��
 */

class Bridge
{
private:
	int bridges = 0;//�ŵĸ���
	int cnt = 0;//��Ƿ���˳��
	std::vector<int> order;//����˳��
	std::vector<int> low;

public:

	Bridge(Graph &g) :order(g.V(),-1),low(g.V(),-1)
	{
		for (int s = 0; s < g.V(); ++s)
			if (order[s] == -1)
				dfs(g, s, s);
	}

private:
	void dfs(Graph &g, int v,int u)
	{
		order[v] = cnt++;
		low[v] = order[v];//��ʼ��
		for (auto it = g.begin(v); it != g.end(v); ++it)
		{
			int w = *it;
			if (order[w] == -1)
			{
				dfs(g, w,v);
				low[v] = std::min(low[v], low[w]);
				if (low[w] == order[w])
				{
					std::cout << v << '-' << w << " is a bridge\n";
					++bridges;
				}
			}
			else if (w != u)
				low[v] = std::min(low[v], order[w]);
		}

	}
};
