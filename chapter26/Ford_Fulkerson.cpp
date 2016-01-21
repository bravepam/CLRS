
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//Ford-Fulkerson��������������ù����������Ѱ������·����ΪEdmonds-Karp�㷨
#include<iostream>
#include<queue>
#include<fstream>
#include<vector>

#define MAX	0x7fffffff

using namespace std;
enum color{ WHITE, GRAY, BLACK };

struct edgeNode
{//�߽ڵ�
	size_t adjvertex;//�ñߵĹ����Ķ���
	int capacity;//�ߵ�ǰ����
	int flow;//�ߵ�ǰ����
	edgeNode *nextEdge;//��һ����
	edgeNode(size_t adj, int w) :adjvertex(adj), capacity(w), flow(0),nextEdge(nullptr){}
};

class AGraph
{//����ͼ
private:
	vector<edgeNode*> G;
	size_t nodenum;
public:
	AGraph(size_t n) :nodenum(n){ G.resize(n + 1); }
	void initGraph();//��ʼ������ͼ
	edgeNode* search(size_t, size_t);//���ұ�
	edgeNode* addEdge(size_t, size_t, int);//��ͼ����ӱ�
	void deleteEdge(size_t, size_t);//����ͼ��ɾ����
	bool BFS(size_t,size_t,vector<size_t>&);//�����������������һ��ǰ������
	int Edmonds_Karp(size_t,size_t);//Ford_Fulkerson������һ�־���ʵ�֣����ù��BFS��������·��
	void print();
	~AGraph();
};

bool AGraph::BFS(size_t s,size_t t, vector<size_t> &p)
{
	bool augment_path = false;//��¼�Ƿ��������·��
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
		if (u == t) augment_path = true;//�����Ա�������㣬�����
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
{//����������ά����ֱ�洢��(u,v)��flow��capacity���Ϳ���ʡȥsearch��addEdge�ˣ�
	//���Ը���Ч�ʣ�����������ʹ�ÿռ�
	vector<size_t> parent(nodenum + 1);
	while (BFS(s, t, parent))//��BFSѰ�Ҵ�s��t������·����parent��¼ǰ��
	{//������
		int increase_flow = MAX;
		size_t tmp = t;
		while (tmp != parent[tmp])
		{//��ȷ��������·���ϵ���С����
			edgeNode *curr = search(parent[tmp], tmp);
			if (curr->capacity < increase_flow) increase_flow = curr->capacity;
			tmp = parent[tmp];
		}
		tmp = nodenum;
		while (tmp != parent[tmp])
		{//Ȼ��Ը�·���ϱ�����������ͬʱ����С��ǰ����
			edgeNode *r = search(parent[tmp], tmp);
			r->flow = r->flow + increase_flow;
			r->capacity -= increase_flow;
			edgeNode *p = addEdge(tmp, parent[tmp], 0);
			p->flow = -r->flow;//�������Գ��Ի�öԳƱߵĵ�ǰ��
			p->capacity -= p->flow;//ͬʱ�����䵱ǰ����
			tmp = parent[tmp];
		}
	}
	//û������·��ʱ��˵�����ҵ������
	int maximum_flow = 0;
	edgeNode *p_s = G[s];
	while (p_s != nullptr)
	{//ͳ��Դ�������������
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
{//��ӱ�
	edgeNode *curr = search(start, end);//�Ȳ���
	if (curr == nullptr)
	{//���߲����ڣ������
		edgeNode *p = new edgeNode(end, capacity);
		p->nextEdge = G[start];
		G[start] = p;
		return p;
	}
	return curr;//������ӣ����ص�ǰ�ߵ�ַ
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
