
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
{//�߽ڵ�
	size_t adjvertex;//�ñߵĹ����Ķ���
	int weight;//��Ȩ��
	edgeNode *nextEdge;//��һ����
	edgeNode(size_t adj, int w) :adjvertex(adj), weight(w), nextEdge(nullptr){}
};

struct vertex
{
	size_t id;//����id
	color c;
	int dis;//��Դ�����
	size_t p;//������
	size_t s, f;//���ʿ�ʼ�ͽ���ʱ��
	vertex(size_t i = 0) :id(i), p(NOPARENT), c(WHITE),dis(0),s(0),f(0){}
};

class AGraph
{//����ͼ
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
	void initGraph();//��ʼ������ͼ
	edgeNode* search(size_t, size_t);//���ұ�
	void addEdge(size_t, size_t, int);//����ͼ����ӱ�
	void deleteEdge(size_t, size_t);//����ͼ��ɾ����
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
{//����ǰ����ͼ�������·����
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
{//��Ȩ��·,ʱ�临�Ӷ�O(V)
	vector<color> onCycle(nodenum + 1);
	for (size_t i = 1; i <= nodenum; ++i)
		onCycle[i] = WHITE;//����ȫ����ʼ��Ϊ��ɫ
	while (onCycle[cycle] == WHITE)
	{//��������ɫ����ʱ�˳���������ʱ�ǵڶ���ɨ�赽���ɶ϶��ö�����ڻ�·��
		onCycle[cycle] = BLACK;
		cycle = V[cycle].p;
	}
	while (onCycle[cycle] == BLACK)
	{//�Ըö���Ϊ��㣬��ʼ�����·�ϵĶ���
		onCycle[cycle] = WHITE;
		cout << cycle << ' ';
		cycle = V[cycle].p;
	}
}

bool AGraph::Bellman_Ford(size_t source,AGraph *spt)
{//Bellman-Ford�㷨���㵥Դ���·����spt�洢���·����SPT
	initVertex(source);
	for (size_t i = 1; i != nodenum; ++i)
	{//����V-1�˵���
		for (size_t j = 1; j != E.size(); ++j)
		{//ÿ�ζ������б߽���һ���ɳ�
			edgeNode *curr = E[j];
			while (curr != nullptr)
			{
				relax(j, curr->adjvertex, curr->weight);
				curr = curr->nextEdge;
			}
		}
	}
	for (size_t j = 1; j != E.size(); ++j)
	{//�ж��Ƿ��и�Ȩ��·
		edgeNode *curr = E[j];
		while (curr != nullptr)
		{
			if (V[curr->adjvertex].dis > V[j].dis + curr->weight)
			{//�����ӡ���û�·,����ֹ����
				printCycle(curr->adjvertex);
				cout << endl;
				return false;
			}
			curr = curr->nextEdge;
		}
	}
	SPT(spt);//�����������·����
	return true;
}

void AGraph::dagSP(size_t source)
{//�����޻�ͼ��Դ���·��
	vector<size_t> topsort;
	topSort(topsort);//�Ƚ����������򣬲�������˳��Ķ����Ŵ���topsort
	initVertex(source);//��ʼ������ľ���͸�����
	for (size_t i = 0; i != topsort.size(); ++i)
	{//������˳��
		edgeNode *curr = E[topsort[i]];
		while (curr != nullptr)
		{//��ÿ������ı߽���һ���ɳ�
			relax(topsort[i], curr->adjvertex, curr->weight);
			curr = curr->nextEdge;
		}
	}
	for (size_t i = 0; i != topsort.size(); ++i)
	{
		size_t index = vertexIndex(topsort[i]);
		cout << topsort[i] << '\t' << V[index].dis << endl;
	}
	//SPT(spt);//������ò�Ҫ�������·�����ˣ���Ҫ���죬�����SPT����Щ���޸ģ�
	//��Ϊ�����ź��������ڶ�Ӧ��������������ƶ��ˡ�
}

void AGraph::dijkstra(size_t source,AGraph *spt)
{//dijkstra�㷨��Դ���·����ֻ�����ڷǸ�Ȩֵ�޻�·ͼ��쳲�������ʵ��
	initVertex(source);
	fibonacci_heap<int, size_t> Q;
	vector<fibonacci_heap_node<int,size_t>*> ptr_fibo_node(nodenum + 1);//�����������Ķѽ���ַ
	for (size_t i = 1; i <= nodenum; ++i)
		ptr_fibo_node[i] = Q.insert(V[i].dis, i);
	while (!Q.empty())
	{
		pair<int, size_t> min = Q.extractMin();//ȡ�õ�ǰ��Դ���������
		ptr_fibo_node[min.second] = nullptr;//�ÿգ�����������ɾ���ö���
		edgeNode *curr = E[min.second];
		while (curr != nullptr)
		{
			relax(min.second, curr->adjvertex, curr->weight);
			if (ptr_fibo_node[curr->adjvertex] != nullptr && //����ö��㵽Դ������С
				V[curr->adjvertex].dis < ptr_fibo_node[curr->adjvertex]->key)
				Q.decreaseKey(ptr_fibo_node[curr->adjvertex], V[curr->adjvertex].dis);
			curr = curr->nextEdge;
		}
	}
	//for (size_t i = 1; i <= nodenum; ++i)
	//{//����V�ε���
	//	int min = MAX, id = 0;
	//	for (size_t j = 1; j <= nodenum; ++j)
	//	{//ÿ��ѡ��δ�������·������dis��С�Ľڵ�
	//		if (V[j].dis < min && V[j].c == WHITE)
	//		{
	//			min = V[j].dis;
	//			id = j;
	//		}
	//	}
	//	V[id].c = BLACK;//�������·����
	//	edgeNode *curr = E[id];
	//	while (curr != nullptr)
	//	{//�Ըö�������б߽���һ���ɳ�
	//		relax(id, curr->adjvertex, curr->weight);
	//		curr = curr->nextEdge;
	//	}
	//}
	SPT(spt);//����ǰ����ͼ������·�����ڽӱ�
	printASP();//��ӡԴ�㵽������·������СȨֵ��
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
	vector<edgeNode*> access_edge(E);//����ÿ��������һ���������ʵı�
	V[u].c = GRAY;
	V[u].s = ++time;
	S.push(u);
	while (!S.empty())
	{//ֻҪջ���գ����Ϸ���
		size_t i = S.top();
		edgeNode *curr = access_edge[i];//�õ�����i��ǰ��Ҫ�����ʵı�
		while (curr != nullptr)
		{//����ѭ����ֱ�����ʵ�һ���׽ڵ㣬���߶���i�������ڽӵ��ѱ�����
			if (V[curr->adjvertex].c == WHITE)
			{//��i���ڵ��ǰ׽ڵ㣬��δ�����ʹ�
				V[curr->adjvertex].c = GRAY;
				V[curr->adjvertex].s = ++time;
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