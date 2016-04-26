
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<utility>
#include"BinomialHeap.h"

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

class AGraph
{//无向图
private:
	vector<edgeNode*> graph;
	size_t nodenum;

	AGraph& operator=(const AGraph&);
	AGraph(const AGraph&);
public:
	AGraph(size_t n = 0){ editGraph(n); }
	void editGraph(size_t n)
	{
		nodenum = n;
		graph.resize(n + 1);
	}
	size_t size()const { return nodenum; }
	void initGraph();//初始化无向图
	edgeNode* search(size_t, size_t);//查找边
	void addOneEdge(size_t, size_t, size_t);
	void addTwoEdge(size_t, size_t, size_t);//无向图中添加边
	void deleteOneEdge(size_t, size_t);
	void deleteTwoEdge(size_t, size_t);//无向图中删除边
	void BHMST(AGraph&);//采用二项堆binomial heap计算MST
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
		addOneEdge(start, end, w);
}

edgeNode* AGraph::search(size_t start, size_t end)
{
	edgeNode *curr = graph[start];
	while (curr != nullptr && curr->adjvertex != end)
		curr = curr->nextEdge;
	return curr;
}

void AGraph::addOneEdge(size_t start, size_t end, size_t weight)
{
	edgeNode *curr = search(start, end);
	if (curr == nullptr)
	{
		edgeNode *p = new edgeNode(end, weight);
		p->nextEdge = graph[start];
		graph[start] = p;
	}
}

inline void AGraph::addTwoEdge(size_t start, size_t end, size_t weight)
{
	addOneEdge(start, end, weight);
	addOneEdge(end, start, weight);
}

void AGraph::deleteOneEdge(size_t start, size_t end)
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

inline void AGraph::deleteTwoEdge(size_t start, size_t end)
{
	deleteOneEdge(start, end);
	deleteOneEdge(end, start);
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

void AGraph::BHMST(AGraph &mst)
{
	struct edge
	{//局部类，边
		size_t start, finish;//包含边起点和终点
		edge(size_t s, size_t f) :start(s), finish(f){}
	};
	vector<binomial_heap<size_t, edge>> E(nodenum + 1);//为每个顶点的邻接链表建立一个二项堆，键为边权值
	vector<size_t> parent(nodenum + 1);//前驱子图
	vector<color> isDestroy(nodenum + 1);//记录顶点集合是否已销毁（合并入其他集合）
	for (size_t i = 1; i <= nodenum; ++i)
	{//初始化
		parent[i] = i;
		isDestroy[i] = WHITE;
		edgeNode *curr = graph[i];
		while (curr != nullptr)
		{
			E[i].insert(curr->weight, edge(i, curr->adjvertex));
			curr = curr->nextEdge;
		}
	}
	struct findRoot:public binary_function<vector<size_t>,size_t,size_t>
	{//局部函数对象类，查找子树根，并查集
		size_t operator()(const vector<size_t> &UFS, size_t v)const
		{
			while (UFS[v] != v) v = UFS[v];
			return v;
		}
	};
	struct getVertexSet :public unary_function <vector<color>, size_t >
	{//局部函数对象类，找寻未被销毁的顶点集合，返回其编号
		size_t operator()(const vector<color> &isDestroy)const
		{
			size_t set_num = 0;
			for (size_t i = 1; i != isDestroy.size();++i)
				if (isDestroy[i] == WHITE)
				{
					set_num = i;
					break;
				}
			return set_num;
		}
	};
	size_t vertex_set_total = nodenum;
	while (vertex_set_total > 1)
	{//不断迭代，直到只剩下一个顶点集合
		size_t vertex_set_num = getVertexSet()(isDestroy);//获得顶点集合
		pair<size_t, edge> min = E[vertex_set_num].extractMin();//取的对应地边集，即二项堆的最小权值及边
		size_t v_root = findRoot()(parent, min.second.finish);
		if (vertex_set_num != v_root)
		{//若两端点所处子树不同
			mst.addTwoEdge(min.second.start,min.second.finish, min.first);//那么加入到mst中
			parent[v_root] = vertex_set_num;//并且合并两顶点集
			isDestroy[v_root] = BLACK;//设置颜色表明销毁
			E[vertex_set_num].BheapUnion(E[v_root]);//合并相对应的边集
			--vertex_set_total;//顶点集数目减1
		}
	}
}

const int nodenum = 9;

int main()
{
	AGraph graph(nodenum), mst(nodenum);
	graph.initGraph();
	graph.print();
	cout << endl;
	graph.BHMST(mst);
	mst.print();
	getchar();
	return 0;
}