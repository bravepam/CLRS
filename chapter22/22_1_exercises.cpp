
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<queue>
#include<fstream>
#include<vector>

#define NOPARENT 0
#define MAX	0x7fffffff

using namespace std;
enum color{ WHITE, GRAY, BLACK };

struct edgeNode
{//边节点
	size_t adjvertex;//该边的关联的顶点
	int weight;//边权重
	edgeNode *nextEdge;//下一条边
	edgeNode(size_t adj, int w) :adjvertex(adj), weight(w), nextEdge(nullptr){}
};

class AGraph
{//图，可以表示有向图和无向图
private:
	vector<edgeNode*> E;
	size_t nodenum;
	void printPath(size_t, size_t, vector<size_t>&);

	AGraph& operator=(const AGraph&);
	AGraph(const AGraph&);
public:
	AGraph(size_t n) :nodenum(n)
	{
		if (n != 0) E.resize(n + 1);
	}
	void initDGraph();//初始化有向图
	void initUGraph();//初始化无向图
	edgeNode* search(size_t, size_t);//查找边
	void add1Edge(size_t, size_t, int);//有向图中添加边
	void add2Edges(size_t, size_t, int);//无向图中添加边，一次添加两条
	void delete1Edge(size_t, size_t);//有向图中删除边
	void delete2Edges(size_t, size_t);//无向图中删除边
	void outDegree();//各节点出度&&无向图的度
	void inDegree();//各节点入度&&无向图的度
	void degree();//有向图各节点的度
	void BFS(size_t);
	void reverse(AGraph*);//图转置
	void square(AGraph*);//平方图
	void print();
	~AGraph();
};

void AGraph::printPath(size_t s, size_t f, vector<size_t> &p)
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
	vector<size_t> dis(nodenum + 1), p(nodenum + 1), color(nodenum + 1);
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
		edgeNode *curr = E[u];
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

void AGraph::initDGraph()
{
	size_t start, end;
	ifstream infile("F:\\ugraph.txt");
	while (infile >> start >> end)
		add1Edge(start, end,1);
}

void AGraph::initUGraph()
{
	size_t start, end;
	ifstream infile("F:\\ugraph.txt");
	while (infile >> start >> end)
		add2Edges(start, end,1);
}

edgeNode* AGraph::search(size_t start, size_t end)
{
	edgeNode *curr = E[start];
	while (curr != nullptr && curr->adjvertex != end)
		curr = curr->nextEdge;
	return curr;
}

void AGraph::add1Edge(size_t start, size_t end, int weight = 1)
{
	edgeNode *curr = search(start, end);
	if (curr == nullptr)
	{
		edgeNode *p = new edgeNode(end, weight);
		p->nextEdge = E[start];
		E[start] = p;
	}
}

inline void AGraph::add2Edges(size_t start, size_t end, int weight = 1)
{
	add1Edge(start, end, weight);
	add1Edge(end, start, weight);
}

void AGraph::delete1Edge(size_t start, size_t end)
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

inline void AGraph::delete2Edges(size_t start, size_t end)
{
	delete1Edge(start, end);
	delete1Edge(end, start);
}

void AGraph::outDegree()
{
	cout << "Out-degree-----------" << endl;
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i];
		size_t count = 0;
		while (curr != nullptr)
		{
			++count;
			curr = curr->nextEdge;
		}
		cout << "vertex " << i << " : " << count << endl;
	}
}

void AGraph::inDegree()
{
	vector<size_t> degree(nodenum + 1);
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i];
		while (curr != nullptr)
		{
			++degree[curr->adjvertex];
			curr = curr->nextEdge;
		}
	}
	cout << "In-degree-------------" << endl;
	for (size_t i = 1; i != degree.size(); ++i)
		cout << "vertex " << i << " : " << degree[i] << endl;
}

void AGraph::degree()
{
	vector<size_t> d(E.size());
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i];
		while (curr != nullptr)
		{
			++d[i];
			++d[curr->adjvertex];
			curr = curr->nextEdge;
		}
	}
	cout << "Degree---------------" << endl;
	for (size_t i = 1; i != d.size(); ++i)
		cout << "vertex " << i << " : " << d[i] << endl;
}

void AGraph::reverse(AGraph *regraph)
{
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i];
		while (curr != nullptr)
		{
			regraph->add1Edge(curr->adjvertex, i);
			curr = curr->nextEdge;
		}
	}
}

void AGraph::square(AGraph *sqgraph)
{
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr1 = E[i];
		while (curr1 != nullptr)
		{
			edgeNode *curr2 = E[curr1->adjvertex];
			while (curr2 != nullptr)
			{
				sqgraph->add1Edge(i, curr2->adjvertex);
				curr2 = curr2->nextEdge;
			}
			curr1 = curr1->nextEdge;
		}
	}
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
				cout << " --<" << curr->weight << ">--> " << curr->adjvertex;
				curr = curr->nextEdge;
			}
		cout << endl;
	}
}

AGraph::~AGraph()
{
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i],*pre;
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
	AGraph E(nodenum);
	E.initUGraph();
	E.print();
	/*AGraph regraph(nodenum);
	E.reverse(&regraph);
	regraph.print();
	AGraph sqgraph(nodenum);
	E.square(&sqgraph);
	sqgraph.print();*/
	/*E.outDegree();
	E.inDegree();*/
	E.outDegree();
	E.BFS(4);
	getchar();
	return 0;
}