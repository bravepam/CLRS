
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

/*****求多个区间的最大重叠点，姑且称为最大重叠点树，红黑树扩张
*增加了p,p_sum,max,pom等数据成员
*增加了update和keep函数
*微调insert，左右旋以及erase函数
*/
#include<iostream>
//#include<cstdlib>

using namespace std;
enum COLOR { red, black };//枚举，定义颜色
#define Max(i,j,k) (i > j ? (i > k ? i : k) : (j > k) ? j : k);
const int MAX = 0x7fffffff;

class node
{
private:
	friend class POMTree;
	node *parent;
	node *left;
	node *right;
	int key;
	int p;//左端点为1，右端点为-1
	int p_sum;//以当前节点为根的子树中p的和
	int max;//以当前节点为根的子树中p的和的最大值
	int pom;//使其取到最大值的关键字,即此子树的最大重叠点
	COLOR color;
	node(){}//默认构造函数，只供创建nil时调用
public:
	node(int k, COLOR c = red) :key(k), p_sum(0), max(0), pom(k), color(c),
		parent(NULL), left(NULL), right(NULL){}
	void print()const
	{
		printf("key:%-10d POM:%-10d max:%-10d p_sum:%-10d p:%-10d", key, pom, max, p_sum, p);
		if (color == red) printf("red\n");
		else printf("black\n");
	}
	//省略指针域的getter和setter
};


class POMTree
{
private:
	static node *nil;//哨兵，静态成员，被整个POMTree类所共有
	node *root;
	POMTree(const POMTree&);//禁止复制构造
	POMTree operator=(const POMTree&);//禁止赋值
	void leftRotate(node*);//左旋
	void rightRotate(node*);//右旋
	void insertFixup(node*);//插入节点后红黑性质调整
	void eraseFixup(node*);//删除节点后红黑性质调整
	void update(node*);//更新节点信息
	void keep(node*);//自底向上更新路径信息
public:
	POMTree() :root(nil)
	{
		root->parent = nil;
		root->left = nil;
		root->right = nil;
		root->color = black;
		root->key = MAX; root->p = 0;
		root->p_sum = 0; root->max = 0;
		root->pom = MAX;
	}
	POMTree(node *rbt) :root(rbt){}//复制构造函数，用于创建子红黑树对象
	void insert(int, bool);//插入
	void create();//创建红黑树
	void erase(int);//删除
	node* locate(int)const;//查找
	node* minMum()const;//最小值
	node* maxMum()const;//最大值
	node* successor(int)const;//找后继
	node* predecessor(int)const;//前驱
	void preTraversal()const;//先根遍历
	void inTraversal()const;//中根遍历
	void destroy();//销毁红黑树
	void findPom()const { root->print(); }
	bool empty()const{ return root == nil; }//判空
};

node *POMTree::nil = new node;//定义静态成员nil

void POMTree::update(node *curr)
{
	curr->p_sum = curr->left->p_sum + curr->p + curr->right->p_sum;
	curr->max = Max(curr->left->max, curr->left->p_sum + curr->p,
		curr->left->p_sum + curr->p + curr->right->max);
	if (curr->left != nil && curr->max == curr->left->max) curr->pom = curr->left->pom;
	else if (curr->right != nil &&
		curr->max == curr->left->p_sum + curr->p + curr->right->max)curr->pom = curr->right->pom;
	else curr->pom = curr->key;
}

void POMTree::keep(node *curr)
{
	while (curr != nil)
	{
		update(curr);
		curr = curr->parent;
	}
}

void POMTree::leftRotate(node *curr)
{
	if (curr->right != nil)
	{//存在右孩子时才能左旋
		node *rchild = curr->right;
		curr->right = rchild->left;
		if (rchild->left != nil)
			rchild->left->parent = curr;
		rchild->parent = curr->parent;
		if (curr->parent == nil)
			root = rchild;
		else if (curr == curr->parent->left)
			curr->parent->left = rchild;
		else curr->parent->right = rchild;
		curr->parent = rchild;
		rchild->left = curr;
		rchild->p_sum = curr->p_sum;
		rchild->max = curr->max;
		rchild->pom = curr->pom;
		update(curr);
	}
}

void POMTree::rightRotate(node *curr)
{
	if (curr->left != nil)
	{//存在左孩子时才能右旋
		node *lchild = curr->left;
		curr->left = lchild->right;
		if (lchild->right != nil)
			lchild->right->parent = curr;
		lchild->parent = curr->parent;
		if (curr->parent == nil)
			root = lchild;
		else if (curr == curr->parent->left)
			curr->parent->left = lchild;
		else curr->parent->right = lchild;
		lchild->right = curr;
		curr->parent = lchild;
		lchild->p_sum = curr->p_sum;
		lchild->max = curr->max;
		lchild->pom = curr->pom;
		update(curr);
	}
}


void POMTree::insert(int k, bool start)
{
	node *pkey = new node(k),
		*p = nil, *curr = root;
	if (start)
	{
		pkey->p = 1;
		pkey->p_sum = 1;
		pkey->max = 1;
		pkey->pom = k;
	}
	else
	{
		pkey->p = -1;
		pkey->p_sum = -1;
		pkey->max = 0;
		pkey->pom = k;
	}
	while (curr != nil)
	{//找插入位置
		p = curr;//记住当前节点父亲
		if (k < curr->key)//往左找
			curr = curr->left;
		else curr = curr->right;//向右找
	}
	pkey->parent = p;
	if (p == nil)//插入的是第一个节点
		root = pkey;
	else if (k < p->key)
		p->left = pkey;
	else p->right = pkey;
	pkey->left = pkey->right = nil;
	keep(pkey->parent);
	insertFixup(pkey);//调整
}


void POMTree::insertFixup(node *curr)
{
	while (curr->parent->color == red)
	{//父亲为红节点时才需要进入循环调整
		if (curr->parent == curr->parent->parent->left)
		{//父亲是祖父左孩子
			node *uncle = curr->parent->parent->right;
			if (uncle != nil && uncle->color == red)
			{//情况1，叔叔节点存在且为红色
				curr->parent->color = black;
				uncle->color = black;
				curr->parent->parent->color = red;
				curr = curr->parent->parent;
			}
			else if (curr == curr->parent->right)
			{//情况2，叔叔节点为黑色，且当前节点是父亲右孩子
				curr = curr->parent;
				leftRotate(curr);//将父节点左旋，以转变为情况3
			}
			else
			{//情况3，叔叔节点为黑色，且当前节点是父亲左孩子
				curr->parent->color = black;
				curr->parent->parent->color = red;
				rightRotate(curr->parent->parent);
			}
		}
		else
		{//父亲是祖父右孩子，与上面三种情况对称
			node *uncle = curr->parent->parent->left;
			if (uncle != nil && uncle->color == red)
			{//情况1
				curr->parent->color = black;
				uncle->color = black;
				curr->parent->parent->color = red;
				curr = curr->parent->parent;
			}
			else if (curr == curr->parent->left)
			{//情况2
				curr = curr->parent;
				rightRotate(curr);
			}
			else
			{//情况3
				curr->parent->color = black;
				curr->parent->parent->color = red;
				leftRotate(curr->parent->parent);
			}
		}
	}
	root->color = black;//跳出循环时将根节点置为黑色
}


void POMTree::create()
{
	int low, high;
	cout << "Enter element(s),CTRL+Z to end" << endl;//换行后CTRL+Z结束输入
	while (cin >> low >> high)
	{
		insert(low, true);
		insert(high, false);
	}
	cin.clear();
}


void POMTree::preTraversal()const
{
	node *curr = root;
	if (curr != nil)
	{
		curr->print();
		POMTree LEFT(curr->left);//继续左子树先根遍历
		LEFT.preTraversal();
		POMTree RIGHT(curr->right);
		RIGHT.preTraversal();
	}
}


void POMTree::inTraversal()const
{
	node *curr = root;
	if (curr != nil)
	{
		POMTree LEFT(curr->left);
		LEFT.inTraversal();
		curr->print();
		POMTree RIGHT(curr->right);//继续右子树中根遍历
		RIGHT.inTraversal();
	}
}


node* POMTree::successor(int k)const
{
	node *curr = locate(k);
	if (curr->right != nil)
	{//若右子树不为空，则后继为右子树最小值
		POMTree RIGHT(curr->right);
		return RIGHT.minMum();
	}
	node *p = curr->parent;
	while (p != nil && curr == p->right)
	{//否则为沿右指针一直向上直到第一个拐弯处节点
		curr = p;
		p = p->parent;
	}
	return p;
}


node* POMTree::minMum()const
{
	node *curr = root;
	while (curr->left != nil)
		curr = curr->left;
	return curr;
}


node* POMTree::maxMum()const
{
	node *curr = root;
	while (curr->right != nil)
		curr = curr->right;
	return curr;
}


node* POMTree::predecessor(int k)const
{
	node *curr = locate(k);
	if (curr->left != nil)
	{//若左子树不为空，则前驱为左子树最大值
		POMTree LEFT(curr->left);
		return LEFT.maxMum();
	}
	node *p = curr->parent;
	while (p != nil && curr == p->left)
	{//否则为沿左指针一直往上的第一个拐弯处节点
		curr = p;
		p = p->parent;
	}
	return p;
}


void POMTree::erase(int k)
{
	node *curr = locate(k), *pdel, *child;
	if (curr == nil)
	{
		cout << "Error:no data" << endl;
		return;
	}
	if (curr->left == nil || curr->right == nil)//决定删除节点
		pdel = curr;//若当前节点至多有一个孩子，则删除它
	else pdel = successor(k);//否则若有两孩子，则删除其后继
	node *pdel_parent = pdel->parent;//记下被删节点父亲
	if (pdel->left != nil)//记下不为空的孩子
		child = pdel->left;
	else child = pdel->right;
	child->parent = pdel_parent;
	if (pdel_parent == nil)//若删除的是根节点
		root = child;
	else if (pdel == pdel_parent->left)//否则若被删节点是其父亲左孩子
		pdel_parent->left = child;
	else pdel_parent->right = child;
	if (curr != pdel)
		curr->key = pdel->key;//若被删的是后继，则将后继值赋给当前节点
	keep(pdel_parent);
	if (pdel->color == black)//被删节点为黑色时才调整
		eraseFixup(child);
	delete pdel;//释放所占内存
}


void POMTree::eraseFixup(node *curr)
{
	while (curr != root && curr->color == black)
	{//当前不为根，且为黑色
		if (curr == curr->parent->left)
		{//若其是父亲左孩子
			node *brother = curr->parent->right;//兄弟节点肯定存在
			if (brother->color == red)
			{//情况1，兄弟是红色，转变为情况2,3,4
				brother->color = black;
				curr->parent->color = red;
				leftRotate(curr->parent);
				brother = curr->parent->right;
			}
			if (brother->left->color == black && brother->right->color == black)
			{//情况2，兄弟是黑色，且两孩子也是黑色，将当前节点和兄弟去一重黑色
				brother->color = red;
				curr = curr->parent;
			}
			else if (brother->right->color == black)
			{//情况3，兄弟左孩子为红，右孩子为黑，转变为情况4
				brother->color = red;
				brother->left->color = black;
				rightRotate(brother);
				brother = curr->parent->right;
			}
			else
			{//情况4，右孩子为黑色，左孩子随意
				brother->color = curr->parent->color;
				curr->parent->color = black;
				brother->right->color = black;
				leftRotate(curr->parent);
				curr = root;
			}
		}
		else
		{//若其是父亲右孩子，与上面四中情况对称
			node *brother = curr->parent->left;
			if (brother->color == red)
			{//情况1
				brother->color = black;
				curr->parent->color = red;
				rightRotate(curr->parent);
				brother = curr->parent->left;
			}
			if (brother->right->color == black && brother->left->color == black)
			{//情况2
				brother->color = red;
				curr = curr->parent;
			}
			else if (brother->left->color == black)
			{//情况3
				brother->color = red;
				brother->right->color = black;
				leftRotate(brother);
				brother = curr->parent->left;
			}
			else
			{//情况4
				brother->color = curr->parent->color;
				curr->parent->color = black;
				brother->left->color = black;
				rightRotate(curr->parent);
				curr = root;
			}
		}
	}
	curr->color = black;//结束循环时将当前节点置为黑色
}


node* POMTree::locate(int k)const
{
	node *curr = root;
	while (curr != nil && curr->key != k)
	{
		if (k < curr->key)curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}


void POMTree::destroy()
{
	while (root != nil)
	{
		//cout << "erase: " << root->key << endl;
		erase(root->key);
	}
	delete nil;
}

int main()
{//16 21   8 9   25 30   5 8   15 23   19 20   17 19   26 26   0 3   6 10
	//16 21   8 9   25 30   5 8   15 23  17 19   26 26   0 3   6 10   19 20 
	//7 9   5 8   0 3   6 10  有时得不出正确答案，实在调不出来了，放下！
	POMTree ptree;
	ptree.create();
	cout << "inTraversal-----" << endl;
	ptree.inTraversal();
	cout << "preTraversal----" << endl;
	ptree.preTraversal();
	int low,high,choice;
	cout << "Enter your choice,1-insert,2-delete,3-POM,4-print,5-exit" << endl;
	do
	{
	cin >> choice;
	switch(choice)
	{
	case 1:
	cout << "Enter a interval,ex-> a b (a <= b): ";
	cin >> low >> high;
	ptree.insert(low,true);
	ptree.insert(high,false);
	break;
	case 2:
	cout << "Enter a interval,ex -> a b (a <= b): ";
	cin >> low >> high;
	ptree.erase(low);
	ptree.erase(high);
	break;
	case 3:
	ptree.findPom();
	break;
	case 4:
	cout << "inTraversal-----" << endl;
	ptree.inTraversal();
	cout << "preTraversal----" << endl;
	ptree.preTraversal();
	break;
	case 5:
	ptree.destroy();
	return 0;
	default:
	cout << "Bad choice" << endl;
	}
	//cout << "Enter your choice,1-insert,2-delete,3-POM,4-print,5-exit" << endl;
	}while(choice >= 1 && choice <= 5);
	ptree.destroy();
	return 0;
}
