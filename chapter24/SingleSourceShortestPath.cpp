
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
#include<algorithm>
#include<functional>
#include"FibonacciHeap.h"

#define NOPARENT 0
#define MAX	0x7ffffff
#define MIN 0x8000000

using namespace std;
enum color{ WHITE, GRAY, BLACK };

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
	size_t s, f;//访问开始和结束时间
	vertex(size_t i = 0) :id(i), p(NOPARENT), c(WHITE),dis(0),s(0),f(0){}
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
	void printSP(size_t v)
	{
		if (v != V[v].p)
		{
			printSP(V[v].p);
			cout << " --> " << v;
		}
		else cout << v;
	}
	void printASP()
	{
		for (size_t i = 1; i <= nodenum; ++i)
		{
			printSP(i);
			cout << '\t' << V[i].dis << endl;
		}
		cout << endl;
	}
	size_t vertexIndex(size_t vertex_id)
	{
		size_t index;
		for (size_t i = 1; i <= nodenum; ++i)
			if (V[i].id == vertex_id) index = i;
		return index;
	}
	void SPT(AGraph*);
	void printCycle(size_t);
	void DFS();
	void DFS_aux_Not_recursive(size_t, size_t&);
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
	bool Bellman_Ford(size_t,AGraph*);
	void dagSP(size_t);
	void dijkstra(size_t,AGraph *);
	void topSort(vector<size_t>&);
	void print();
	~AGraph();
};

void AGraph::initGraph()
{
	size_t start, end;
	int w;
	ifstream infile("F:\\Bellman_Ford.txt");
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
{//根据前驱子图构造最短路径树
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
{//求负权回路,时间复杂度O(V)
	vector<color> onCycle(nodenum + 1);
	for (size_t i = 1; i <= nodenum; ++i)
		onCycle[i] = WHITE;//顶点全部初始化为白色
	while (onCycle[cycle] == WHITE)
	{//当遇到黑色顶点时退出，表明此时是第二次扫描到，可断定该顶点必在回路上
		onCycle[cycle] = BLACK;
		cycle = V[cycle].p;
	}
	while (onCycle[cycle] == BLACK)
	{//以该顶点为起点，开始输出回路上的顶点
		onCycle[cycle] = WHITE;
		cout << cycle << ' ';
		cycle = V[cycle].p;
	}
}

bool AGraph::Bellman_Ford(size_t source,AGraph *spt)
{//Bellman-Ford算法计算单源最短路径，spt存储最短路径树SPT
	initVertex(source);
	for (size_t i = 1; i != nodenum; ++i)
	{//进行V-1此迭代
		for (size_t j = 1; j != E.size(); ++j)
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
	SPT(spt);//否则计算出最短路径树
	return true;
}

void AGraph::dagSP(size_t source)
{//有向无环图求单源最短路径
	vector<size_t> topsort;
	topSort(topsort);//先进行拓扑排序，并将拓扑顺序的顶点标号存入topsort
	initVertex(source);//初始化顶点的距离和父顶点
	for (size_t i = 0; i != topsort.size(); ++i)
	{//按拓扑顺序
		edgeNode *curr = E[topsort[i]];
		while (curr != nullptr)
		{//对每个顶点的边进行一次松弛
			relax(topsort[i], curr->adjvertex, curr->weight);
			curr = curr->nextEdge;
		}
	}
	for (size_t i = 0; i != topsort.size(); ++i)
	{
		size_t index = vertexIndex(topsort[i]);
		cout << topsort[i] << '\t' << V[index].dis << endl;
	}
	//SPT(spt);//这里最好不要构造最短路径树了，若要构造，则需对SPT进行些许修改，
	//因为顶点编号和索引不在对应，在排序过程中移动了。
}

void AGraph::dijkstra(size_t source,AGraph *spt)
{//dijkstra算法求单源最短路径，只适用于非负权值无回路图，斐波那契堆实现
	initVertex(source);
	fibonacci_heap<int, size_t> Q;
	vector<fibonacci_heap_node<int,size_t>*> ptr_fibo_node(nodenum + 1);//顶点所关联的堆结点地址
	for (size_t i = 1; i <= nodenum; ++i)
		ptr_fibo_node[i] = Q.insert(V[i].dis, i);
	while (!Q.empty())
	{
		pair<int, size_t> min = Q.extractMin();//取得当前离源点最近顶点
		ptr_fibo_node[min.second] = nullptr;//置空，表明堆中已删除该顶点
		edgeNode *curr = E[min.second];
		while (curr != nullptr)
		{
			relax(min.second, curr->adjvertex, curr->weight);
			if (ptr_fibo_node[curr->adjvertex] != nullptr && //如果该顶点到源点距离减小
				V[curr->adjvertex].dis < ptr_fibo_node[curr->adjvertex]->key)
				Q.decreaseKey(ptr_fibo_node[curr->adjvertex], V[curr->adjvertex].dis);
			curr = curr->nextEdge;
		}
	}
	//for (size_t i = 1; i <= nodenum; ++i)
	//{//进行V次迭代
	//	int min = MAX, id = 0;
	//	for (size_t j = 1; j <= nodenum; ++j)
	//	{//每次选出未纳入最短路径树的dis最小的节点
	//		if (V[j].dis < min && V[j].c == WHITE)
	//		{
	//			min = V[j].dis;
	//			id = j;
	//		}
	//	}
	//	V[id].c = BLACK;//纳入最短路径树
	//	edgeNode *curr = E[id];
	//	while (curr != nullptr)
	//	{//对该顶点的所有边进行一次松弛
	//		relax(id, curr->adjvertex, curr->weight);
	//		curr = curr->nextEdge;
	//	}
	//}
	SPT(spt);//根据前驱子图求得最短路径树邻接表
	printASP();//打印源点到各顶点路径及最小权值和
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

void AGraph::DFS_aux_Not_recursive(size_t u, size_t &time)
{
	stack<size_t> S;
	vector<edgeNode*> access_edge(E);//记下每个顶点下一条将被访问的边
	V[u].c = GRAY;
	V[u].s = ++time;
	S.push(u);
	while (!S.empty())
	{//只要栈不空，不断访问
		size_t i = S.top();
		edgeNode *curr = access_edge[i];//得到顶点i当前将要被访问的边
		while (curr != nullptr)
		{//不断循环，直到访问到一个白节点，或者顶点i的所有邻接点已被访问
			if (V[curr->adjvertex].c == WHITE)
			{//与i相邻的是白节点，即未被访问过
				V[curr->adjvertex].c = GRAY;
				V[curr->adjvertex].s = ++time;
				S.push(curr->adjvertex);//访问后入栈
				access_edge[i] = curr->nextEdge;//记下顶点i下一条将要被访问的边
				break;
			}
			else curr = curr->nextEdge;
		}
		if (curr == nullptr)
		{//顶点i的所有邻接点已被访问，则出栈
			V[i].c = BLACK;
			V[i].f = ++time;
			S.pop();
		}
	}
}

void AGraph::DFS()
{
	size_t time = 0;
	for (size_t i = 1; i != E.size(); ++i)
		if (V[i].c == WHITE)
			DFS_aux_Not_recursive(i, time);
}

void AGraph::topSort(vector<size_t> &topsort)
{
	struct vertexCompare
	{
		bool operator()(const vertex &lhs, const vertex &rhs)const
		{
			return lhs.f > rhs.f;
		}
	};
	DFS();
	sort(++V.begin(), V.end(), vertexCompare());
	for (size_t i = 1; i != V.size(); ++i)
		topsort.push_back(V[i].id);
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

const int nodenum = 5;

int main()
{
	AGraph graph(nodenum),spt(nodenum);
	graph.initGraph();
	graph.print();
	cout << endl;
	graph.Bellman_Ford(1, &spt);
	spt.print();
	getchar();
	return 0;
}