#pragma once
#include<string>

int search(const std::string &pat, const std::string &txt)
{
	int M = pat.size();
	int N = txt.size();

	for (int i = 0; i <= N - M; ++i)
	{
		int j;
		for (j = 0; j < M; ++j)
			if (txt[i + j] != pat[j]) break;
		if (j == M) return i;//�ҵ�ƥ��
	}
	return N;//δ�ҵ�ƥ��
}

int search2(const std::string &pat, const std::string &txt)
{
	int j, M = pat.size();
	int i, N = txt.size();
	
	for (i = 0, j = 0; i < N && j < M; ++i)
	{
		if (txt[i] == pat[j]) ++j;
		else 
		{
			i -= j; j = 0;
		}
	}

	if (j == M) return i - M;
	else return N;
}

#include<vector>
class KMP
{
private:
	std::string pat;
	std::vector<std::vector<int>> dfa;

public:
	KMP(const std::string &p) :pat(p)
	{
		int M = pat.size();
		int R = 256;
		dfa = std::vector<std::vector<int>>(R, std::vector<int>(M, 0));
		dfa[pat[0]][0] = 1;

		for (int X = 0, j = 1; j < M; ++j)
		{//X��Ӧ����j��ƥ�������£�Ӧ�û��˵���״̬��ʵ��һ��X��ƥ��״̬
			for (int c = 0; c < R; ++c)
				dfa[c][j] = dfa[c][X];
			dfa[pat[j]][j] = j + 1;
			//for j+1 ��ƥ��,�ͻ���˵�ʲô����أ����j��ƥ��Ļ���״̬��pat[j]�����ͬ
			//��ô����Ļ���״̬��dfa[pat[j]][X]->X+1;
			//���j�Ļ���״̬X��pat[j]��ƥ�䣬��ôֻ�ܼ��������ˣ�����ʲôʱ����˵�һ�ֺ��ʵ�״̬
			X = dfa[pat[j]][X];
		}
	}
	
	int search(const std::string &txt)
	{
		int i, j, N = txt.size(), M = pat.size();
		for (i = 0; i < N&&j < M; ++i)
			j = dfa[txt[j]][j];
		if (j == M) return i - M;
		else return N;
	}
};

class BoyerMoore
{
private:
	std::vector<int> right;
	std::string pat;

public:
	BoyerMoore(const std::string &p):pat(p)
	{
		int M = pat.size();
		int R = 256;
		right = std::vector<int>(R, -1);

		for (int j = 0; j < M; ++j)
			right[pat[j]] = j;
	}

	int search(const std::string &txt)
	{
		int N = txt.size();
		int M = pat.size();
		int skip;

		for (int i = 0; i <= N - M; i += skip)
		{
			skip = 0;
			for (int j = M - 1; j >= 0; --j)
				if (pat[j] != txt[i + j])
				{
					skip = j - right[txt[i + j]];
					if (skip < 1) skip = 1;
					break;
				}
			if (skip == 0) return i;
		}

		return N;
	}
};

#include"digraph.h"
#include"stack.h"

class NFA
{
private:
	Digraph graph;
	std::string regexp;
	int M;

public:
	NFA(const std::string &reg) :regexp(reg), M(reg.size()), graph(reg.size()+1)
	{
		Stack<int> ops;
		for (int i = 0; i < M; ++i)
		{
			int lp = i;
			if (regexp[i] == '(' || regexp[i] == '|')
				ops.push(i);
			else if (regexp[i] == ')')
			{
				int r = ops.pop();
				//����ǻ�
				if (regexp[r] == '|')
				{
					lp = ops.pop();//(
					graph.addEdge(lp, r + 1);
					graph.addEdge(r, i);
				}
				else if (regexp[r] == '(')//�����һ�����ű��ʽ
					lp = r;
			}

			if (i < M - 1 && regexp[i + 1] == '*')//��ʱ��һ���հ�(���ַ������ű��ʽ��
			{
				graph.addEdge(lp, i + 1);
				graph.addEdge(i + 1, lp);
			}

			if (regexp[i] == '(' || regexp[i] == '*' || regexp[i] == ')')
				graph.addEdge(i, i + 1);
		}
	}

	bool recognizes(const std::string &txt)
	{
		DirectedDFS dfs(graph, 0);
		std::vector<int> pc;

		for (int v = 0; v < graph.V(); ++v)
			if (dfs.marked(v)) pc.push_back(v);

		for (int i = 0; i < txt.size(); ++i)
		{
			std::vector<int> match;
			for (int v : pc)
			{
				if (v == M) continue;
				if (regexp[v] == txt[i] || regexp[v] == '.')
					match.push_back(v + 1);
			}

			dfs = DirectedDFS(graph, match);
			pc.clear();
			for (int v = 0; v < graph.V(); ++v)
				if (dfs.marked(v)) pc.push_back(v);

			if (pc.size() == 0) return false;
		}

		for (int v : pc)
			if (v == M) return true;
		return false;
	}
};