
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

#define NOPARENT 0
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
	size_t id, c;
	size_t d, f;
	size_t p;
	vertex(size_t i = 0) :id(i), c(WHITE), p(NOPARENT), d(0), f(0){}
};

class AGraph
{//图
private:
	vector<edgeNode*> E;//存储图的边信息，索引即为顶点id
	vector<vertex> V;//存储图的顶点信息
	size_t nodenum;
	void printEdge();
	void printVertex();
	void DFS_aux_Not_recursive(size_t, size_t &);//非递归的DFS辅助函数

	AGraph& operator=(const AGraph&);
	AGraph(const AGraph&);
public:
	AGraph(size_t n) :nodenum(n)
	{
		E.resize(n + 1);
		V.resize(n + 1);
		for (size_t i = 1; i <= nodenum; ++i)
			V[i].id = i;
	}
	void initDGraph();//初始化有向图
	edgeNode* search(size_t, size_t);//查找边
	void addEdge(size_t, size_t, int);//有向图中添加边
	void pathNumber(size_t, size_t, int&);//求两个顶点之间互通的路径数目
	void DFS();
	void print();
	void topSort();//输出一个拓扑排序序列
	~AGraph();
};


void AGraph::initDGraph()
{
	size_t start, end;
	ifstream infile("F:\\topsort.txt");//须为无环图
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

void AGraph::pathNumber(size_t s, size_t e, int &num)
{
	edgeNode *curr = E[s];
	while (curr != nullptr)
	{
		if (curr->adjvertex == e)//若与节点s相邻的是e，则路径数增1
			++num;
		else pathNumber(curr->adjvertex, e, num);//否则以该节点为起点继续搜寻
		curr = curr->nextEdge;
	}
}

inline void AGraph::printEdge()
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

inline void AGraph::printVertex()
{
	cout << "vertex ID" << endl;
	for (size_t i = 1; i != V.size(); ++i)
		printf("%-4d ", V[i].id);
	cout << endl << "vertex color" << endl;
	for (size_t i = 1; i != V.size(); ++i)
		printf("%-4d ", V[i].c);
	cout << endl << "vertex parent" << endl;
	for (size_t i = 1; i != V.size(); ++i)
		printf("%-4d ", V[i].p);
	cout << endl << "vertex start time" << endl;
	for (size_t i = 1; i != V.size(); ++i)
		printf("%-4d ", V[i].d);
	cout << endl << "vertex finish time" << endl;
	for (size_t i = 1; i != V.size(); ++i)
		printf("%-4d ", V[i].f);
	cout << endl;
}

inline void AGraph::print()
{
	printEdge();
	printVertex();
}

void AGraph::DFS_aux_Not_recursive(size_t u, size_t &time)
{
	stack<size_t> S;
	vector<edgeNode*> access_edge(E);//记下每个顶点下一条将被访问的边
	V[u].c = GRAY;
	V[u].d = ++time;
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
				V[curr->adjvertex].d = ++time;
				V[curr->adjvertex].p = i;
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

void AGraph::topSort()
{
	DFS();//先DFS取的每个节点访问结束时间
	struct compare
	{//局部函数对象，用于根据访问结束时间排序顶点
		bool operator()(const vertex &lhs, const vertex &rhs)const
		{
			return lhs.f > rhs.f;
		}
	};
	sort(++V.begin(), V.end(), compare());//按访问结束时间降序排序
	for (size_t i = 1; i != V.size(); ++i)
		cout << V[i].id << ' ';
	cout << endl;
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

const int nodenum = 9;

int main()
{
	AGraph G(nodenum);
	int num = 0;
	G.initDGraph();
	G.topSort();
	G.pathNumber(1, 3, num);
	cout << num << endl;
	getchar();
	return 0;
}