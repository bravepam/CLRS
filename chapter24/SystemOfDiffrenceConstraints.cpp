
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<stack>
#include<fstream>
#include<vector>

#define NOPARENT 0
#define MAX	0x7ffffff
#define MIN 0x8000000

using namespace std;
enum color{ WHITE, GRAY, BLACK };

struct edgeNode
{//�߽ڵ�
	size_t adjvertex;//�ñߵĹ����Ķ���
	int weight;//��Ȩ��
	edgeNode *nextEdge;//��һ����
	edgeNode(size_t adj, int w) :adjvertex(adj), weight(w), nextEdge(nullptr){}
};

struct vertex
{
	size_t id;//����id
	int dis;//��Դ�����
	size_t p;//������
	vertex(size_t i = 0) :id(i), p(NOPARENT), dis(0){}
};

class AGraph
{//����ͼ
private:
	vector<edgeNode*> E;
	vector<vertex> V;
	size_t nodenum;
	void initVertex(size_t source)
	{//�����·��ǰ��ʼ����������
		for (size_t i = 1; i <= nodenum; ++i)
		{
			V[i].dis = MAX;
			V[i].p = i;
		};
		V[source].dis = 0;
	}
	void relax(size_t u, size_t v, int w)
	{//�ɳڲ���
		if (V[v].dis > V[u].dis + w)
		{
			V[v].dis = V[u].dis + w;
			V[v].p = u;
		}
	}
	void printSP(size_t v)
	{//��ӡԴ�㵽v�����·��
		if (v != V[v].p)
		{
			printSP(V[v].p);
			cout << " --> " << v;
		}
		else cout << v;
	}
	void printASP()
	{//��ӡԴ�㵽���ж�������·��������
		for (size_t i = 1; i < nodenum; ++i)
		{
			printSP(i);
			cout << '\t' << V[i].dis << endl;
		}
		cout << endl;
	}
	void printFeasibleSolution()
	{
		for (size_t i = 1; i < nodenum; ++i)
			cout << 'x' << i << " : " << V[i].dis << endl;
	}
	void printCycle(size_t);
	void SPT(AGraph*);//����ǰ����ͼ�������·����
public:
	AGraph(size_t n = 0) :nodenum(n) { editGraph(n); }
	void editGraph(size_t n)
	{
		V.resize(n + 1);
		E.resize(n + 1);
		for (size_t i = 1; i <= nodenum; ++i)
			V[i].id = i;
	}
	void initGraph();//��ʼ������ͼ
	edgeNode* search(size_t, size_t);//���ұ�
	void addEdge(size_t, size_t, int);//����ͼ����ӱ�
	void deleteEdge(size_t, size_t);//����ͼ��ɾ����
	bool Bellman_Ford(size_t, AGraph*);
	void print();
	~AGraph();
};

void AGraph::initGraph()
{
	size_t start, end;
	int w;
	ifstream infile("F:\\24.4-1.txt");
	while (infile >> start >> end >> w)
		addEdge(start, end, w);
}

edgeNode* AGraph::search(size_t start, size_t end)
{
	edgeNode *curr = E[start];
	while (curr != nullptr && curr->adjvertex != end)
		curr = curr->nextEdge;
	return curr;
}

void AGraph::addEdge(size_t start, size_t end, int weight = 1)
{
	edgeNode *curr = search(start, end);
	if (curr == nullptr)
	{
		edgeNode *p = new edgeNode(end, weight);
		p->nextEdge = E[start];
		E[start] = p;
	}
}

void AGraph::deleteEdge(size_t start, size_t end)
{
	edgeNode *curr = search(start, end);
	if (curr != nullptr)
	{
		if (curr->adjvertex == end)
		{
			E[start] = curr->nextEdge;
			delete curr;
		}
		else
		{
			edgeNode *pre = E[start];
			while (pre->nextEdge->adjvertex != end)
				pre = pre->nextEdge;
			pre->nextEdge = curr->nextEdge;
			delete curr;
		}
	}
}

void AGraph::SPT(AGraph *spt)
{//����ǰ����ͼ�������·����
	for (size_t i = 1; i <= nodenum; ++i)
	{
		size_t j = i;
		while (j != V[j].p)
		{
			edgeNode *curr = search(V[j].p, j);
			spt->addEdge(V[j].p, j, curr->weight);
			j = V[j].p;
		}
	}
}

void AGraph::printCycle(size_t cycle)
{//��Ȩ��·,ʱ�临�Ӷ�O(V)
	vector<color> isCycle(nodenum + 1);
	stack<size_t> C;
	for (size_t i = 1; i <= nodenum; ++i)
		isCycle[i] = WHITE;
	while (cycle != V[cycle].p)
	{
		if (isCycle[cycle] == WHITE)
		{//����ǰ���㻹û�б������·
			isCycle[cycle] = BLACK;
			C.push(cycle);
		}
		else break;//�����ѽ���·ѭ��һȦ���˳�
		cycle = V[cycle].p;
	}
	cout << cycle;
	while (!C.empty())
	{
		cout << " --> " << C.top();
		C.pop();
	}
}

bool AGraph::Bellman_Ford(size_t source, AGraph *spt)
{//Bellman-Ford�㷨���㵥Դ���·����spt�洢���·����SPT
	initVertex(source);
	for (size_t i = 1; i != nodenum; ++i)
	{//����V-1�˵���
		for (size_t j = 1; j != E.size(); ++j)
		{//ÿ�ζ������б߽���һ���ɳ�
			edgeNode *curr = E[j];
			while (curr != nullptr)
			{
				relax(j, curr->adjvertex, curr->weight);
				curr = curr->nextEdge;
			}
		}
	}
	for (size_t j = 1; j != E.size(); ++j)
	{//�ж��Ƿ��и�Ȩ��·
		edgeNode *curr = E[j];
		while (curr != nullptr)
		{
			if (V[curr->adjvertex].dis > V[j].dis + curr->weight)
			{//�����ӡ���û�·,����ֹ����
				cout << "Negative weight cycle exists" << endl;
				printCycle(curr->adjvertex);
				return false;
			}
			curr = curr->nextEdge;
		}
	}
	SPT(spt);//�����������·����
	printFeasibleSolution();
	/*cout << endl;
	printASP();//��ӡ·��*/
	return true;
}

inline void AGraph::print()
{
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i];
		cout << i;
		if (curr == nullptr) cout << " --> null";
		else
			while (curr != nullptr)
			{
				cout << " --" << "<" << curr->weight << ">--> " << curr->adjvertex;
				curr = curr->nextEdge;
			}
		cout << endl;
	}
}

AGraph::~AGraph()
{
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i], *pre;
		while (curr != nullptr)
		{
			pre = curr;
			curr = curr->nextEdge;
			delete pre;
		}
	}
}

const int nodenum = 7;

int main()
{
	AGraph graph(nodenum), spt(nodenum);
	graph.initGraph();
	graph.print();
	cout << endl;
	if (graph.Bellman_Ford(7, &spt))
		spt.print();
	getchar();
	return 0;
}