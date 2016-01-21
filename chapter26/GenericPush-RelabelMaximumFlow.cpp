
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

#define MAX	0x7fffffff

using namespace std;
enum operation{ PUSH,RELABEL };

struct edgeNode
{//边节点
	size_t adjvertex;//该边的关联的顶点
	int capacity;//边当前容量
	int flow;//边当前流量
	edgeNode *nextEdge;//下一条边
	edgeNode(size_t adj, int w) :adjvertex(adj), capacity(w), flow(0), nextEdge(nullptr){}
};

struct vertex
{
	int e;
	size_t h;
	vertex() :e(0), h(0){}
};

class AGraph
{//有向图
private:
	vector<edgeNode*> E;
	vector<vertex> V;
	size_t nodenum;
	void push(size_t, size_t);
	void relabel(size_t);
	void initializeFlow(size_t);
	operation selectOperation(size_t,size_t&);
	size_t selectVertex();
public:
	AGraph(size_t n) :nodenum(n)
	{
		V.resize(n + 1);
		E.resize(n + 1);
	}
	void initGraph();//初始化有向图
	edgeNode* search(size_t, size_t);//查找边
	edgeNode* addEdge(size_t, size_t, int);//向图中添加边
	void deleteEdge(size_t, size_t);//有向图中删除边
	int genericPushRelabel(size_t,size_t);
	void print();
	~AGraph();
};

void AGraph::initializeFlow(size_t s)
{
	V[s].h = nodenum;
	edgeNode *curr = E[s];
	while (curr != nullptr)
	{
		edgeNode *p = addEdge(curr->adjvertex, s, 0);
		curr->flow = curr->capacity;	
		p->flow = -curr->capacity;
		V[curr->adjvertex].e = curr->capacity;
		V[s].e = V[s].e - curr->capacity;
		p->capacity -= p->flow;
		curr->capacity -= curr->flow;
		curr = curr->nextEdge;
	}
}

void AGraph::push(size_t u, size_t v)
{
	edgeNode *p = search(u, v),*p_r = addEdge(v,u,0);
	int push_flow = V[u].e < p->capacity ? V[u].e : p->capacity;
	p->flow += push_flow;
	p_r->flow = -p->flow;
	V[u].e -= push_flow;
	V[v].e += push_flow;
	p->capacity -= push_flow;
	p_r->capacity = -p_r->flow;
}

void AGraph::relabel(size_t u)
{
	edgeNode *curr = E[u];
	size_t min_h = MAX;
	while (curr != nullptr)
	{
		if (curr->capacity > 0 && V[curr->adjvertex].h < min_h)
			min_h = V[curr->adjvertex].h;
		curr = curr->nextEdge;
	}
	V[u].h = min_h + 1;
}

operation AGraph::selectOperation(size_t u,size_t &v)
{
	edgeNode *curr = E[u];
	operation op = RELABEL;
	while (curr != nullptr)
	{
		if (curr->capacity > 0 && V[u].h == V[curr->adjvertex].h + 1)
		{
			op = PUSH;
			v = curr->adjvertex;
			break;
		}
		curr = curr->nextEdge;
	}
	return op;
}

size_t AGraph::selectVertex()
{
	size_t selected_vertex = 0;
	for (size_t i = 1; i <= nodenum;++i)
		if (V[i].e > 0)
		{
			selected_vertex = i;
			break;
		}
	return selected_vertex;
}

int AGraph::genericPushRelabel(size_t s,size_t t)
{
	initializeFlow(s);
	size_t u = selectVertex();
	while (u < t)
	{
		size_t v = 0;
		operation op = selectOperation(u,v);
		if (op == PUSH) push(u, v);
		else relabel(u);
		u = selectVertex();
	}
	return V[t].e;
}

void AGraph::initGraph()
{
	size_t start, end;
	int w;
	ifstream infile("F:\\maximumflow.txt");
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

edgeNode* AGraph::addEdge(size_t start, size_t end, int capacity)
{//添加边
	edgeNode *curr = search(start, end);//先查找
	if (curr == nullptr)
	{//若边不存在，则添加
		edgeNode *p = new edgeNode(end, capacity);
		p->nextEdge = E[start];
		E[start] = p;
		return p;
	}
	return curr;//否则不添加，返回当前边地址
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
				cout << " --<" << curr->capacity << ">--> " << curr->adjvertex;
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

const int nodenum = 6;

int main()
{
	AGraph graph(nodenum);
	graph.initGraph();
	graph.print();
	cout << endl;
	int maximum_flow = graph.genericPushRelabel(1,6);
	cout << maximum_flow << endl;
	getchar();
	return 0;
}