#include<iostream>
#include<vector>
#include<fstream>
#include"DisjointSet.h"

using namespace std;

enum color{ BLACK, WHITE };

struct edgeNode
{
	size_t adjvertex;
	edgeNode *nextEdge;
	edgeNode(size_t f_) :adjvertex(f_), nextEdge(nullptr){}
};

class AGraph
{
private:
	vector<edgeNode*> E;
	size_t nodenum;

	AGraph& operator=(const AGraph&);
	AGraph(const AGraph&);
public:
	AGraph(size_t n) :E(n + 1), nodenum(n){}
	void initGraph();
	void addEdge(size_t, size_t);
	edgeNode* search(size_t, size_t);
	void print();
	void Tarjan_LCA(size_t, UFS&, vector<pair<size_t,size_t>>&, vector<color>&, vector<size_t>&);
};

void AGraph::initGraph()
{
	size_t start, end;
	ifstream infile("F:\\lca.txt");
	while (infile >> start >> end)
		addEdge(start, end);
}

edgeNode* AGraph::search(size_t start, size_t end)
{
	edgeNode *curr = E[start];
	while (curr != nullptr && curr->adjvertex != end)
		curr = curr->nextEdge;
	return curr;
}

void AGraph::addEdge(size_t start, size_t end)
{
	edgeNode *curr = search(start, end);
	if (curr == nullptr)
	{
		edgeNode *p = new edgeNode(end);
		p->nextEdge = E[start];
		E[start] = p;
	}
}

void AGraph::print()
{
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i];
		cout << i;
		if (curr == nullptr) cout << " --> null";
		else
			while (curr != nullptr)
			{
				cout << " --> " << curr->adjvertex;
				curr = curr->nextEdge;
			}
		cout << endl;
	}
}

void AGraph::Tarjan_LCA(size_t root,UFS &ufs,vector<pair<size_t,size_t>> &P,
	vector<color> &c,vector<size_t> &ancestor)
{//Tarjan�㷨����С��������
	ufs.makeSet(root);//�Գ�һ������
	ancestor[root] = root;//��Ϊ����
	edgeNode *curr = E[root];
	while (curr != nullptr)
	{
		Tarjan_LCA(curr->adjvertex, ufs, P, c,ancestor);//�����������ÿһ������
		ufs.Union(root, curr->adjvertex);//�ϲ����ӵ����ļ�����
		ancestor[ufs.findSet(root)] = root;//ȷ���ϲ���ļ��ϵ�����Ϊroot
		curr = curr->nextEdge;
	}
	c[root] = BLACK;//��ʶΪ�ѷ���
	for (size_t i = 0; i != P.size(); ++i)
	{//����ѯ��
		if (root == P[i].first && c[P[i].second] == BLACK)//�����ڴ�ѯ������һ�����ѷ���
			cout << P[i].first << " and " << P[i].second << " have LCA: "
			<< ancestor[ufs.findSet(P[i].second)] << endl;//���LCA
	}
}

const int nodenum = 12;

int main()
{
	AGraph tree(nodenum);
	tree.initGraph();
	tree.print();
	UFS ufs(nodenum + 1);
	vector<size_t> ancestor(nodenum + 1);
	vector<color> c(nodenum + 1, WHITE);
	ifstream inpair("F:\\pair.txt");
	vector<pair<size_t, size_t>> P;
	size_t f, s;
	while (inpair >> s >> f)
	{
		P.push_back(pair<size_t, size_t>(s, f));//�����洢��������©
		P.push_back(pair<size_t, size_t>(f, s));//Ҳ�����޸�һ��Tarjan�㷨�е�if���
	}
	tree.Tarjan_LCA(1, ufs, P, c, ancestor);
	getchar();
	return 0;
}