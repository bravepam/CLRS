
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<stack>

#define NOPARENT 0
using namespace std;
enum color{ WHITE, GRAY, BLACK };
enum edge{GRAPH, TREE, BACK, FORWARD, CROSS };

struct edgeNode
{//�߽ڵ�
	size_t adjvertex;//�ñߵĹ����Ķ���
	edge e;//������
	int weight;//��Ȩ��
	edgeNode *nextEdge;//��һ����
	edgeNode(size_t adj, int w) :adjvertex(adj), weight(w),e(GRAPH),nextEdge(nullptr){}
};

struct vertex
{
	size_t c;//��ɫ
	size_t d, f;//���ʿ�ʼ�ͽ���ʱ��
	size_t p;//����
	vertex() :c(WHITE), p(NOPARENT), d(0), f(0){}
};

class AGraph
{//ͼ
private:
	vector<edgeNode*> E;
	vector<vertex> V;
	size_t nodenum;
	void DFS_aux(size_t u,size_t &time);
	void DFS_aux_Not_recursive(size_t u, size_t &time);
	void printEdge();//��ӡͼ�ı�����
	void printVertex();//��ӡ������Ϣ

	AGraph& operator=(const AGraph&);
	AGraph(const AGraph&);
public:
	AGraph(size_t n) :nodenum(n)
	{
		E.resize(n + 1);
		V.resize(n + 1);
	}
	void initGraph();//��ʼ������ͼ
	void clearVertex()
	{//�����������״̬
		for (size_t i = 1; i <= nodenum; ++i)
		{
			V[i].c = WHITE;
			V[i].p = NOPARENT;
			V[i].d = 0;
			V[i].f = 0;
		}
	}
	edgeNode* search(size_t, size_t);//���ұ�
	void addEdge(size_t, size_t, int);//����ͼ����ӱ�
	void DFS();
	void print();
	~AGraph();
};


void AGraph::initGraph()
{
	size_t start, end;
	ifstream infile("F:\\dfs.txt");
	while (infile >> start >> end)
		addEdge(start, end, 1);
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
				cout << " --> " << curr->adjvertex;
				curr = curr->nextEdge;
			}
		cout << endl;
	}
}

void AGraph::printEdge()
{
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i];
		while (curr != nullptr)
		{
			cout << i << " --> " << curr->adjvertex;
			switch (curr->e)
			{
			case TREE:
				cout << " : TREE" << endl;
				break;
			case BACK:
				cout << " : BACK" << endl;
				break;
			case FORWARD:
				cout << " : FORWARD" << endl;
				break;
			case CROSS:
				cout << " : CROSS" << endl;
				break;
			}
			curr = curr->nextEdge;
		}
	}
}

void AGraph::printVertex()
{
	cout << "parent vector" << endl;
	for (size_t i = 1; i != E.size(); ++i)
		cout << V[i].p << ' ';
	cout << endl << "start time vector" << endl;
	for (size_t i = 1; i != E.size(); ++i)
		cout << V[i].d << ' ';
	cout << endl << "finish time vector" << endl;
	for (size_t i = 1; i != E.size(); ++i)
		cout << V[i].f << ' ';
	cout << endl;
}

void AGraph::DFS_aux(size_t u, size_t &time)
{
	V[u].c = GRAY;
	V[u].d = ++time;
	edgeNode *curr = E[u];
	while (curr != nullptr)
	{
		if (V[curr->adjvertex].c == WHITE)
		{
			curr->e = TREE;
			V[curr->adjvertex].p = u;
			DFS_aux(curr->adjvertex, time);
		}
		else if (V[curr->adjvertex].c == GRAY)
			curr->e = BACK;
		else
		{
			if (V[u].d < V[curr->adjvertex].d) curr->e = FORWARD;
			else curr->e = CROSS;
		}
		curr = curr->nextEdge;
	}
	V[u].c = BLACK;
	V[u].f = ++time;
}

void AGraph::DFS_aux_Not_recursive(size_t u, size_t &time)
{
	stack<size_t> S;
	vector<edgeNode*> access_edge(E);//����ÿ��������һ���������ʵı�
	V[u].c = GRAY;
	V[u].d = ++time;
	S.push(u);
	while (!S.empty())
	{//ֻҪջ���գ����Ϸ���
		size_t i = S.top();
		edgeNode *curr = access_edge[i];//�õ�����i��ǰ��Ҫ�����ʵı�
		while (curr != nullptr)
		{//����ѭ����ֱ�����ʵ�һ���׽ڵ㣬���߶���i�������ڽӵ��ѱ�����
			if (V[curr->adjvertex].d == WHITE)
			{//��i���ڵ��ǰ׽ڵ㣬��δ�����ʹ�
				V[curr->adjvertex].c = GRAY;
				V[curr->adjvertex].p = i;
				V[curr->adjvertex].d = ++time;
				S.push(curr->adjvertex);//���ʺ���ջ
				access_edge[i] = curr->nextEdge;//���¶���i��һ����Ҫ�����ʵı�
				break;
			}
			else curr = curr->nextEdge;
		}
		if (curr == nullptr)
		{//����i�������ڽӵ��ѱ����ʣ����ջ
			V[i].c = BLACK;
			V[i].f = ++time;
			S.pop();
		}
		//��Ҫwhileѭ������ʽ
		/*if (curr != nullptr)
		{
			if (c[curr->adjvertex] == WHITE)
			{
				V[curr->adjvertex].c = GRAY;
				V[curr->adjvertex].p = i;
				V[curr->adjvertex].d = ++time;
				S.push(curr->adjvertex);
			}
			access_edge[i] = curr->nextEdge;
		}
		else
		{
			V[i].c = BLACK;
			V[i].f = ++time;
			S.pop();
		}*/
	}
}

void AGraph::DFS()
{
	size_t time = 0;
	for (size_t i = 1; i != E.size(); ++i)
		if (V[i].c == WHITE)
			DFS_aux(i, time);
	printVertex();
	printEdge();
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

const int nodenum = 8;

int main()
{
	AGraph G(nodenum);
	G.initGraph();
	G.print();
	G.DFS();
	getchar();
	return 0;
}