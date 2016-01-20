
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
{//顶点
	size_t id, c;//编号，颜色
	size_t d, f;//访问开始和结束时间
	size_t p;//父节点编号
	vertex(size_t i = 0) :id(i), c(WHITE), p(NOPARENT), d(0), f(0){}
};

struct SCCvertex
{//强连通分支顶点
	size_t sccID;//强连通分支编号
	vector<size_t> sccSet;//该强连通分支包含的顶点
	void print()
	{
		cout << "SCC " << sccID << " includes vertex : ";
		for (size_t i = 0; i != sccSet.size(); ++i)
			cout << sccSet[i] << ' ';
		cout << endl;
	}
};

class AGraph
{//图
private:
	//边信息容器和顶点信息容器均从索引1开始存储信息，同时所有的图顶点也从1开始编号，因此容器中索引i对应地
	//就是顶点i及其边信息，顶点容器中顶点编号和索引的对应关系在需要移动顶点的操作下会错位，边容器一般不变
	vector<edgeNode*> E;
	vector<vertex> V;
	size_t nodenum;//顶点数
	void printEdge();
	void printVertex();
	void SCC_DFS(vector<SCCvertex>&);//求强连通分支
	void DFS_aux_Not_recursive(size_t, size_t &);//非递归的DFS辅助函数
	void SCC_DFS_aux(size_t,SCCvertex&);//求强连通分支辅助函数，修改递归的DFS而得
	size_t vertexIndex(size_t id)
	{//查找id号顶点的在顶点容器中的索引
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
	void editGraph(size_t n) { init(n + 1); }//修改图大小，并初始化
public:
	AGraph(size_t n = 0) :nodenum(n){ init(n + 1); }
	void initGraph();//初始化有向图
	edgeNode* search(size_t, size_t);//查找边
	void addEdge(size_t, size_t, int);//有向图中添加边
	void reverse(AGraph *);//求图的转置
	void DFS();
	void stronglyConnectedCompenents(vector<SCCvertex>&);//求强连通分量
	void branchGraph(vector<SCCvertex>&,AGraph&);//求分支图
	void simplifyGraph(AGraph&);//简化图
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
	edgeNode *curr = search(start, end);//插入边之前查找是否已存在
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
		{//新强连通分支出现
			SCCvertex tmp_scc;
			tmp_scc.sccID = ++branchnum;//分配强连通分支编号
			tmp_scc.sccSet.push_back(V[i].id);//该强连通分支包含的顶点
			SCC_DFS_aux(V[i].id,tmp_scc);//其他顶点
			branch.push_back(tmp_scc);
		}
}

void AGraph::SCC_DFS_aux(size_t v,SCCvertex &tmp_scc)
{
	size_t index = vertexIndex(v);//取的节点v在V中的索引，以正确从E中获得其边信息，下同
	V[index].c = GRAY;
	edgeNode *curr = E[v];
	while (curr != nullptr)
	{
		size_t i = vertexIndex(curr->adjvertex);
		if (V[i].c == WHITE)
		{//若为白色节点，则为该强连通分支一员
			tmp_scc.sccSet.push_back(V[i].id);
			SCC_DFS_aux(V[i].id,tmp_scc);
		}
		curr = curr->nextEdge;
	}
	V[index].c = BLACK;
}

void AGraph::stronglyConnectedCompenents(vector<SCCvertex> &branch)
{//branch存储强连通分支信息，包括各强连通分支id以及包含的节点id，若用计数排序时间复杂度为O(V+E)
	DFS();//先对图进行一次DFS，算得每个节点访问完成时间,时间O(V+E)
	AGraph reG(nodenum);
	reverse(&reG);//对图求转置，存入reG.E,时间O(V+E)
	for (size_t i = 1; i != reG.V.size(); ++i)
	{//获得图顶点信息，存入reG.V，同时更改顶点颜色
		reG.V[i] = V[i];
		reG.V[i].c = WHITE;
	}
	struct compare
	{//局部函数对象类，定义sort的比较规则
		bool operator()(const vertex &lhs, const vertex &rhs)const
		{
			return lhs.f > rhs.f;
		}
	};
	sort(++reG.V.begin(), reG.V.end(), compare());//按访问结束时间降序排序，时间O(VlgV)，用计数排序可达到O(V)
	reG.SCC_DFS(branch);//求强连通分支，由递归的DFS修改而得,时间O(V+E)
}

void AGraph::branchGraph(vector<SCCvertex> &branch,AGraph &branchG)
{//求该图的分支图，存储于branchG中;branch存储强连通分支信息，时间O(V+E)
	stronglyConnectedCompenents(branch);//获取图的强连通分支信息,时间O(V+E)，用计数排序。
	branchG.editGraph(branch.size());//根据分支数更改图
	vector<size_t> vertex_branch_id(nodenum + 1);//顶点的SCC编号，索引即为顶点编号
	for (size_t i = 0; i != branch.size(); ++i)//将每个顶点的分支id记下来，便于后续操作，时间O(V)
		for (size_t j = 0; j != branch[i].sccSet.size(); ++j)
			vertex_branch_id[branch[i].sccSet[j]] = branch[i].sccID;
	for (size_t i = 1; i != E.size(); ++i)
	{//遍历图的边表E，时间O(V+E)
		edgeNode *curr = E[i];
		while (curr != nullptr)
		{
			//(1)如果相连两顶点(u和v)的分支id(x和y)不同，则加入该分支边(x,y)，不用担心分支图中已经存在
			//边(y,x)，因为如果存在此边，则说明在分支y中有顶点能够到达分支x，现在又有从分支x到分支y
			//的边(u，v)，那么这两个分支是能够互达的，应该是同一个分支，矛盾。
			//(2)若分支图中已经存在边(x,y)，则不会添加，因为addEdge会先查找是否有此边
			if (vertex_branch_id[i] != vertex_branch_id[curr->adjvertex])
				branchG.addEdge(vertex_branch_id[i], vertex_branch_id[curr->adjvertex]);
			curr = curr->nextEdge;
		}
	}
}

void AGraph::simplifyGraph(AGraph &simpleG)
{//简化图，存储于simpleG中，使其强连通分支和分支图与原图一样，且边尽量小。
 //思路如下：
	vector<SCCvertex> branch;
	AGraph branchG;
	branchGraph(branch, branchG);//1、运用分支图函数求出原图的强连通分支信息和分支图信息
	for (size_t i = 0; i != branch.size(); ++i)
	{//2、对同一个分支里的顶点循环插入边，如某分支中有顶点v1,v2,v3...vk，则插入边(v1,v2),(v2,v3)...(vk,v1)
		//若分支只有一个顶点，则不作处理
		size_t i_branch_size = branch[i].sccSet.size();
		for (size_t j = 0; i_branch_size > 1 && j != i_branch_size; ++j)
			simpleG.addEdge(branch[i].sccSet[j % i_branch_size], branch[i].sccSet[(j + 1) % i_branch_size]);
	}
	for (size_t i = 1; i != branchG.E.size(); ++i)
	{//3、再根据分支图中各分支之间的关系插入边，如分支编号为x和y的两分支存在边(x,y)，则任取x和y中一个顶点
		//假设为u,v，插入边(u,v)
		edgeNode *curr = branchG.E[i];
		while (curr != nullptr)
		{//branch是从0开始存储分支的，而分支编号从1开始，并且分支j存储在索引j - 1中，
		 //因而分支编号i减1即为该分支索引。运用了一点小把戏，最好的做法应该是查找出该分支的索引
			simpleG.addEdge(branch[i - 1].sccSet[0], branch[curr->adjvertex - 1].sccSet[0]);
			curr = curr->nextEdge;
		}
	}
}

void AGraph::DFS_aux_Not_recursive(size_t u, size_t &time)
{//非递归的DFS
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