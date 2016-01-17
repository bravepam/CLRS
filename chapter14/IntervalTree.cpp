
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

/****区间树Interval Tree——红黑树扩充而得
*增加了key_low，high和max数据成员
*node增加了getInterval函数，ITree增加了search系列函数
*修改了其他函数，以支持对上述三个域的维护
*/

#include<iostream>

using namespace std;
#define MAX(i,j,k) (i > j ? (i > k ? i : k):(j > k ? j : k));//求三个树最大值
enum COLOR { red, black };//枚举，定义颜色

class node
{
private:
	friend class ITree;
	node *parent;
	node *left;
	node *right;
	int key_low;//既是区间左端点，也是节点的关键字
	int high;//区间右端点
	int max;//以当前节点为根的子树中右端点最大值
	COLOR color;
	node(){}//默认构造函数，只供创建nil时调用
public:
	node(int kl, int hi, COLOR c = red) :key_low(kl), high(hi), color(c),
		max(hi), parent(NULL), left(NULL), right(NULL){}
	void getInterval()//打印区间信息
	{
		cout << '[' << key_low << ',' << high << "]\t" << max;
		if (color == red) cout << "\tred" << endl;
		else cout << "\tblack" << endl;
	}
	//省略指针域的getter和setter
};

class ITree
{
private:
	static node *nil;//哨兵，静态成员，被整个ITree类所共有
	node *root;
	ITree(const ITree&);//只声明不定义，以禁止复制构造
	ITree operator=(const ITree&);//禁止赋值
	void leftRotate(node*);//左旋
	void rightRotate(node*);//右旋
	void insertFixup(node*);//插入节点后区间性质调整
	void eraseFixup(node*);//删除节点后区间性质调整
public:
	ITree() :root(nil)
	{//设置nil的各个域
		root->parent = nil;
		root->left = nil;
		root->right = nil;
		root->color = black;
		nil->key_low = nil->high = nil->max = -1;
	}
	ITree(node *rbt) :root(rbt){}//复制构造函数，用于创建子区间树对象
	void insert(int, int);//插入
	void create();//创建区间树
	void erase(int);//删除
	node* locate(int)const;//查找
	node* search(int low, int high)const;//在区间树中查找与[low,high]重叠的区间
	node* searchMinLeftPoint(int low, int high)const;//在区间树中查找与[low,high]重叠的左端点最小的区间
	void searchAll(int low, int high)const;//在区间树中找出所有与[low,high]重叠的区间
	node* searchExactly(int low, int high)const;//在区间树中找出与[low,high]完全匹配的区间
	node* minMumInterval()const;//找到区间左端点最小值区间
	node* maxMumInterval()const;//区间左端点最大值区间
	//node* longestInterval()const;//查找长度最长的区间
	node* successor(int)const;//找后继区间
	node* predecessor(int)const;//找前驱区间
	void inTraversal()const;//中根遍历
	void destroy();//销毁区间树
	bool empty()const{ return root == nil; }//判空
};

node *ITree::nil = new node;//定义静态成员nil

void ITree::leftRotate(node *curr)
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
		rchild->max = curr->max;
		curr->max = MAX(curr->max, curr->left->max, curr->right->max);
	}
}

void ITree::rightRotate(node *curr)
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
		lchild->max = curr->max;
		curr->max = MAX(curr->max, curr->left->max, curr->right->max);
	}
}

void ITree::insert(int low, int high)
{
	node *pkey = new node(low, high),
		*p = nil, *curr = root;
	while (curr != nil)
	{//找插入位置
		p = curr;//记住当前节点父亲
		if (low < curr->key_low)//往左找
			curr = curr->left;
		else curr = curr->right;//向右找
	}
	pkey->parent = p;
	if (p == nil)//插入的是第一个节点
		root = pkey;
	else if (low < p->key_low)
		p->left = pkey;
	else p->right = pkey;
	pkey->left = pkey->right = nil;
	while (p != nil)
	{
		p->max = MAX(p->max, p->left->max, p->right->max);
		p = p->parent;
	}
	insertFixup(pkey);//调整
}

void ITree::insertFixup(node *curr)
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

void ITree::create()
{
	int low, high;
	cout << "Enter interval(s),CTRL+Z to end" << endl;//换行后CTRL+Z结束输入
	while (cin >> low >> high)
		insert(low, high);
	cin.clear();
}

void ITree::inTraversal()const
{
	node *curr = root;
	if (curr != nil)
	{
		ITree LEFT(curr->left);
		LEFT.inTraversal();
		curr->getInterval();//打印区间信息
		ITree RIGHT(curr->right);//继续右子树中根遍历
		RIGHT.inTraversal();
	}
}

node* ITree::successor(int kl)const
{
	node *curr = locate(kl);
	if (curr->right != nil)
	{//若右子树不为空，则后继为右子树最小值
		ITree RIGHT(curr->right);
		return RIGHT.minMumInterval();
	}
	node *p = curr->parent;
	while (p != nil && curr == p->right)
	{//否则为沿右指针一直向上直到第一个拐弯处节点
		curr = p;
		p = p->parent;
	}
	return p;
}

node* ITree::minMumInterval()const
{
	node *curr = root;
	while (curr->left != nil)
		curr = curr->left;
	return curr;
}

node* ITree::maxMumInterval()const
{
	node *curr = root;
	while (curr->right != nil)
		curr = curr->right;
	return curr;
}

node* ITree::predecessor(int kl)const
{
	node *curr = locate(kl);
	if (curr->left != nil)
	{//若左子树不为空，则前驱为左子树最大值
		ITree LEFT(curr->left);
		return LEFT.maxMumInterval();
	}
	node *p = curr->parent;
	while (p != nil && curr == p->left)
	{//否则为沿左指针一直往上的第一个拐弯处节点
		curr = p;
		p = p->parent;
	}
	return p;
}

void ITree::erase(int kl)
{
	node *curr = locate(kl), *pdel, *child;
	if (curr == nil) return;
	if (curr->left == nil || curr->right == nil)//决定删除节点
		pdel = curr;//若当前节点至多有一个孩子，则删除它
	else pdel = successor(kl);//否则若有两孩子，则删除其后继
	if (pdel->left != nil)//记下不为空的孩子
		child = pdel->left;
	else child = pdel->right;
	child->parent = pdel->parent;
	if (pdel->parent == nil)//若删除的是根节点
		root = child;
	else if (pdel == pdel->parent->left)//否则若被删节点是其父亲左孩子
		pdel->parent->left = child;
	else pdel->parent->right = child;
	if (curr != pdel)
	{
		curr->key_low = pdel->key_low;//若被删的是后继，则将后继值赋给当前节点
		curr->high = pdel->high;
	}
	curr = child->parent;
	while (curr != nil)
	{
		curr->max = MAX(curr->max, curr->left->max, curr->right->max);
		curr = curr->parent;
	}
	if (pdel->color == black)//被删节点为黑色时才调整
		eraseFixup(child);
	delete pdel;//释放所占内存
}

void ITree::eraseFixup(node *curr)
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

node* ITree::locate(int kl)const
{
	node *curr = root;
	while (curr != nil && curr->key_low != kl)
	{
		if (kl < curr->key_low)curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}

node* ITree::search(int low, int high)const
{
	node *curr = root;
	while (curr != nil && (low > curr->high || curr->key_low > high))
		if (curr->left->max >= low) curr = curr->left;
		else curr = curr->right;
		return curr;
}

node* ITree::searchMinLeftPoint(int low, int high)const
{
	node *curr = root, *pMin = nil;
	int minPoint = 0x7fffffff;
	while (curr != nil)
	{
		if ((curr->key_low <= high && low <= curr->high) && curr->key_low < minPoint)
		{
			pMin = curr;
			minPoint = curr->key_low;
		}
		if (curr->left != nil && curr->left->max >= low)
			curr = curr->left;
		else curr = curr->right;
	}
	return pMin;
}

void ITree::searchAll(int low, int high)const
{//递归，左右两子树均可能存在满足条件的
	node *curr = root;
	if (curr->key_low <= high && low <= curr->high)
		curr->getInterval();
	if (curr->left != nil && curr->left->max >= low)
	{
		ITree LEFT(curr->left);
		LEFT.searchAll(low, high);
	}
	if (curr->right != nil)
	{
		ITree RIGHT(curr->right);
		RIGHT.searchAll(low, high);
	}
}

node* ITree::searchExactly(int low, int high)const
{
	node *curr = root;
	while (curr != nil)
	{
		if (curr->key_low == low && curr->high == high)
			return curr;
		if (curr->key_low >= low)//注意这里和其他search的一点区别
			curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}

void ITree::destroy()
{
	while (root != nil)
	{
		//cout << "erase: " << root->key_low << endl;
		erase(root->key_low);
	}
	delete nil;
}

int main()
{//16 21   8 9   25 30   5 8   15 23   17 19   26 26   0 3   6 10   19 20
	ITree itree;
	cout << "creare-------------" << endl;
	itree.create();
	cout << "inTraversal--------" << endl;
	itree.inTraversal();
	cout << endl;
	itree.searchExactly(26, 26)->getInterval();
	cout << "search-------------" << endl;
	itree.search(22,25)->getInterval();
	cout << endl;
	itree.minMumInterval()->getInterval();
	cout << endl;
	itree.maxMumInterval()->getInterval();
	itree.erase(16);
	itree.erase(26);
	itree.erase(16);
	cout << endl;
	itree.inTraversal();
	itree.insert(13,19);
	cout << endl;
	itree.searchAll(17, 19);
	itree.inTraversal();
	itree.destroy();
	getchar();
	return 0;
}