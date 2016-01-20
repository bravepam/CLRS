
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
{//����
	size_t id, c;//��ţ���ɫ
	size_t d, f;//���ʿ�ʼ�ͽ���ʱ��
	size_t p;//���ڵ���
	vertex(size_t i = 0) :id(i), c(WHITE), p(NOPARENT), d(0), f(0){}
};

struct SCCvertex
{//ǿ��ͨ��֧����
	size_t sccID;//ǿ��ͨ��֧���
	vector<size_t> sccSet;//��ǿ��ͨ��֧�����Ķ���
	void print()
	{
		cout << "SCC " << sccID << " includes vertex : ";
		for (size_t i = 0; i != sccSet.size(); ++i)
			cout << sccSet[i] << ' ';
		cout << endl;
	}
};

class AGraph
{//ͼ
private:
	//����Ϣ�����Ͷ�����Ϣ������������1��ʼ�洢��Ϣ��ͬʱ���е�ͼ����Ҳ��1��ʼ��ţ��������������i��Ӧ��
	//���Ƕ���i�������Ϣ�����������ж����ź������Ķ�Ӧ��ϵ����Ҫ�ƶ�����Ĳ����»��λ��������һ�㲻��
	vector<edgeNode*> E;
	vector<vertex> V;
	size_t nodenum;//������
	void printEdge();
	void printVertex();
	void SCC_DFS(vector<SCCvertex>&);//��ǿ��ͨ��֧
	void DFS_aux_Not_recursive(size_t, size_t &);//�ǵݹ��DFS��������
	void SCC_DFS_aux(size_t,SCCvertex&);//��ǿ��ͨ��֧�����������޸ĵݹ��DFS����
	size_t vertexIndex(size_t id)
	{//����id�Ŷ�����ڶ��������е�����
		size_t id_index;
		for (size_t i = 1; i != V.size(); ++i)
			if (V[i].id == id) id_index = i;
		return id_index;
	}
	void init(size_t n)
	{
		E.resize(n);
		V.resize(n);
		for (size_t i = 1; i != n; ++i)
			V[i].id = i;
	}
	void editGraph(size_t n) { init(n + 1); }//�޸�ͼ��С������ʼ��
public:
	AGraph(size_t n = 0) :nodenum(n){ init(n + 1); }
	void initGraph();//��ʼ������ͼ
	edgeNode* search(size_t, size_t);//���ұ�
	void addEdge(size_t, size_t, int);//����ͼ����ӱ�
	void reverse(AGraph *);//��ͼ��ת��
	void DFS();
	void stronglyConnectedCompenents(vector<SCCvertex>&);//��ǿ��ͨ����
	void branchGraph(vector<SCCvertex>&,AGraph&);//���֧ͼ
	void simplifyGraph(AGraph&);//��ͼ
	void print();
	~AGraph();
};


void AGraph::initGraph()
{
	size_t start, end;
	ifstream infile("F:\\scc.txt");
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
	edgeNode *curr = search(start, end);//�����֮ǰ�����Ƿ��Ѵ���
	if (curr == nullptr)
	{
		edgeNode *p = new edgeNode(end, weight);
		p->nextEdge = E[start];
		E[start] = p;
	}
}

void AGraph::reverse(AGraph *regraph)
{
	for (size_t i = 1; i != E.size(); ++i)
	{
		edgeNode *curr = E[i];
		while (curr != nullptr)
		{
			regraph->addEdge(curr->adjvertex, i);
			curr = curr->nextEdge;
		}
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
	//printVertex();
}
void AGraph::SCC_DFS(vector<SCCvertex> &branch)
{
	size_t branchnum = 0;
	for (size_t i = 1; i != E.size(); ++i)
		if (V[i].c == WHITE)
		{//��ǿ��ͨ��֧����
			SCCvertex tmp_scc;
			tmp_scc.sccID = ++branchnum;//����ǿ��ͨ��֧���
			tmp_scc.sccSet.push_back(V[i].id);//��ǿ��ͨ��֧�����Ķ���
			SCC_DFS_aux(V[i].id,tmp_scc);//��������
			branch.push_back(tmp_scc);
		}
}

void AGraph::SCC_DFS_aux(size_t v,SCCvertex &tmp_scc)
{
	size_t index = vertexIndex(v);//ȡ�Ľڵ�v��V�е�����������ȷ��E�л�������Ϣ����ͬ
	V[index].c = GRAY;
	edgeNode *curr = E[v];
	while (curr != nullptr)
	{
		size_t i = vertexIndex(curr->adjvertex);
		if (V[i].c == WHITE)
		{//��Ϊ��ɫ�ڵ㣬��Ϊ��ǿ��ͨ��֧һԱ
			tmp_scc.sccSet.push_back(V[i].id);
			SCC_DFS_aux(V[i].id,tmp_scc);
		}
		curr = curr->nextEdge;
	}
	V[index].c = BLACK;
}

void AGraph::stronglyConnectedCompenents(vector<SCCvertex> &branch)
{//branch�洢ǿ��ͨ��֧��Ϣ��������ǿ��ͨ��֧id�Լ������Ľڵ�id�����ü�������ʱ�临�Ӷ�ΪO(V+E)
	DFS();//�ȶ�ͼ����һ��DFS�����ÿ���ڵ�������ʱ��,ʱ��O(V+E)
	AGraph reG(nodenum);
	reverse(&reG);//��ͼ��ת�ã�����reG.E,ʱ��O(V+E)
	for (size_t i = 1; i != reG.V.size(); ++i)
	{//���ͼ������Ϣ������reG.V��ͬʱ���Ķ�����ɫ
		reG.V[i] = V[i];
		reG.V[i].c = WHITE;
	}
	struct compare
	{//�ֲ����������࣬����sort�ıȽϹ���
		bool operator()(const vertex &lhs, const vertex &rhs)const
		{
			return lhs.f > rhs.f;
		}
	};
	sort(++reG.V.begin(), reG.V.end(), compare());//�����ʽ���ʱ�併������ʱ��O(VlgV)���ü�������ɴﵽO(V)
	reG.SCC_DFS(branch);//��ǿ��ͨ��֧���ɵݹ��DFS�޸Ķ���,ʱ��O(V+E)
}

void AGraph::branchGraph(vector<SCCvertex> &branch,AGraph &branchG)
{//���ͼ�ķ�֧ͼ���洢��branchG��;branch�洢ǿ��ͨ��֧��Ϣ��ʱ��O(V+E)
	stronglyConnectedCompenents(branch);//��ȡͼ��ǿ��ͨ��֧��Ϣ,ʱ��O(V+E)���ü�������
	branchG.editGraph(branch.size());//���ݷ�֧������ͼ
	vector<size_t> vertex_branch_id(nodenum + 1);//�����SCC��ţ�������Ϊ������
	for (size_t i = 0; i != branch.size(); ++i)//��ÿ������ķ�֧id�����������ں���������ʱ��O(V)
		for (size_t j = 0; j != branch[i].sccSet.size(); ++j)
			vertex_branch_id[branch[i].sccSet[j]] = branch[i].sccID;
	for (size_t i = 1; i != E.size(); ++i)
	{//����ͼ�ı߱�E��ʱ��O(V+E)
		edgeNode *curr = E[i];
		while (curr != nullptr)
		{
			//(1)�������������(u��v)�ķ�֧id(x��y)��ͬ�������÷�֧��(x,y)�����õ��ķ�֧ͼ���Ѿ�����
			//��(y,x)����Ϊ������ڴ˱ߣ���˵���ڷ�֧y���ж����ܹ������֧x���������дӷ�֧x����֧y
			//�ı�(u��v)����ô��������֧���ܹ�����ģ�Ӧ����ͬһ����֧��ì�ܡ�
			//(2)����֧ͼ���Ѿ����ڱ�(x,y)���򲻻���ӣ���ΪaddEdge���Ȳ����Ƿ��д˱�
			if (vertex_branch_id[i] != vertex_branch_id[curr->adjvertex])
				branchG.addEdge(vertex_branch_id[i], vertex_branch_id[curr->adjvertex]);
			curr = curr->nextEdge;
		}
	}
}

void AGraph::simplifyGraph(AGraph &simpleG)
{//��ͼ���洢��simpleG�У�ʹ��ǿ��ͨ��֧�ͷ�֧ͼ��ԭͼһ�����ұ߾���С��
 //˼·���£�
	vector<SCCvertex> branch;
	AGraph branchG;
	branchGraph(branch, branchG);//1�����÷�֧ͼ�������ԭͼ��ǿ��ͨ��֧��Ϣ�ͷ�֧ͼ��Ϣ
	for (size_t i = 0; i != branch.size(); ++i)
	{//2����ͬһ����֧��Ķ���ѭ������ߣ���ĳ��֧���ж���v1,v2,v3...vk��������(v1,v2),(v2,v3)...(vk,v1)
		//����ֻ֧��һ�����㣬��������
		size_t i_branch_size = branch[i].sccSet.size();
		for (size_t j = 0; i_branch_size > 1 && j != i_branch_size; ++j)
			simpleG.addEdge(branch[i].sccSet[j % i_branch_size], branch[i].sccSet[(j + 1) % i_branch_size]);
	}
	for (size_t i = 1; i != branchG.E.size(); ++i)
	{//3���ٸ��ݷ�֧ͼ�и���֧֮��Ĺ�ϵ����ߣ����֧���Ϊx��y������֧���ڱ�(x,y)������ȡx��y��һ������
		//����Ϊu,v�������(u,v)
		edgeNode *curr = branchG.E[i];
		while (curr != nullptr)
		{//branch�Ǵ�0��ʼ�洢��֧�ģ�����֧��Ŵ�1��ʼ�����ҷ�֧j�洢������j - 1�У�
		 //�����֧���i��1��Ϊ�÷�֧������������һ��С��Ϸ����õ�����Ӧ���ǲ��ҳ��÷�֧������
			simpleG.addEdge(branch[i - 1].sccSet[0], branch[curr->adjvertex - 1].sccSet[0]);
			curr = curr->nextEdge;
		}
	}
}

void AGraph::DFS_aux_Not_recursive(size_t u, size_t &time)
{//�ǵݹ��DFS
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

const int nodenum = 8;

int main()
{
	AGraph graph(nodenum), /*branchG*/simpleG(nodenum);
	//vector<SCCvertex> v;
	graph.initGraph();
	graph.print();
	cout << endl;
	/*graph.branchGraph(v,branchG);
	for (size_t i = 0; i != v.size(); ++i)
		v[i].print();
	cout << endl;
	branchG.print();*/
	graph.simplifyGraph(simpleG);
	simpleG.print();
	cout << endl;
	getchar();
	return 0;
}