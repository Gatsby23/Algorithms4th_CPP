#pragma once

/**
 *һ���������˵��graph�����ڽӱ���ʵ�֣���������������£�Ҳ�п��ܻ�������������ݽṹ������μ�P337
 *
 */
#include"../Chapter1/bag.h"
#include<vector>
#include<algorithm>
class Graph
{

private:

	int V_;
	int E_ = 0;
	std::vector<Bag<int>> adj;

public:
	typedef Bag<int>::Iterator Iterator;

	Graph() :V_(0) {}
	Graph(int v) :V_(v), adj(v) {}

	Graph(std::istream &in)
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
		if (V_-1 < u)
		{
			for (int i = V_; i <= u; ++i)
			{
				++V_;
				adj.push_back(Bag<int>());
			}
		}
		++E_;
		adj[v].add(w);
		adj[w].add(v);
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
 *�����㷨1��Search���ж���s����ͨ�ĵ㣬��s����ͨ�ĵ�ĸ���
 *������������Ŀ�ĵĻ������繹��Union-Find���ÿ�
 *���ǣ��˴�ʹ��DFS��ʵ��
 */
#include"../Chapter1/queue.h"
class Search
{
private:
	Graph &graph;
	int s_;

	std::vector<bool> marked_;
	int count_ = 0;

public:

	Search(Graph& g, int s) :graph(g), s_(s), marked_(g.V(), false)
	{
		dfs(graph, s);
	}

	bool merked(int v) { return marked_[v]; }
	int count() { return count_; }

private:

	void dfs(Graph& g, int v)
	{
		marked_[v] = true;
		++count_;

		for (auto it = g.begin(v); it != g.end(v); ++it)
			if (!marked_[*it]) dfs(g, *it);
	}

	void bfs(Graph &g, int s)
	{
		Queue<int> q;
		q.enqueue(s);

		while (!q.isEmpty())
		{
			int v = q.dequeue();
			marked_[v] = true;
			++count_;
			for (auto it = g.begin(v); it != g.end(v); ++it)
				if (!marked_[*it]) q.enqueue(*it);
		}
	}
};

/**
 *�����㷨2��Ѱ��·��
 *·���Ͳ�����union-find��Ѱ���ˣ����ǵ�ʹ����������
 */
#include"../Chapter1/stack.h"
class Path
{
private:
	Graph &graph;
	int s_;

	std::vector<bool> marked_;
	std::vector<int> edgeTo_;

public:

	Path(Graph &g, int s) :graph(g), s_(s), marked_(g.V(), false), edgeTo_(g.V()) 
	{
		dfs(graph, s_);
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

	void dfs(Graph &g, int v)
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
 *�����㷨3����ͨ����
 *�����ֿ���ʹ��union-find�㷨
 *�����DFS��union-find��һ����̬�㷨
 */
class CC
{
private:
	Graph &graph;

	std::vector<bool> marked_;
	std::vector<int> id_;
	int count_ = 0;

public:

	CC(Graph &g) :graph(g), marked_(g.V(), false), id_(g.V())
	{
		for (int s = 0; s < graph.V();++s)
			if (!marked_[s])
			{
				dfs(g, s);
				++count_;
			}
	}

	bool connected(int v, int w)
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

	void dfs(Graph &g, int v)
	{
		marked_[v] = true;
		id_[v] = count_;
		for (auto it = g.begin(v); it != g.end(v); ++it)
			if (!marked_[*it]) dfs(g, *it);
	}

};

/**
 *�����㷨4����⻷
 *Graph�ǲ����޻�ͼ��
 *���ԭ��ÿ����ֻ����һ�����ڵ�
 */

class Cycle
{
private:
	Graph &graph;

	std::vector<bool> marked_;
	bool hasCycle_ = false;

public:
	Cycle(Graph &g) :graph(g), marked_(g.V(), false)
	{
		for (int s = 0; s < g.V(); ++s)
			if (!marked_[s]) dfs(graph, s, s);
	}

	bool hasCycle() { return hasCycle_; }

private:
	void dfs(Graph &g, int v, int u)
	{
		marked_[v] = true;
		for (auto it = g.begin(v); it != g.end(v); ++it)
			if (!marked_[*it]) dfs(g, *it,v);
			else if (*it != u) hasCycle_ = true;
	}

};

/**
 *�����㷨5������ͼ���
 */

class TwoColor
{
private:
	Graph &graph;
	std::vector<bool> marked_;
	std::vector<bool> color_;
	bool isTwoColorable_ = true;

public:
	TwoColor(Graph &g) :graph(g), marked_(g.V(), false), color_(g.V(), false)
	{
		for (int s = 0; s < g.V(); ++s)
			if (!marked_[s]) dfs(graph, s);
	}

	bool isBipartite() { return isTwoColorable_; }

private:
	void dfs(Graph &g, int v)
	{
		marked_[v] = true;
		for (auto it = g.begin(v); it != g.end(v); ++it)
			if (!marked_[*it])
			{
				color_[*it] = !color_[v];
				dfs(g, *it);
			}
			else if (color_[*it] == color_[v]) isTwoColorable_ = false;
	}
};

/**
 *�����㷨6������ͼ
 */
#include"../Chapter3/ST2017.h"
class SymbolGraph
{
private:
	RedBlackBST<const std::string,int> st;//������->����
	std::vector<std::string> keys;//����->������
	Graph graph;

public:
	SymbolGraph(std::istream &in) :st("",-1)
	{
		std::string v, w;
		while (in >> v >> w)
		{
			addEdge(v, w);
		}
	}

	bool contains(const std::string &s) { return st.contains(s); }
	int index(const std::string &s) { return st.get(s); }
	std::string name(int v) { return keys[v]; }
	Graph G() { return graph; }

	void addEdge(const std::string &v, const std::string &w)
	{
		if (!st.contains(v))
		{	
			st.put(v, st.size());
			keys.push_back(v);
		}
		
		if (!st.contains(w))
		{
			st.put(w, st.size());
			keys.push_back(w);
		}
			
		graph.addEdge(st.get(v), st.get(w));
	}
	
};



