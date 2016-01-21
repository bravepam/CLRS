
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//Ford-Fulkerson方法求最大流，用广度优先搜索寻找增广路径，为Edmonds-Karp算法
#include<iostream>
#include<queue>
#include<fstream>
#include<vector>

#define MAX	0x7fffffff

using namespace std;
enum color{ WHITE, GRAY, BLACK };

struct edgeNode
{//边节点
	size_t adjvertex;//该边的关联的顶点
	int capacity;//边当前容量
	int flow;//边当前流量
	edgeNode *nextEdge;//下一条边
	edgeNode(size_t adj, int w) :adjvertex(adj), capacity(w), flow(0),nextEdge(nullptr){}
};

class AGraph
{//有向图
private:
	vector<edgeNode*> G;
	size_t nodenum;
public:
	AGraph(size_t n) :nodenum(n){ G.resize(n + 1); }
	void initGraph();//初始化有向图
	edgeNode* search(size_t, size_t);//查找边
	edgeNode* addEdge(size_t, size_t, int);//向图中添加边
	void deleteEdge(size_t, size_t);//有向图中删除边
	bool BFS(size_t,size_t,vector<size_t>&);//广度优先搜索，返回一个前驱数组
	int Edmonds_Karp(size_t,size_t);//Ford_Fulkerson方法的一种具体实现，采用广度BFS搜索增广路经
	void print();
	~AGraph();
};

bool AGraph::BFS(size_t s,size_t t, vector<size_t> &p)
{
	bool augment_path = false;//记录是否存在增广路径
	queue<size_t> Q;
	vector<size_t> dis(nodenum + 1), color(nodenum + 1);
	for (size_t i = 1; i <= nodenum; ++i)
	{
		dis[i] = MAX;
		p[i] = i;
		color[i] = WHITE;
	}
	color[s] = GRAY;
	dis[s] = 0;
	p[s] = s;
	Q.push(s);
	while (!Q.empty())
	{
		size_t u = Q.front();
		if (u == t) augment_path = true;//若可以遍历到汇点，则存在
		Q.pop();
		edgeNode *curr = G[u];
		while (curr != nullptr)
		{
			if (color[curr->adjvertex] == WHITE && curr->capacity > 0)
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
	return augment_path;
}

int AGraph::Edmonds_Karp(size_t s,size_t t)
{//若用两个二维数组分别存储边(u,v)的flow和capacity，就可以省去search和addEdge了，
	//可以改善效率，不过增加了使用空间
	vector<size_t> parent(nodenum + 1);
	while (BFS(s, t, parent))//用BFS寻找从s到t的增广路径，parent记录前驱
	{//若存在
		int increase_flow = MAX;
		size_t tmp = t;
		while (tmp != parent[tmp])
		{//则确定该增广路径上的最小容量
			edgeNode *curr = search(parent[tmp], tmp);
			if (curr->capacity < increase_flow) increase_flow = curr->capacity;
			tmp = parent[tmp];
		}
		tmp = nodenum;
		while (tmp != parent[tmp])
		{//然后对该路径上边增加流量，同时将减小当前容量
			edgeNode *r = search(parent[tmp], tmp);
			r->flow = r->flow + increase_flow;
			r->capacity -= increase_flow;
			edgeNode *p = addEdge(tmp, parent[tmp], 0);
			p->flow = -r->flow;//根据流对称性获得对称边的当前流
			p->capacity -= p->flow;//同时更新其当前容量
			tmp = parent[tmp];
		}
	}
	//没有增广路径时，说明已找到最大流
	int maximum_flow = 0;
	edgeNode *p_s = G[s];
	while (p_s != nullptr)
	{//统计源点的流总量即可
		maximum_flow += p_s->flow;
		p_s = p_s->nextEdge;
	}
	return maximum_flow;
}

void AGraph::initGraph()
{
	size_t start, end;
	int w = 1;
	ifstream infile("F:\\maximummatch.txt");
	while (infile >> start >> end/* >> w*/)
		addEdge(start, end, w);
}

edgeNode* AGraph::search(size_t start, size_t end)
{
	edgeNode *curr = G[start];
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
		p->nextEdge = G[start];
		G[start] = p;
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
			G[start] = curr->nextEdge;
			delete curr;
		}
		else
		{
			edgeNode *pre = G[start];
			while (pre->nextEdge->adjvertex != end)
				pre = pre->nextEdge;
			pre->nextEdge = curr->nextEdge;
			delete curr;
		}
	}
}

inline void AGraph::print()
{
	for (size_t i = 1; i != G.size(); ++i)
	{
		edgeNode *curr = G[i];
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
	for (size_t i = 1; i != G.size(); ++i)
	{
		edgeNode *curr = G[i], *pre;
		while (curr != nullptr)
		{
			pre = curr;
			curr = curr->nextEdge;
			delete pre;
		}
	}
}

const int nodenum = 11;

int main()
{
	AGraph graph(nodenum);
	graph.initGraph();
	graph.print();
	cout << endl;
	int maximum_flow = graph.Edmonds_Karp(1, nodenum);
	cout << "The maximum flow is " << maximum_flow << endl;
	getchar();
	return 0;
}
