
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<stack>

using namespace std;
template <typename T> class BSTree;

template <typename T>
class node
{
private:
	friend class BSTree<T>;
	T data;
	node *left;//左孩子
	node *right;//右孩子
	node *parent;//父节点
public:
	node(const T &d):data(d),left(NULL),right(NULL),parent(NULL){}
	T getData()const {return data;}
	void setData(const T &d) {data = d;}
	/*省略指针域setter和getter*/
};

template <typename T>
class BSTree
{
private:
	node<T> *root;
	BSTree& operator=(const BSTree&);
	BSTree(const BSTree&);
	template<typename input_iter>
	void rebuildBST(input_iter,input_iter,size_t,node<T>*&);
public:
	BSTree(node<T> *r):root(r){}//复制构造函数，接受一个节点指针形参
	BSTree():root(NULL){}
	bool empty()const {return root == NULL;}
	void insert(const T &);
	void create();
	node<T>* locate(const T&)const;
	void preTraversal()const;
	void preTraversalNotRecursive()const;
	void inTraversal()const;
	void inTraversalNotRecursive()const;
	void postTraversal()const;
	void postTraversalNotRecursive()const;
	void erase(const T &);
	node<T>* minMum()const;
	node<T>* maxMum()const;
	node<T>* successor(const T &)const;//找后继
	node<T>* predecessor(const T &)const;//找前驱
	void destroy();
	

	template<typename input_iter>
	void rebuildBST(input_iter preorder,input_iter inorder,size_t tree_size)//根据前序和中序遍历结果重建二叉树
	{
		rebuildBST(preorder,inorder,tree_size,root);
	}
	bool traversalAtLevel(size_t)const;//层次遍历某一层的节点，从左至右
	void levelTraversal_h()const;//层次遍历解法1
	void levelTraversal_b()const;//层次遍历解法2
	void levelTraversal_q()const;//层次遍历解法3
	size_t height()const;//树高度
};

template <typename T>
void BSTree<T>::insert(const T &d)
{//插入，非递归
	node<T> *p = NULL,*curr = root;
	while(curr != NULL)
	{//找到插入位置
		p = curr;
		if(d <= curr->data)
			curr = curr->left;
		else curr = curr->right;
	}
	curr = new node<T>(d);
	if(p == NULL)//若树为空
		root = curr;
	else if(d <= p->data) p->left = curr;
		else p->right = curr;
	curr->parent = p;
	curr->left = curr->right = NULL;
}


/*template <typename T>
void BSTree<T>::insert(const T &d)
{//插入数据递归版本，有瑕疵，除了第一个节点，无法连接到以后创建的节点，即子树根无法连接到root的孩子域
	node<T> *p = NULL;
	if(root == NULL)
	{
		root = new node<T>(d);
		root->left = root->right = NULL;
		root->parent = p;
	}
	else if(d <= root->data)
	{
		BSTree LEFT(root->left);
		LEFT.insert(d);
	}
	else
	{
		BSTree RIGHT(root->left);
		RIGHT.insert(d);
	}
}
*/

template <typename T>
void BSTree<T>::create()
{
	T data;
	cout << "Enter the value(s),CTRL+Z to end" << endl;
	while(cin >> data)
		insert(data);
	cin.clear();
}

template <typename T>
void BSTree<T>::preTraversal()const
{
	node<T> *curr = root;
	if(curr != NULL)
	{
		cout << curr->data << ' ';
		BSTree LEFT(curr->left);//用左子树构建一个BSTree对象，继续递归
		LEFT.preTraversal();
		BSTree RIGHT(curr->right);
		RIGHT.preTraversal();
	}
}

template <typename T>
void BSTree<T>::preTraversalNotRecursive()const
{
	if (!root) return;
	stack<node<T>*> S;
	node<T> *curr = nullptr;
	S.push(root);
	while (!S.empty())
	{
		curr = S.top();
		S.pop();
		cout << curr->data << ' ';
		if (curr->right)
			S.push(curr->right);
		if (curr->left)
			S.push(curr->left);
	}
	cout << endl;
}

template <typename T>
void BSTree<T>::inTraversal()const
{
	node<T> *curr = root;
	if(curr != NULL)
	{
		BSTree LEFT(curr->left);
		LEFT.inTraversal();
		cout << curr->data << ' ';
		BSTree RIGHT(curr->right);//用右子树构建一个BSTree对象，继续递归
		RIGHT.inTraversal();
	}
}

template <typename T>
void BSTree<T>::inTraversalNotRecursive()const
{
	if (!root) return;
	node<T> *curr = root;
	stack<node<T>*> S;
	while (curr)
	{
		S.push(curr);
		curr = curr->left;
	}
	while (!S.empty())
	{
		curr = S.top();
		cout << curr->data << ' ';
		S.pop();
		if (curr->right)
		{
			curr = curr->right;
			while (curr)
			{
				S.push(curr);
				curr = curr->left;
			}
		}
	}
}

template <typename T>
void BSTree<T>::postTraversal()const
{
	node<T> *curr = root;
	if (curr)
	{
		BSTree<T> LEFT(curr->left);
		LEFT.postTraversal();
		BSTree<T> RIGHT(curr->right);
		RIGHT.postTraversal();
		cout << curr->data << ' ';
	}
}

template <typename T>
void BSTree<T>::postTraversalNotRecursive()const
{
	if (!root) return;
	node<T> *curr = nullptr, *prev = nullptr;
	stack<node<T>*> S;
	S.push(root);
	while (!S.empty())
	{
		curr = S.top();
		if ((!curr->right && !curr->left) || (curr->left && curr->left == prev)
			|| (curr->right && curr->right == prev))
		{
			cout << curr->data << ' ';
			S.pop();
			prev = curr;
		}
		else
		{
			if (curr->right)
				S.push(curr->right);
			if (curr->left)
				S.push(curr->left);
		}
	}
}

/*template <typename T>
node<T>* BSTree<T>::locate(const T &d)const
{//查找，非递归
	node<T> *curr = root;
	while(curr != NULL && curr->data != d)
	{
		if(curr->data > d)
			curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}*/

template <typename T>
node<T>* BSTree<T>::locate(const T &d)const
{//查找递归版本
	node<T> *curr = root;
	if(curr == NULL || curr->data == d)
		return curr;
	else if(curr->data > d)
	{
		BSTree LEFT(curr->left);
		return LEFT.locate(d);
	}
	else
	{
		BSTree RIGHT(curr->right);
		return RIGHT.locate(d);
	}
}


/*template <typename T>
node<T>* BSTree<T>::minMum()const
{//求最小值，非递归
	if(root == NULL) return root;
	node<T> *curr = root;
	while(curr->left != NULL)
		curr = curr->left;
	return curr;
}
*/
template <typename T>
node<T>* BSTree<T>::minMum()const
{//求最小值递归版本
	if(root == NULL) return root;
	node<T> *curr = root;
	if(curr->left == NULL) return curr;
	BSTree LEFT(curr->left);
	return LEFT.minMum();
}


/*template <typename T>
node<T>* BSTree<T>::maxMum()const
{//求最大值，非递归
	if(root == NULL) return root;
	node<T> *curr = root;
	while(curr->right != NULL)
		curr = curr->right;
	return curr;
}
*/
template <typename T>
node<T>* BSTree<T>::maxMum()const
{//求最大值递归版本
	if(root == NULL) return root;
	node<T> *curr = root;
	if(curr->right == NULL) return curr;
	BSTree RIGHT(curr->right);
	return RIGHT.maxMum();
}


template <typename T>
node<T>* BSTree<T>::successor(const T &d)const
{//找后继
	node<T> *p = locate(d);
	if(p->right != NULL)
	{//若右子树不为空，则后继为右子树的最小值
		BSTree RIGHT(p->right);
		return RIGHT.minMum();
	}
	node<T> *par = p->parent;
	while(par != NULL && par->right == p)
	{//若为空，则后继在沿右指针反向而上第一个拐点处
		p = par;
		par = p->parent;
	}
	return par;
}

template <typename T>
node<T>* BSTree<T>::predecessor(const T &d)const
{
	node<T> *p = locate(d);
	if(p->left != NULL)
	{//若左子树部位空，则前驱为左子树最大值
		BSTree LEFT(p->left);
		return LEFT.maxMum();
	}
	node<T> *par = p->parent;
	while(par != NULL && par->left == p)
	{//若为空，则前驱在沿左指针反向而上第一个拐点处
		p = par;
		par = p->parent;
	}
	return par;
}

template <typename T>
void BSTree<T>::erase(const T &d)
{//删除一个节点
	node<T> *p = locate(d),*next,*child;
	if(p->left == NULL || p->right == NULL)
		next = p;//确定删除节点，若其最多有一个子女
	else next = successor(d);//这里也可以改为删除前驱，predecessor，其他不用变。
	if(next->left != NULL)
		child = next->left;//取后继节点的子女，用以代替后继的位置，后继节点最多有一个子女
	else child = next->right;
	if(child != NULL)//后继节点有子女
		child->parent = next->parent;
	if(next->parent == NULL)//若要删除的节点为根节点
		root = child;
	else if(next == next->parent->left)//若后继为其父节点的左孩子
		next->parent->left = child;
	else next->parent->right = child;//若为右孩子
	if(next != p)
		p->data = next->data;
	delete next;//释放后继节点
}

template <typename T>
void BSTree<T>::destroy()
{//销毁二叉树
	if(root == NULL) return;
	if(root->left != NULL)
	{
		BSTree<T> LEFT(root->left);
		LEFT.destroy();
	}
	if(root->right != NULL)
	{
		BSTree<T> RIGHT(root->right);
		RIGHT.destroy();
	}
	delete root;
}

//-----------------------------------二次添加---------------------------------------------------
template <typename T> template<typename input_iter>
void BSTree<T>::rebuildBST(input_iter preorder,input_iter inorder,size_t tree_size,node<T> *&root)
{//重建二叉树
	node<T> *p = new node<T>(*preorder);//子树根
	if(root == NULL) root = p;//若树为空，则p为树根
	if(tree_size == 1) return ;//当子树规模为1时，结束
	input_iter right_in_start = inorder,left_pre_start = preorder + 1;
	size_t left_tree_size = 0;
	while(*right_in_start++ != *preorder)//计算右子树中序起始位置
		++left_tree_size;//计算左子树规模
	input_iter right_pre_start = left_pre_start + left_tree_size;//右子树前序起始位置
	size_t right_tree_size = tree_size - left_tree_size - 1;//右子树规模
	if(left_tree_size > 0) rebuildBST(left_pre_start,inorder,left_tree_size,p->left);//递归重建左子树
	if(right_tree_size > 0) rebuildBST(right_pre_start,right_in_start,right_tree_size,p->right);//递归重建右子树
	if(p->left != NULL) p->left->parent = p;//设置各自父指针
	if(p->right != NULL) p->right->parent = p;
}

template <typename T>
bool BSTree<T>::traversalAtLevel(size_t level)const
{//从左到右输出level层的数据
	if(root == NULL) return false;
	if(level == 0)
	{//如果已经递归到当前层，输出
		cout << root->data << ' ';
		return true;
	}
	BSTree<T> LEFT(root->left),RIGHT(root->right);//否则继续向下递归
	bool left_status = LEFT.traversalAtLevel(level - 1),
		right_status = RIGHT.traversalAtLevel(level - 1);
	return left_status || right_status;//返回递归执行的状态
}

template <typename T>
size_t BSTree<T>::height()const
{//求树高
	if (root == NULL) return 0;
	size_t left_height = 0, right_height = 0;
	if (root->left != NULL)
	{
		BSTree<T> LEFT(root->left);
		left_height = LEFT.height();
	}
	if (root->right != NULL)
	{
		BSTree<T> RIGHT(root->right);
		right_height = RIGHT.height();
	}
	return left_height > right_height ? left_height + 1 : right_height + 1;
}

template <typename T>
void BSTree<T>::levelTraversal_h()const
{//二叉树的层次遍历，解法1
	size_t h = height();//先求树高，即层数
	for (size_t i = 0; i <= h; ++i)
	{
		traversalAtLevel(i);//然后在输出每一层的数据
		cout << endl;
	}
}

template <typename T>
void BSTree<T>::levelTraversal_b()const
{//层次遍历解法2，不要求先明确层数
	for (size_t level = 0;; ++level)
	{
		if (!traversalAtLevel(level)) break;//根据当前层的返回结果判断，是否已经遍历完
		cout << endl;
	}
	return;
}

#include<vector>

template<typename T>
void BSTree<T>::levelTraversal_q()const
{//层次遍历解法3
	if (root == NULL) return;
	cout << root->data << endl;
	vector<node<T>*> pvec;//利用一个数组（或者队列）存储已访问过的节点
	pvec.push_back(root);
	size_t curr = 0, last = 1;
	while (curr != pvec.size())
	{//只要数组中还存在元素
		size_t num = 0;//记录新添加的元素数
		for (; curr != last;++curr)
		{//遍历当前层的元素
			if (pvec[curr]->left != NULL)
			{//左孩子
				cout << pvec[curr]->left->data << ' ';
				pvec.push_back(pvec[curr]->left);
				++num;
			}
			if (pvec[curr]->right != NULL)
			{//右孩子
				cout << pvec[curr]->right->data << ' ';
				pvec.push_back(pvec[curr]->right);
				++num;
			}
		}
		cout << endl;
		last += num;
	}
}

int main()
{//12 5 18 2 9 15 19 17
	BSTree<int> btree;
	cout << "btree.create()" << endl;
	btree.create();
	cout << "btree.inTraversal()" << endl;
	btree.inTraversal();
	/*cout <<  endl << btree.locate(19)->getData();*/
	cout << endl << "btree.preTraversal()" << endl;
	btree.postTraversal();
	cout << endl;
	btree.postTraversalNotRecursive();
	cout << endl;
	btree.levelTraversal_q();
	btree.destroy();
	getchar();
	return 0;
}
//#include<string>
//
//int main()
//{
//	string pre = "abcde",in = "edcba";
//	BSTree<char> bt;
//	bt.rebuildBST(pre.begin(),in.begin(),pre.size());
//	bt.preTraversal();
//	cout << endl;
//	bt.inTraversal();
//	bt.destroy();
//	getchar();
//	return 0;
//}