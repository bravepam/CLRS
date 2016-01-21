
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
#include<algorithm>

#define NOPARENT 0
#define MAX	0x7ffffff
#define MIN 0x8000000

using namespace std;
enum color{ WHITE, GRAY, BLACK };
typedef vector<vector<int>> matrix;

ostream& operator<<(ostream &out, const matrix &m)
{//重载输出操作符，以输出矩阵
	for (size_t i = 1; i != m.size(); ++i)
	{
		for (size_t j = 1; j != m[i].size(); ++j)
			if (m[i][j] == MAX) out << 'M' << ' ';
			else out << m[i][j] << ' ';
			out << endl;
	}
	return out;
}

inline void initMatrix(matrix &W, size_t n)
{//初始化矩阵，除对角线初始化为0外，其他全部为无穷大
	W.resize(n + 1);
	for (size_t i = 1; i <= n; ++i)
		W[i].resize(n + 1, MAX);
	for (size_t i = 1; i <= n; ++i)
		W[i][i] = 0;
}

struct edgeNode
{//边节点
	size_t adjvertex;//该边的关联的顶点
	int weight;//边权重
	edgeNode *nextEdge;//下一条边
	edgeNode(size_t adj, int w) :adjvertex(adj), weight(w), nextEdge(nullptr){}
};

struct vertex
{
	size_t id;//顶点id
	color c;
	int dis;//距源点距离
	size_t p;//父顶点
	vertex(size_t i = 0) :id(i), p(NOPARENT), c(WHITE), dis(0){}
};

class AGraph
{//有向图
private:
	vector<edgeNode*> E;
	vector<vertex> V;
	size_t nodenum;
	void initVertex(size_t source)
	{
		for (size_t i = 1; i <= nodenum; ++i)
		{
			V[i].dis = MAX;
			V[i].p = i;
			V[i].c = WHITE;
		};
		V[source].dis = 0;
	}
	void relax(size_t u, size_t v, int w)
	{
		if (V[v].dis > V[u].dis + w)
		{
			V[v].dis = V[u].dis + w;
			V[v].p = u;
		}
	}
	void printCycle(size_t);
public:
	AGraph(size_t n = 0) :nodenum(n) { editGraph(n); }
	void editGraph(size_t n)
	{
		V.resize(n + 1);
		E.resize(n + 1);
		for (size_t i = 1; i <= nodenum; ++i)
			V[i].id = i;
	}
	void initGraph();//初始化有向图
	edgeNode* search(size_t, size_t);//查找边
	void addEdge(size_t, size_t, int);//有向图中添加边
	void deleteEdge(size_t, size_t);//有向图中删除边
	bool Bellman_Ford(size_t);
	void dijkstra(size_t);
	void Johnson(matrix&, matrix&);
	void print();
	~AGraph();
};

void AGraph::initGraph()
{
	size_t start, end;
	int w;
	ifstream infile("F:\\asp25_2.txt");
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
	edgeNode *p = new edgeNode(end, weight);
	p->nextEdge = E[start];
	E[start] = p;
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

void AGraph::printCycle(size_t cycle)
{//求负权回路,时间复杂度O(V)
	vector<color> onCycle(nodenum + 1);
	for (size_t i = 1; i <= nodenum; ++i)
		onCycle[i] = WHITE;
	while (cycle != V[cycle].p)
	{
		if (onCycle[cycle] == WHITE)
		{//若当前顶点还没有被纳入回路
			onCycle[cycle] = BLACK;
			cout << cycle << ' ';
		}
		else break;//否则已将回路循环一圈，退出
		cycle = V[cycle].p;
	}
}

bool AGraph::Bellman_Ford(size_t source)
{//Bellman-Ford算法计算单源最短路径，spt存储最短路径树SPT
	initVertex(source);
	for (size_t i = 1; i != nodenum; ++i)
	{//进行V-1此迭代
		for (size_t j = 0; j != E.size(); ++j)
		{//每次都对所有边进行一次松弛
			edgeNode *curr = E[j];
			while (curr != nullptr)
			{
				relax(j, curr->adjvertex, curr->weight);
				curr = curr->nextEdge;
			}
		}
	}
	for (size_t j = 1; j != E.size(); ++j)
	{//判断是否有负权回路
		edgeNode *curr = E[j];
		while (curr != nullptr)
		{
			if (V[curr->adjvertex].dis > V[j].dis + curr->weight)
			{//有则打印出该回路,并终止程序
				printCycle(curr->adjvertex);
				cout << endl;
				return false;
			}
			curr = curr->nextEdge;
		}
	}
	return true;
}

void AGraph::dijkstra(size_t source)
{//dijkstra算法求单源最短路径，只适用于非负权值无回路图
	initVertex(source);
	for (size_t i = 1; i <= nodenum; ++i)
	{//进行V次迭代
		int min = MAX, id = -1;
		for (size_t j = 1; j <= nodenum; ++j)
		{//每次选出未纳入最短路径树的dis最小的节点
			if (V[j].dis <= min && V[j].c == WHITE)
			{
				min = V[j].dis;
				id = j;
			}
		}
		V[id].c = BLACK;//纳入最短路径树
		edgeNode *curr = E[id];
		while (curr != nullptr)
		{//对该顶点的所有边进行一次松弛
			relax(id, curr->adjvertex, curr->weight);
			curr = curr->nextEdge;
		}
	}
}

void AGraph::Johnson(matrix &d, matrix &p)
{
	if (!Bellman_Ford(0)) return;
	vector<int> h(nodenum + 1);
	for (size_t i = 1; i <= nodenum; ++i)
		h[i] = V[i].dis;
	for (size_t i = 1; i <= nodenum; ++i)
	{
		edgeNode *curr = E[i];
		while (curr != nullptr)
		{
			curr->weight = h[i] + curr->weight - h[curr->adjvertex];
			curr = curr->nextEdge;
		}
	}
	for (size_t i = 1; i <= nodenum; ++i)
	{
		dijkstra(i);
		for (size_t j = 1; j <= nodenum;++j)
			if (i != j && V[j].p != j)
			{
				p[i][j] = V[j].p;
				d[i][j] = V[j].dis + h[j] - h[i];
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

const int nodenum = 6;

int main()
{
	AGraph graph(nodenum);
	graph.initGraph();
	matrix d, p;
	initMatrix(d,nodenum); initMatrix(p,nodenum);
	graph.Johnson(d, p);
	cout << d << endl << p;
	getchar();
	return 0;
}