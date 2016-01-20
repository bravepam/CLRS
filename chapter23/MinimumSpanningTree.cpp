
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<algorithm>
#include<fstream>
#include<vector>
#include<utility>
#include"FibonacciHeap.h"

//#define NOPARENT 0
#define MAX	0x7fffffff

using namespace std;
//enum color{ WHITE, GRAY, BLACK };

struct edgeNode
{//边节点
	size_t adjvertex;//该边的关联的顶点
	size_t weight;//边权重
	edgeNode *nextEdge;//下一条边
	edgeNode(size_t adj, size_t w) :adjvertex(adj), weight(w), nextEdge(nullptr){}
};

struct edge
{//边，和edgeNode有别
	size_t u, v;
	size_t weight;
	edge(size_t u_, size_t v_, size_t w) :u(u_), v(v_), weight(w){}
};


class AGraph
{//无向图
private:
	vector<edgeNode*> graph;
	size_t nodenum;
	void transformGraph(vector<edge>&);
public:
	AGraph(size_t n = 0){editGraph(n); }
	void editGraph(size_t n)
	{
		nodenum = n;
		graph.resize(n + 1);
	}
	size_t size()const { return nodenum; }
	void initGraph();//初始化无向图
	edgeNode* search(size_t, size_t);//查找边
	void add1Edge(size_t, size_t, size_t);
	void add2Edges(size_t, size_t, size_t);//添加边
	void delete1Edge(size_t, size_t);
	void delete2Edges(size_t, size_t);//删除边
	size_t kruskal(AGraph*);
	size_t prim(AGraph*,size_t);
	void print();
	void destroy();
	~AGraph(){ destroy(); }
};

void AGraph::initGraph()
{
	size_t start, end;
	size_t w;
	ifstream infile("F:\\mst.txt");
	while (infile >> start >> end >> w)
		add1Edge(start, end, w);
}

void AGraph::transformGraph(vector<edge> &E)
{
	for (size_t i = 1; i != graph.size(); ++i)
	{//改造edgeNode，变成edge
		edgeNode *curr = graph[i];
		while (curr != nullptr)
		{
			if (i < curr->adjvertex)
			{//顶点u,v之间的边只存储一条，(u,v)，且u < v。
				edge e(i, curr->adjvertex, curr->weight);
				E.push_back(e);
			}
			curr = curr->nextEdge;
		}
	}
}

edgeNode* AGraph::search(size_t start, size_t end)
{
	edgeNode *curr = graph[start];
	while (curr != nullptr && curr->adjvertex != end)
		curr = curr->nextEdge;
	return curr;
}

void AGraph::add1Edge(size_t start, size_t end, size_t weight)
{
	edgeNode *curr = search(start, end);
	if (curr == nullptr)
	{
		edgeNode *p = new edgeNode(end, weight);
		p->nextEdge = graph[start];
		graph[start] = p;
	}
}

inline void AGraph::add2Edges(size_t start, size_t end, size_t weight)
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
			graph[start] = curr->nextEdge;
			delete curr;
		}
		else
		{
			edgeNode *pre = graph[start];
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

size_t AGraph::kruskal(AGraph *mst)
{//克鲁斯卡尔算法求最小生成树，返回最小权值和，最小生成树记录在mst中
	struct findRoot:public binary_function<vector<size_t>,size_t,size_t>
	{//局部函数对象类，用于查询并查集
		size_t operator()(const vector<size_t> &UFS, size_t v)const
		{
			while (v != UFS[v]) v = UFS[v];
			return v;
		}
	};
	struct edgeCompare:public binary_function<edge,edge,bool>
	{//局部函数对象类，用于边比较器
		bool operator()(const edge &left, const edge &right)const
		{
			return left.weight < right.weight;
		}
	};
	vector<edge> E;
	transformGraph(E);//将邻接链表转换为边集合
	vector<size_t> UFS(nodenum + 1);
	size_t sum = 0;
	for (size_t i = 1; i <= nodenum; ++i)
		UFS[i] = i;//初始化并查集Union-Find-Set
	sort(E.begin(), E.end(), edgeCompare());//对edge边按权值非递减排序
	for (size_t i = 0; i != E.size(); ++i)
	{//逐条考察边
		size_t u_root = findRoot()(UFS, E[i].u), v_root = findRoot()(UFS, E[i].v);
		if (u_root != v_root)
		{//若属于不同的树，即根节点不一样，则加入
			sum += E[i].weight;
			mst->add2Edges(E[i].u, E[i].v, E[i].weight);
			UFS[u_root] = v_root;
		}
	}
	return sum;
}

size_t AGraph::prim(AGraph *mst,size_t u)
{//普利姆算法求最小生成树，采用斐波那契堆。返回最小权值和；mst存储最小生成树，时间O(E+VlgV)
	vector<size_t> parent(nodenum + 1);
	//存储每个顶点在斐波那契堆中的对应节点的地址，这样便于修改距离等
	vector<fibonacci_heap_node<size_t,size_t>*> V(nodenum + 1);
	fibonacci_heap<size_t, size_t> Q;//斐波那契堆，键为距离，值为顶点标号
	for (size_t i = 1; i <= nodenum; ++i)
	{
		parent[i] = i;
		if (i == u) V[i] = Q.insert(0, i);//向堆中插入元素，并且将节点句柄存入数组
		else V[i] = Q.insert(MAX, i);
	}
	size_t sum = 0;
	while (!Q.empty())
	{
		pair<size_t, size_t> min = Q.extractMin();
		V[min.second] = nullptr;//置空，标志着该节点已从图中删除
		sum += min.first;
		for (edgeNode *curr = graph[min.second]; curr; curr = curr->nextEdge)
		{//以其为中介，更新各点到MST的距离
			if (V[curr->adjvertex] != nullptr && curr->weight < V[curr->adjvertex]->key)
			{
				Q.decreaseKey(V[curr->adjvertex], curr->weight);
				parent[curr->adjvertex] = min.second;
			}
		}//将该边加入MST
		if (min.second != u) mst->add2Edges(parent[min.second], min.second,min.first);
	}
	return sum;
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
				cout << " --<" << curr->weight << ">--> " << curr->adjvertex;
				curr = curr->nextEdge;
			}
		cout << endl;
	}
}

void AGraph::destroy()
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
		graph[i] = curr;
	}
}

const size_t nodenum = 9;

size_t main()
{
	AGraph graph(nodenum), mst(nodenum);
	graph.initGraph();
	graph.print();
	cout << endl;
	cout << graph.prim(&mst,5) << endl;
	mst.print();
	getchar();
	return 0;
}