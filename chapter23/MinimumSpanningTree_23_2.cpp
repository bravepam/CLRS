
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
#include<queue>
#include<map>
#include"FibonacciHeap.h"

#define NOPARENT 0
#define MAX	0x7fffffff

using namespace std;
enum color{ WHITE, GRAY, BLACK };

struct edgeNode
{//边节点
	size_t adjvertex;//该边的关联的顶点
	size_t weight;//边权重
	edgeNode *nextEdge;//下一条边
	edgeNode(size_t adj, size_t w) :adjvertex(adj), weight(w), nextEdge(nullptr){}
};

struct findRoot:public binary_function<vector<size_t>,size_t,size_t>
{//函数对象类，用于查询并查集
	size_t operator()(const vector<size_t> &UFS, size_t v)const
	{
		while (v != UFS[v]) v = UFS[v];
		return v;
	}
};

struct edge
{//边，和edgeNode有别
	size_t u, v;
	size_t weight;
	edge(size_t u_, size_t v_, size_t w) :u(u_), v(v_), weight(w){}
};

struct edgeRef
{//在preMST和MST23_2过程用到
	size_t u, v;//边
	size_t x, y;//及其引用边
	size_t weight;
	size_t u_map, v_map;
	edgeRef(size_t u_, size_t v_, size_t x_, size_t y_, 
		size_t w,size_t u_m = 0,size_t v_m = 0) :u(u_), v(v_), x(x_), y(y_), 
		weight(w),u_map(u_m),v_map(v_m){}
};

class AGraph
{//无向图
private:
	vector<edgeNode*> graph;
	size_t nodenum;
	void transformGraph(vector<edge>&);
	void preMST(AGraph*, AGraph*, vector<edgeRef>&);
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
	void add1Edge(size_t, size_t, size_t);//有向图中添加边
	void add2Edges(size_t, size_t, size_t);//无向图中添加边
	size_t prim(AGraph*,size_t);
	void mst23_2(AGraph *mst);
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

size_t AGraph::prim(AGraph *mst, size_t u)
{//普利姆算法求最小生成树，采用斐波那契堆。返回最小权值和；mst存储最小生成树，时间O(E+VlgV)
	vector<size_t> parent(nodenum + 1);
	//存储每个顶点在斐波那契堆中的对应节点的地址，这样便于修改距离等
	vector<fibonacci_heap_node<size_t, size_t>*> V(nodenum + 1);
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
		V[min.second] = nullptr;//置空，标志着该节点已删除
		sum += min.first;
		for (edgeNode *curr = graph[min.second]; curr; curr = curr->nextEdge)
		{//以其为中介，更新各点到MST的距离
			if (V[curr->adjvertex] != nullptr && curr->weight < V[curr->adjvertex]->key)
			{
				Q.decreaseKey(V[curr->adjvertex], curr->weight);
				parent[curr->adjvertex] = min.second;
			}
		}//将该边加入MST
		if (min.second != u) mst->add2Edges(parent[min.second], min.second, min.first);
	}
	return sum;
}

void AGraph::preMST(AGraph *T, AGraph *G, vector<edgeRef> &orig)
{//稀疏图求MST预处理，T存储mst，G存储收缩后的图，orig存储收缩后的图的边，以及它所引用的原图的边
	//和该边权值，注意该过程结束后mst并未完全求出。
	vector<color> mark(nodenum + 1);//访问标志
	vector<size_t> ufs(nodenum + 1);//并查集
	for (size_t i = 1; i <= nodenum; ++i)
	{
		mark[i] = WHITE;
		ufs[i] = i;
	}
	//-------------------------------------------------------
	for (size_t i = 1; i != graph.size(); ++i)
	{//一次扫描每个顶点
		if (mark[i] == WHITE)
		{//若未访问，
			edgeNode *curr = graph[i];
			size_t u = 0, w = MAX;
			while (curr != nullptr)
			{//则一次访问其邻接表，
				if (curr->weight < w)
				{//找到最短的边
					u = curr->adjvertex;
					w = curr->weight;
				}
				curr = curr->nextEdge;
			}
			T->add2Edges(i, u, w);//将其加入到T中成为mst的一条边
			ufs[i] = u;//并设置并查集
			mark[i] = mark[u] = BLACK;//且标为访问
		}
	}//该过程结束后，T是森林，存储了一些mst的边，森林中树的根则在ufs中可以查到
	//-------------------------------------------------------------------------
	map<size_t, size_t> V_of_G;//记录图G的顶点，即T中森林中各树的树根,键为树根编号，值为其在收缩后的图的编号
	size_t num_of_V = 0;
	for (size_t i = 1; i != ufs.size(); ++i)
	{//扫描ufs
		size_t p = findRoot()(ufs, i);//找寻各顶点的根，
		map<size_t, size_t>::iterator it = V_of_G.find(p);
		if (it == V_of_G.end())//若没有记录则加入，并一次编号为1,2,3...便于之后的处理，故用map存储
			V_of_G.insert(pair<size_t, size_t>(p, ++num_of_V));
	}
	//------------------------------------------------------------------------------
	vector<edge> E;
	transformGraph(E);//该函数在原图的邻接表中抽取所有的边
	for (size_t i = 0; i != E.size(); ++i)
	{//依次访问这些边
		size_t u_root = findRoot()(ufs, E[i].u), v_root = findRoot()(ufs, E[i].v),j;//找到改变两顶点的根
		if (u_root == v_root) continue;//若相等，说明该边已存在于mst中，则不处理，继续扫描下一条边
		for (j = 0; j != orig.size(); ++j)//否则查询是否以存入orig
			if ((orig[j].u == u_root && orig[j].v == v_root)
				|| (orig[j].u == v_root && orig[j].v == u_root)) break;
		if (j == orig.size())
		{//若没有，则添加，其中(u_root,v_root)，是G中的边，其引用的是E[i]这条边
			edgeRef er(u_root, v_root, E[i].u, E[i].v, E[i].weight);
			orig.push_back(er);
		}
		else if (E[i].weight < orig[j].weight)
		{//若存在，且新边比之前的引用边的权值更小，则更改引用边信息
			orig[j].x = E[i].u;
			orig[j].y = E[i].v;
			orig[j].weight = E[i].weight;
		}
	}//该过程结束后，orig记录了T中森林之间的联系，以及该联系引用的权值最小的边
	//------------------------------------------------------------------------
	G->editGraph(num_of_V);//根据顶点数目重新编辑收缩图G的大小
	for (size_t i = 0; i != orig.size(); ++i)
	{//根据orig，构造出图G的邻接表，此时用树根的相应编号构造图G，便于后续处理
		map<size_t, size_t>::iterator it1 = V_of_G.find(orig[i].u), it2 = V_of_G.find(orig[i].v);
		orig[i].u_map = it1->second; orig[i].v_map = it2->second;//记下orig中u和v的编号
		G->add2Edges(it1->second, it2->second, orig[i].weight);
	}
}

void AGraph::mst23_2(AGraph *T)
{//稀疏图求mst
	AGraph G;
	vector<edgeRef> orig;
	preMST(T, &G, orig);//调用预处理过程以求得MST雏形，存储于T中；收缩后的图G，以及G中的引用边orig
	AGraph mst_G(G.size());
	G.prim(&mst_G,1);//对图G用普利姆算法求出MST
	for (size_t i = 1; i != mst_G.graph.size(); ++i)
	{//依次扫描G的MST的每个顶点
		edgeNode *curr = mst_G.graph[i];
		while (curr != nullptr)
		{//若该顶点有邻接表
			size_t j;
			//由于图G的顶点是经过编号的，为1,2,3...，因而要找出它在原图中的顶点标号
			for (j = 0; j != orig.size(); ++j)
				if (i == orig[j].u_map && curr->adjvertex == orig[j].v_map)
					//找到后，在T中加入该边的的引用边————T中森林是用该引用边联系起来的
					//根据引用边的求取过程，可以知道每条引用边是联系这两棵树的最小权值边
					T->add2Edges(orig[j].x, orig[j].y, orig[j].weight);
			curr = curr->nextEdge;
		}
	}
}//结束后即构造出稀疏图的MST

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
	graph.mst23_2(&mst);
	mst.print();
	getchar();
	return 0;
}