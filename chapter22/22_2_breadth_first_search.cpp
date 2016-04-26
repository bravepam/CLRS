
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<queue>

#define NOPARENT 0
#define MAX	0x7fffffff
using namespace std;
enum color{ WHITE, GRAY, BLACK };

struct edgeNode
{//�߽ڵ�
	size_t adjvertex;//�ñߵĹ����Ķ���
	int weight;//��Ȩ��
	edgeNode *nextEdge;//��һ����
	edgeNode(size_t adj, int w) :adjvertex(adj), weight(w), nextEdge(nullptr){}
};

class AGraph
{//ͼ�����Ա�ʾ����ͼ������ͼ
private:
	vector<edgeNode*> graph;
	size_t nodenum;
	void printPath(size_t, size_t,vector<size_t>&);

	AGraph& operator=(const AGraph&);
	AGraph(const AGraph&);
public:
	AGraph(size_t n) :nodenum(n)
	{
		if (n != 0) graph.resize(n + 1);
	}
	void initDGraph();//��ʼ������ͼ
	void initUGraph();//��ʼ������ͼ
	edgeNode* search(size_t, size_t);//���ұ�
	void addOneEdge(size_t, size_t, int);//����ͼ����ӱ�
	void addTwoEdge(size_t, size_t, int);//����ͼ����ӱ�
	void BFS(size_t);
	void print();
	~AGraph();
};


void AGraph::initDGraph()
{
	size_t start, end;
	ifstream infile("F:\\ugraph.txt");
	while (infile >> start >> end)
		addOneEdge(start, end, 1);
}

void AGraph::initUGraph()
{
	size_t start, end;
	ifstream infile("F:\\ugraph.txt");
	while (infile >> start >> end)
		addTwoEdge(start, end, 1);
}

edgeNode* AGraph::search(size_t start, size_t end)
{
	edgeNode *curr = graph[start];
	while (curr != nullptr && curr->adjvertex != end)
		curr = curr->nextEdge;
	return curr;
}

void AGraph::addOneEdge(size_t start, size_t end, int weight = 1)
{
	edgeNode *curr = search(start, end);
	if (curr == nullptr)
	{
		edgeNode *p = new edgeNode(end, weight);
		p->nextEdge = graph[start];
		graph[start] = p;
	}
}

inline void AGraph::addTwoEdge(size_t start, size_t end, int weight = 1)
{
	addOneEdge(start, end, weight);
	addOneEdge(end, start, weight);
}

inline void AGraph::print()
{
	for (size_t i = 1; i != graph.size(); ++i)
	{
		edgeNode *curr = graph[i];
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

void AGraph::printPath(size_t s, size_t f,vector<size_t> &p)
{
	if (s == f) cout << s;
	else if (p[f] == NOPARENT)
		cout << s << " has no path to " << f;
	else
	{
		printPath(s, p[f], p);
		cout << " --> " << f;
	}
}

void AGraph::BFS(size_t s)
{
	queue<size_t> Q;
	vector<size_t> dis(nodenum + 1), p(nodenum + 1),color(nodenum + 1);
	for (size_t i = 1; i <= nodenum; ++i)
	{
		dis[i] = MAX;
		p[i] = NOPARENT;
		color[i] = WHITE;
	}
	color[s] = GRAY;
	dis[s] = 0;
	p[s] = NOPARENT;
	Q.push(s);
	while (!Q.empty())
	{
		size_t u = Q.front();
		Q.pop();
		edgeNode *curr = graph[u];
		while (curr != nullptr)
		{
			if (color[curr->adjvertex] == WHITE)
			{
				color[curr->adjvertex] = GRAY;
				dis[curr->adjvertex] = dis[u] + 1;
				p[curr->adjvertex] = u;
				Q.push(curr->adjvertex);
			}
			curr = curr->nextEdge;
		}
		color[u] = BLACK;
	}
	for (size_t i = 1; i <= nodenum; ++i)
	{
		if (s != i)
		{
			printPath(s, i, p);
			cout << "\tdistance: " << dis[i] << endl;
		}
	}
}

AGraph::~AGraph()
{
	for (size_t i = 1; i != graph.size(); ++i)
	{
		edgeNode *curr = graph[i], *pre;
		while (curr != nullptr)
		{
			pre = curr;
			curr = curr->nextEdge;
			delete pre;
		}
	}
}

const int nodenum = 8;

int main()
{
	AGraph G(nodenum);
	G.initUGraph();
	G.print();
	cout << endl;
	G.BFS(4);
	getchar();
	return 0;
}