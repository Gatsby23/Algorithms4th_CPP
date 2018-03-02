#pragma once
#include"../Chapter1/bag.h"
#include<vector>
#include<algorithm>

class Digraph
{

private:

	int V_;
	int E_ = 0;
	std::vector<Bag<int>> adj;

public:
	typedef Bag<int>::Iterator Iterator;

	Digraph() :V_(0) {}
	Digraph(int v) :V_(v), adj(v) {}

	Digraph(std::istream &in)
	{
		in >> V_;
		adj = std::vector<Bag<int>>(V_);
		int E;
		in >> E;

		for (int i = 0; i < E; ++i)
		{
			int v, w;
			in >> v >> w;
			addEdge(v, w);
		}
	}

	int V() { return V_; }
	int E() { return E_; }

	void addEdge(int v, int w)
	{
		int u = std::max(v, w);
		if (V_ - 1 < u)
		{
			for (int i = V_; i <= u; ++i)
			{
				++V_;
				adj.push_back(Bag<int>());
			}
		}
		++E_;
		adj[v].add(w);
	}

	Digraph reverse()
	{
		Digraph R(V_);
		for (int v = 0; v < V_; ++v)
			for (int u : adj[v])
				R.addEdge(u, v);
		return R;
	}

	Iterator begin(int v) { return adj[v].begin(); }
	Iterator end(int v) { return adj[v].end(); }

	std::string toString()
	{
		std::string ret = std::to_string(V_) + " vertices, " + std::to_string(E_) + "edges\n";
		for (int v = 0; v < V_; ++v)
		{
			ret += std::to_string(v) + ": ";
			for (auto w : adj[v])
				ret += std::to_string(w) + " ";
			ret += '\n';
		}

		return ret;
	}

};


/**
 *�����㷨1������ͼ�Ŀɴ���
 *�ش��Ƿ����һ����s����������v������·����
 */

class DirectedDFS
{
private:
	std::vector<bool> marked_;

public:
	DirectedDFS(Digraph &g, int s) :marked_(g.V(),false)
	{
		dfs(g, s);
	}

	DirectedDFS(Digraph &g, std::vector<int>& sources) :marked_(g.V(),false)
	{/**
	  *����������ô������-����������ռ�
	  */
		for (int s : sources)
			if (!marked_[s]) dfs(g, s);
	}

	bool marked(int v)
	{
		return marked_[v];
	}

private:
	void dfs(Digraph &g, int v)
	{
		marked_[v] = true;
		for (auto it = g.begin(v); it != g.end(v); ++it)
			if (!marked_[*it]) dfs(g, *it);
	}
};

/**
 *�����㷨2��Ѱ��·��
 */
#include"../Chapter1/stack.h"
class DirectedPaths
{
private:
	std::vector<bool> marked_;
	std::vector<int> edgeTo_;
	int s_;

public:
	DirectedPaths(Digraph &g, int s) :marked_(g.V(), false), edgeTo_(g.V(), -1),s_(s)
	{
		dfs(g, s);
	}

	bool hasPathTo(int v)
	{
		return marked_[v];
	}

	std::vector<int> pathTo(int v)
	{
		std::vector<int> ret;
		if (!hasPathTo(v)) return ret;
		Stack<int> stack;
		for (int x = v; x != s_; x = edgeTo_[v])
			stack.push(x);
		stack.push(s_);

		while(!stack.isEmpty())
			ret.push_back(stack.pop());
		return ret;
	}

private:
	void dfs(Digraph &g, int v)
	{
		marked_[v] = true;
		for (auto it = g.begin(v); it != g.end(v); ++it)
			if (!marked_[*it])
			{
				edgeTo_[*it] = v;
				dfs(g, *it);
			}
	}
};

/**
 *�����㷨3.1�����ļ��
 *ǣ��������ͼ������ͼ�ĶԱ�
 *������ͼ�У���û�л����ǹؼ�
 *������ͼ�У�ֻҪadj(v)�г���u֮�⣬����н�㱻���ʹ�����ô�Ϳ϶����л���
 *���ǣ�����ͼ��������ô�������ԡ�
 *
 *��������ɫmarkd��ʵ��
 */
class  DirectedCycle
{
private:
	std::vector<bool> marked_;
	std::vector<int> edgeTo_;
	std::vector<bool> onStack_;
	Stack<int> cycle_;

public:
	DirectedCycle(Digraph &g) :marked_(g.V(), false), edgeTo_(g.V(), -1), onStack_(g.V(), false)
	{
		for (int s = 0; s < g.V(); ++s)
			if (!marked_[s]) dfs(g, s);

	}

	bool hasCycle() { return !cycle_.isEmpty(); }

	std::vector<int> cycle()
	{
		std::vector<int> c;
		for (auto x : cycle_)
			c.push_back(x);
		return c;
	}

private:
	void dfs(Digraph &g, int v)
	{
		onStack_[v] = true;
		marked_[v] = true;

		for (auto it = g.begin(v); it != g.end(v); ++it)
		{
			int w = *it;
			if (hasCycle()) return;
			else if (!marked_[w])
			{
				edgeTo_[w] = v;
				dfs(g, w);
			}
			else if (onStack_[w])
			{
				for (int x = v; x != w; x = edgeTo_[x])
					cycle_.push(x);
				cycle_.push(w);
				cycle_.push(v);
			}
		}
		onStack_[v] = false;	
	}
};

/**
 *�����㷨3.2����������
 *
 *����������������ͨ����Ҫ3����
 *1. ָ����������ȼ�����
 *2. ���ϼ�Ⲣȥ������ͼ�����л�����ȷ�����ڿ��з���
 *3. ʹ��������������������
 */
#include"../Chapter1/queue.h"
class DepthFirstOrder
{
private:
	std::vector<bool> marked_;
	std::vector<int> pre_;//ǰ��
	std::vector<int> post_;//����
	Stack<int> reversePost_;//�����

public:
	
	DepthFirstOrder(Digraph &g) :marked_(g.V(), false)
	{
		for (int s = 0; s < g.V(); ++s) 
			if(!marked_[s]) dfs(g, s);
	}

	std::vector<int> pre()
	{
		return pre_;
	}

	std::vector<int> post()
	{
		return post_;
	}

	std::vector<int> reversePost()
	{
		std::vector<int> ret;
		for (auto x : reversePost_)
			ret.push_back(x);
		return ret;
	}

private:
	void dfs(Digraph &g, int v)
	{
		pre_.push_back(v);
		marked_[v] = true;
		for (auto it = g.begin(v); it != g.end(v); ++it)
		{
			if (!marked_[*it]) dfs(g, *it);
		}
			

		post_.push_back(v);
		reversePost_.push(v);
	}
};

class Topological
{
private:

	std::vector<int> order_;

public:
	Topological(Digraph &g)
	{
		bool hasCycle = DirectedCycle(g).hasCycle();
		if (!hasCycle)
			order_ = DepthFirstOrder(g).reversePost();
	}

	std::vector<int> order()
	{
		return order_;

	}

	bool isDAG() { return !order_.empty(); }
};

/**
 *�����㷨4��ǿ��ͨ����
 */

class SCC
{
private:
	std::vector<bool> marked_;
	std::vector<int> id_;
	int count_ = 0;

public:
	SCC(Digraph &g) :marked_(g.V(), false), id_(g.V(), 0)
	{
		auto order = DepthFirstOrder(g.reverse()).reversePost();
		for(int s:order)
			if (!marked_[s])
			{
				dfs(g, s);
				++count_;
			}
	}

	bool stronglyConnected(int v,int w)
	{
		return id_[v] == id_[w];
	}

	int id(int v)
	{
		return id_[v];
	}

	int count()
	{
		return count_;
	}

private:
	void dfs(Digraph &g, int v)
	{
		marked_[v] = true;
		id_[v] = count_;
		for (auto it = g.begin(v); it != g.end(v); ++it)
			if (!marked_[*it]) dfs(g, *it);
	}
};

/**
 *�����㷨5������ԵĿɴ���
 */

class TransitiveClosure
{
private:
	std::vector<DirectedDFS> all;

public:
	TransitiveClosure(Digraph &g)
	{
		for (int s = 0; s < g.V(); ++s)
			all.push_back(DirectedDFS(g, s));
	}

	bool reachable(int v, int w)
	{
		return all[v].marked(w);
	}
};
