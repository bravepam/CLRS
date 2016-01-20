
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
{//�߽ڵ�
	size_t adjvertex;//�ñߵĹ����Ķ���
	size_t weight;//��Ȩ��
	edgeNode *nextEdge;//��һ����
	edgeNode(size_t adj, size_t w) :adjvertex(adj), weight(w), nextEdge(nullptr){}
};

struct findRoot:public binary_function<vector<size_t>,size_t,size_t>
{//���������࣬���ڲ�ѯ���鼯
	size_t operator()(const vector<size_t> &UFS, size_t v)const
	{
		while (v != UFS[v]) v = UFS[v];
		return v;
	}
};

struct edge
{//�ߣ���edgeNode�б�
	size_t u, v;
	size_t weight;
	edge(size_t u_, size_t v_, size_t w) :u(u_), v(v_), weight(w){}
};

struct edgeRef
{//��preMST��MST23_2�����õ�
	size_t u, v;//��
	size_t x, y;//�������ñ�
	size_t weight;
	size_t u_map, v_map;
	edgeRef(size_t u_, size_t v_, size_t x_, size_t y_, 
		size_t w,size_t u_m = 0,size_t v_m = 0) :u(u_), v(v_), x(x_), y(y_), 
		weight(w),u_map(u_m),v_map(v_m){}
};

class AGraph
{//����ͼ
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
	void initGraph();//��ʼ������ͼ
	edgeNode* search(size_t, size_t);//���ұ�
	void add1Edge(size_t, size_t, size_t);//����ͼ����ӱ�
	void add2Edges(size_t, size_t, size_t);//����ͼ����ӱ�
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
	{//����edgeNode�����edge
		edgeNode *curr = graph[i];
		while (curr != nullptr)
		{
			if (i < curr->adjvertex)
			{//����u,v֮��ı�ֻ�洢һ����(u,v)����u < v��
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
{//����ķ�㷨����С������������쳲������ѡ�������СȨֵ�ͣ�mst�洢��С��������ʱ��O(E+VlgV)
	vector<size_t> parent(nodenum + 1);
	//�洢ÿ��������쳲��������еĶ�Ӧ�ڵ�ĵ�ַ�����������޸ľ����
	vector<fibonacci_heap_node<size_t, size_t>*> V(nodenum + 1);
	fibonacci_heap<size_t, size_t> Q;//쳲������ѣ���Ϊ���룬ֵΪ������
	for (size_t i = 1; i <= nodenum; ++i)
	{
		parent[i] = i;
		if (i == u) V[i] = Q.insert(0, i);//����в���Ԫ�أ����ҽ��ڵ�����������
		else V[i] = Q.insert(MAX, i);
	}
	size_t sum = 0;
	while (!Q.empty())
	{
		pair<size_t, size_t> min = Q.extractMin();
		V[min.second] = nullptr;//�ÿգ���־�Ÿýڵ���ɾ��
		sum += min.first;
		for (edgeNode *curr = graph[min.second]; curr; curr = curr->nextEdge)
		{//����Ϊ�н飬���¸��㵽MST�ľ���
			if (V[curr->adjvertex] != nullptr && curr->weight < V[curr->adjvertex]->key)
			{
				Q.decreaseKey(V[curr->adjvertex], curr->weight);
				parent[curr->adjvertex] = min.second;
			}
		}//���ñ߼���MST
		if (min.second != u) mst->add2Edges(parent[min.second], min.second, min.first);
	}
	return sum;
}

void AGraph::preMST(AGraph *T, AGraph *G, vector<edgeRef> &orig)
{//ϡ��ͼ��MSTԤ����T�洢mst��G�洢�������ͼ��orig�洢�������ͼ�ıߣ��Լ��������õ�ԭͼ�ı�
	//�͸ñ�Ȩֵ��ע��ù��̽�����mst��δ��ȫ�����
	vector<color> mark(nodenum + 1);//���ʱ�־
	vector<size_t> ufs(nodenum + 1);//���鼯
	for (size_t i = 1; i <= nodenum; ++i)
	{
		mark[i] = WHITE;
		ufs[i] = i;
	}
	//-------------------------------------------------------
	for (size_t i = 1; i != graph.size(); ++i)
	{//һ��ɨ��ÿ������
		if (mark[i] == WHITE)
		{//��δ���ʣ�
			edgeNode *curr = graph[i];
			size_t u = 0, w = MAX;
			while (curr != nullptr)
			{//��һ�η������ڽӱ�
				if (curr->weight < w)
				{//�ҵ���̵ı�
					u = curr->adjvertex;
					w = curr->weight;
				}
				curr = curr->nextEdge;
			}
			T->add2Edges(i, u, w);//������뵽T�г�Ϊmst��һ����
			ufs[i] = u;//�����ò��鼯
			mark[i] = mark[u] = BLACK;//�ұ�Ϊ����
		}
	}//�ù��̽�����T��ɭ�֣��洢��һЩmst�ıߣ�ɭ�������ĸ�����ufs�п��Բ鵽
	//-------------------------------------------------------------------------
	map<size_t, size_t> V_of_G;//��¼ͼG�Ķ��㣬��T��ɭ���и���������,��Ϊ������ţ�ֵΪ�����������ͼ�ı��
	size_t num_of_V = 0;
	for (size_t i = 1; i != ufs.size(); ++i)
	{//ɨ��ufs
		size_t p = findRoot()(ufs, i);//��Ѱ������ĸ���
		map<size_t, size_t>::iterator it = V_of_G.find(p);
		if (it == V_of_G.end())//��û�м�¼����룬��һ�α��Ϊ1,2,3...����֮��Ĵ�������map�洢
			V_of_G.insert(pair<size_t, size_t>(p, ++num_of_V));
	}
	//------------------------------------------------------------------------------
	vector<edge> E;
	transformGraph(E);//�ú�����ԭͼ���ڽӱ��г�ȡ���еı�
	for (size_t i = 0; i != E.size(); ++i)
	{//���η�����Щ��
		size_t u_root = findRoot()(ufs, E[i].u), v_root = findRoot()(ufs, E[i].v),j;//�ҵ��ı�������ĸ�
		if (u_root == v_root) continue;//����ȣ�˵���ñ��Ѵ�����mst�У��򲻴�������ɨ����һ����
		for (j = 0; j != orig.size(); ++j)//�����ѯ�Ƿ��Դ���orig
			if ((orig[j].u == u_root && orig[j].v == v_root)
				|| (orig[j].u == v_root && orig[j].v == u_root)) break;
		if (j == orig.size())
		{//��û�У�����ӣ�����(u_root,v_root)����G�еıߣ������õ���E[i]������
			edgeRef er(u_root, v_root, E[i].u, E[i].v, E[i].weight);
			orig.push_back(er);
		}
		else if (E[i].weight < orig[j].weight)
		{//�����ڣ����±߱�֮ǰ�����ñߵ�Ȩֵ��С����������ñ���Ϣ
			orig[j].x = E[i].u;
			orig[j].y = E[i].v;
			orig[j].weight = E[i].weight;
		}
	}//�ù��̽�����orig��¼��T��ɭ��֮�����ϵ���Լ�����ϵ���õ�Ȩֵ��С�ı�
	//------------------------------------------------------------------------
	G->editGraph(num_of_V);//���ݶ�����Ŀ���±༭����ͼG�Ĵ�С
	for (size_t i = 0; i != orig.size(); ++i)
	{//����orig�������ͼG���ڽӱ���ʱ����������Ӧ��Ź���ͼG�����ں�������
		map<size_t, size_t>::iterator it1 = V_of_G.find(orig[i].u), it2 = V_of_G.find(orig[i].v);
		orig[i].u_map = it1->second; orig[i].v_map = it2->second;//����orig��u��v�ı��
		G->add2Edges(it1->second, it2->second, orig[i].weight);
	}
}

void AGraph::mst23_2(AGraph *T)
{//ϡ��ͼ��mst
	AGraph G;
	vector<edgeRef> orig;
	preMST(T, &G, orig);//����Ԥ������������MST���Σ��洢��T�У��������ͼG���Լ�G�е����ñ�orig
	AGraph mst_G(G.size());
	G.prim(&mst_G,1);//��ͼG������ķ�㷨���MST
	for (size_t i = 1; i != mst_G.graph.size(); ++i)
	{//����ɨ��G��MST��ÿ������
		edgeNode *curr = mst_G.graph[i];
		while (curr != nullptr)
		{//���ö������ڽӱ�
			size_t j;
			//����ͼG�Ķ����Ǿ�����ŵģ�Ϊ1,2,3...�����Ҫ�ҳ�����ԭͼ�еĶ�����
			for (j = 0; j != orig.size(); ++j)
				if (i == orig[j].u_map && curr->adjvertex == orig[j].v_map)
					//�ҵ�����T�м���ñߵĵ����ñߡ�������T��ɭ�����ø����ñ���ϵ������
					//�������ñߵ���ȡ���̣�����֪��ÿ�����ñ�����ϵ������������СȨֵ��
					T->add2Edges(orig[j].x, orig[j].y, orig[j].weight);
			curr = curr->nextEdge;
		}
	}
}//�����󼴹����ϡ��ͼ��MST

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