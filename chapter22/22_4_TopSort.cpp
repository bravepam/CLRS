
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
{//�߽ڵ�
	size_t adjvertex;//�ñߵĹ����Ķ���
	int weight;//��Ȩ��
	edgeNode *nextEdge;//��һ����
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
{//ͼ
private:
	vector<edgeNode*> E;//�洢ͼ�ı���Ϣ��������Ϊ����id
	vector<vertex> V;//�洢ͼ�Ķ�����Ϣ
	size_t nodenum;
	void printEdge();
	void printVertex();
	void DFS_aux_Not_recursive(size_t, size_t &);//�ǵݹ��DFS��������

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
	void initDGraph();//��ʼ������ͼ
	edgeNode* search(size_t, size_t);//���ұ�
	void addEdge(size_t, size_t, int);//����ͼ����ӱ�
	void pathNumber(size_t, size_t, int&);//����������֮�以ͨ��·����Ŀ
	void DFS();
	void print();
	void topSort();//���һ��������������
	~AGraph();
};


void AGraph::initDGraph()
{
	size_t start, end;
	ifstream infile("F:\\topsort.txt");//��Ϊ�޻�ͼ
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
		if (curr->adjvertex == e)//����ڵ�s���ڵ���e����·������1
			++num;
		else pathNumber(curr->adjvertex, e, num);//�����Ըýڵ�Ϊ��������Ѱ
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
	vector<edgeNode*> access_edge(E);//����ÿ��������һ���������ʵı�
	V[u].c = GRAY;
	V[u].d = ++time;
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
				V[curr->adjvertex].d = ++time;
				V[curr->adjvertex].p = i;
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

void AGraph::topSort()
{
	DFS();//��DFSȡ��ÿ���ڵ���ʽ���ʱ��
	struct compare
	{//�ֲ������������ڸ��ݷ��ʽ���ʱ�����򶥵�
		bool operator()(const vertex &lhs, const vertex &rhs)const
		{
			return lhs.f > rhs.f;
		}
	};
	sort(++V.begin(), V.end(), compare());//�����ʽ���ʱ�併������
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