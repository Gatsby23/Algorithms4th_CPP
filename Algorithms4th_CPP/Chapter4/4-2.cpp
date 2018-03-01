#include"digraph.h"
#include<fstream>
#include<iostream>

/**
 *4.2.7:���/����,���/�յ�
 */

class Degrees
{
private:
	std::vector<int> indegree_;
	std::vector<int> outdegree_;
	std::vector<int> sources_;
	std::vector<int> sinks_;
	bool isMap_ = true;

public:
	Degrees(Digraph &g) :indegree_(g.V(), 0), outdegree_(g.V(), 0)
	{
		for (int v = 0; v < g.V(); ++v)
			for (auto it = g.begin(v); it != g.end(v); ++it)
			{
				++outdegree_[v];
				++indegree_[*it];
			}

		for (int v = 0; v < g.V(); ++v)
		{
			if (indegree_[v] == 0)
				sources_.push_back(v);
			if (outdegree_[v] == 0)
				sinks_.push_back(v);
		}

		for (int v = 0; v < g.V();++v)
			if (outdegree_[v] != 1)
			{
				isMap_ = false;
				break;
			}		
	}

	int indegree(int v) { return indegree_[v]; }
	int outdegree(int v) { return outdegree_[v]; }

	std::vector<int> sources() { return sources_; }
	std::vector<int> sinks() { return sinks_; }

	bool isMap() { return isMap_; }

};

/**
 *4.2.21:�����޻�ͼ�������ͬ����
 *
 *1.������ж���ĸ߶�
 *2.Ѱ��v�����ȼ���
 *3.Ѱ��w�����ȼ���
 *4.Ѱ���������
 */


