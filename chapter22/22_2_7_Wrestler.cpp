
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//�㷨���۵�22�£�22.2-6��ְҵˤ����ָ�����⣬ʵΪ����ͼ���ж�����
//������ɫ���������������

#include<iostream>
#include<fstream>
#include<vector>
#include<queue>

using namespace std;
enum color{NOCOLOR,WHITE, BLACK };

struct edgeNode
{//�߽ڵ�
	size_t adjvertex;//�ñߵĹ����Ķ���
	int weight;//��Ȩ��
	edgeNode *nextEdge;//��һ����
	edgeNode(size_t adj, int w) :adjvertex(adj), weight(w), nextEdge(nullptr){}
};

class AGraph
{//ͼ
private:
	vector<edgeNode*> graph;
	size_t nodenum;
public:
	AGraph(size_t n) :nodenum(n)
	{
		if (n != 0) graph.resize(n + 1);
	}
	void initUGraph();//��ʼ������ͼ
	edgeNode* search(size_t, size_t);//���ұ�
	void addOneEdge(size_t, size_t, int);//����ͼ����ӱ�
	void addTwoEdge(size_t, size_t, int);//����ͼ����ӱ�
	void assignWrestler(size_t);
	void print();
	~AGraph();
};

void AGraph::initUGraph()
{
	size_t start, end;
	ifstream infile("F:\\wrestler.txt");
	while (infile >> start >> end)
		addTwoEdge(start, end, 1);
}

edgeNode* AGraph::search(size_t start, size_t end)
{
	edgeNode *curr = graph[start];
	while (curr != nullptr && curr->adjvertex != end)
		curr = curr->nextEdge;
	return curr;
}

void AGraph::addOneEdge(size_t start, size_t end, int weight = 1)
{
	edgeNode *curr = search(start, end);
	if (curr == nullptr)
	{
		edgeNode *p = new edgeNode(end, weight);
		p->nextEdge = graph[start];
		graph[start] = p;
	}
}

inline void AGraph::addTwoEdge(size_t start, size_t end, int weight = 1)
{
	addOneEdge(start, end, weight);
	addOneEdge(end, start, weight);
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
				cout << " --> " << curr->adjvertex;
				curr = curr->nextEdge;
			}
		cout << endl;
	}
}

void AGraph::assignWrestler(size_t s)
{
	queue<size_t> Q;
	vector<size_t> color(nodenum + 1);
	for (size_t i = 1; i <= nodenum; ++i)
		color[i] = NOCOLOR;
	color[s] = WHITE;
	Q.push(s);
	while (!Q.empty())
	{
		size_t u = Q.front();
		Q.pop();
		edgeNode *curr = graph[u];
		while (curr != nullptr)
		{
			if (color[curr->adjvertex] == color[u])
			{
				cout << "No solution!" << endl;
				return;
			}
			else if (color[curr->adjvertex] == NOCOLOR)
			{
				if (color[u] == WHITE) color[curr->adjvertex] = BLACK;
				else color[curr->adjvertex] = WHITE;
				Q.push(curr->adjvertex);
			}
			curr = curr->nextEdge;
		}
	}
	for (size_t i = 1; i <= nodenum; ++i)
		if (color[i] == WHITE) cout << i << " good" << endl;
		else cout << i << " bad" << endl;
}

AGraph::~AGraph()
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
	}
}

const int nodenum = 4;

int main()
{
	AGraph G(nodenum);
	G.initUGraph();
	G.print();
	cout << endl;
	G.assignWrestler(4);
	getchar();
	return 0;
}