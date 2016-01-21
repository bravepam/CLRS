
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/
#include<iostream>
#include<vector>
#include<fstream>

using namespace std;
typedef vector<vector<int>> matrix;

#define MAX 0x7ffffff

ostream& operator<<(ostream &out,const matrix &m)
{//������������������������
	for (size_t i = 1; i != m.size(); ++i)
	{
		for (size_t j = 1; j != m[i].size(); ++j)
			if (m[i][j] == MAX) out << 'M' << ' ';
			else out << m[i][j] << ' ';
		out << endl;
	}
	return out;
}

inline void initMatrix(matrix &W,size_t n)
{//��ʼ�����󣬳��Խ��߳�ʼ��Ϊ0�⣬����ȫ��Ϊ�����
	W.resize(n + 1);
	for (size_t i = 1; i <= n; ++i)
		W[i].resize(n + 1, MAX);
	for (size_t i = 1; i <= n; ++i)
		W[i][i] = 0;
}

class MGraph
{//����ͼ���ڽӾ����ʾ
private:
	matrix W;//��Ȩֵ����
	size_t nodenum;
	void computePSP(MGraph&,matrix&);//һ�����·������
	void initEdge()
	{//��ʼ����Ȩֵ����
		initMatrix(W, nodenum);
	}
	void printAPSP(matrix &p)
	{
		for (size_t i = 1; i <= nodenum; ++i)
		{
			cout << "vertex " << i << "----------------" << endl;
			for (size_t j = 1; j <= nodenum; ++j)
			{
				printPSP(p, i, j);
				cout << endl;
			}
		}
	}
	void printPSP(matrix&,size_t,size_t);
public:
	MGraph(size_t n = 0) :nodenum(n){ initEdge(); }
	MGraph& operator=(const MGraph &rhs)
	{
		W = rhs.W;
		nodenum = rhs.nodenum;
		return *this;
	}
	void initGraph();//��ʼ��ͼ
	void print(){ cout << W; }
	void slowAPSP(MGraph&,matrix&);//�������ж����֮������·���������n-1��
	bool fastAPSP(MGraph&,matrix&);//APSP = all pairs shortest paths
	void Floyd_Warshall(matrix &);
	void Floyd_Warshall_Transitive_Closure(matrix &);
	//void computeParent(MGraph &,matrix &);
};

void MGraph::initGraph()
{
	ifstream infile("F:\\asp25_2.txt");
	size_t s, e;
	int w;
	while (infile >> s >> e >> w)
		W[s][e] = w;
}

void MGraph::printPSP(matrix &p, size_t i, size_t j)
{
	if (i == j) cout << i;
	else if (p[i][j] == MAX) cout << "No path from " << i << " to " << j << " exists";
	else 
	{
		printPSP(p, i, p[i][j]);
		cout << " --> " << j;
	}
}

void MGraph::computePSP(MGraph &L,matrix &p)
{
	size_t W_size = W.size();
	for (size_t i = 1; i != W_size; ++i)
	{
		for (size_t j = 1; j != W_size; ++j)
		{
			for (size_t k = 1; k != W_size; ++k)
				if (L.W[i][k] != MAX && W[k][j] != MAX && L.W[i][k] + W[k][j] < L.W[i][j])
				{//����!(L.W[i][k] == MAX && W[k][j] < 0)
					L.W[i][j] = L.W[i][k] + W[k][j];
					p[i][j] = k;
				}
		}
	}
}

void MGraph::slowAPSP(MGraph &L,matrix &p)
{//mΪ���·���߳������ӱ߳�Ϊ1һֱ���㵽�߳�Ϊn-1�����һ�˵õ����ս��,ʱ��O(n^4)
	size_t W_size = W.size();
	L.W = W;
	for (size_t i = 1; i != W_size; ++i)//�����·����Ϊ1ʱ��·��i->j��j��ǰ��Ϊi
		for (size_t j = 1; j != W_size; ++j)
			if (i != j && W[i][j] < MAX) p[i][j] = i;
	for (size_t m = 2; m != W_size - 1; ++m)
		computePSP(L, p);//��̬�滮˼��
	printAPSP(p);
}

bool MGraph::fastAPSP(MGraph &L,matrix &p)
{
	size_t W_size = W.size();
	L.W = W;
	for (size_t i = 1; i != W_size; ++i)//ͬ����
		for (size_t j = 1; j != W_size; ++j)
			if (i != j && W[i][j] < MAX) p[i][j] = i;
	for (size_t m = 1; m < W_size - 1; m *= 2)
		L.computePSP(L,p);
	for (size_t i = 1; i != W_size; ++i)
	{//����i
		for (size_t j = 1; j != W_size; ++j)
		{//��j�����·��
			if (i != j && p[i][j] != MAX && L.W[i][j] > L.W[i][p[i][j]] + W[p[i][j]][j])
			{//���������������˵���и�Ȩ��·
				cout << "Negative weight cycle exists" << endl;
				return false;
			}
		}
	}
	printAPSP(p);
	return true;
}

void MGraph::Floyd_Warshall(matrix &p)
{
	size_t W_size = W.size();
	for (size_t i = 1; i != W_size; ++i)
		for (size_t j = 1; j != W_size; ++j)
			if (i != j && W[i][j] < MAX) p[i][j] = i;
	for (size_t k = 1; k != W_size; ++k)
		for (size_t i = 1; i != W_size; ++i)
			for (size_t j = 1; j != W_size;++j)
				if (W[i][k] != MAX && W[k][j] != MAX && W[i][k] + W[k][j] < W[i][j])
				{
					W[i][j] = W[i][k] + W[k][j];
					p[i][j] = p[k][j];
				}
	printAPSP(p);
	cout << W << endl;
}

void MGraph::Floyd_Warshall_Transitive_Closure(matrix &t)
{
	size_t W_size = W.size();
	for (size_t i = 1; i != W_size; ++i)
	{
		for (size_t j = 1; j != W_size; ++j)
			if (W[i][j] < MAX) t[i][j] = 1;
			else t[i][j] = 0;
	}
	for (size_t k = 1; k != W_size; ++k)
		for (size_t i = 1; i != W_size; ++i)
			for (size_t j = 1; j != W_size; ++j)
				t[i][j] = t[i][j] | (t[i][k] & t[k][j]);
}

//void MGraph::computeParent(MGraph &L,matrix &p)
//{
//	size_t W_size = L.W.size();
//	for (size_t i = 1; i != W_size; ++i)
//	{
//		for (size_t j = 1; j != W_size; ++j)
//		{
//			if (i == j) continue;
//			for (size_t k = 1; k != W_size; ++k)
//				if ((k != j) && (L.W[i][j] == L.W[i][k] + W[k][j]))
//					p[i][j] = k;
//		}
//	}
//}

const int nodenum = 6;

int main()
{
	MGraph graph(nodenum);
	matrix p;
	initMatrix(p, nodenum);
	graph.initGraph();
	graph.print();
	cout << endl;
	graph.Floyd_Warshall(p);
	cout << endl << p;
	getchar();
	return 0;
}