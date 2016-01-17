
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

/*******红黑树加强版********
#节点成员添加了黑高度bh数据成员
#相应的insertFixup和eraseFixup函数添加了对黑高度的调整
#添加了红黑树连接操作jion(Left/Right)成员函数以及配套的功能函数
*/
#include<iostream>
#include<iomanip>

using namespace std;
enum COLOR { red, black };//枚举，定义颜色

template <typename T> class RBTree;

template <typename T>
class node
{
private:
	friend class RBTree<T>;
	node *parent;
	node *left;
	node *right;
	T key;
	int bh;//黑高度
	COLOR color;
	node(){}//默认构造函数，只供创建nil时调用
public:
	node(const T &k, COLOR c = red) :key(k), color(c), bh(1),
		parent(NULL), left(NULL), right(NULL){}
	T& getKey(){ return key; }
	const T& getKey()const { return key; }
	//省略指针域的getter和setter
};

template <typename T>
class RBTree
{
private:
	static node<T> *nil;//哨兵，静态成员，被整个RBTree类所共有
	node<T> *root;
	RBTree(const RBTree&);//禁止复制构造
	RBTree operator=(const RBTree&);//禁止赋值
	void leftRotate(node<T>*);//左旋
	void rightRotate(node<T>*);//右旋
	void insertFixup(node<T>*);//插入节点后红黑性质调整
	void eraseFixup(node<T>*);//删除节点后红黑性质调整
public:
	RBTree() :root(nil)
	{
		root->parent = nil;
		root->left = nil;
		root->right = nil;
		root->color = black;
		root->bh = -1;//哨兵的黑高度设为-1.
	}
	RBTree(node<T> *rbt) :root(rbt){}//复制构造函数，用于创建子红黑树对象
	void insert(const T&);//插入
	void create();//创建红黑树
	void erase(const T&);//删除
	//将两棵树(T & rbt)和一个值k链接，链接在右边，其中key[T]<=k<=key[rbt]
	void joinRight(const T &k, RBTree *rbt);
	//将两棵树(T & rbt)和一个值k链接，链接在左边，其中key[T]>=k>=key[rbt]
	void joinLeft(const T &k, RBTree *rbt);
	void preTraversal()const;//先根遍历
	void inTraversal()const;//中根遍历
	void destroy();//销毁红黑树
	node<T>* locateMaxNodeOfBh(int bh)const;//在红黑树中查找某一黑高度的最大值节点
	node<T>* locateMinNodeOfBh(int bh)const;//在红黑树中查找某一黑高度的最小值节点
	node<T>* locate(const T&)const;//查找
	node<T>* minMum()const;//最小值
	node<T>* maxMum()const;//最大值
	node<T>* successor(const T&)const;//找后继
	node<T>* predecessor(const T&)const;//前驱
	bool empty()const{ return root == nil; }//判空
};

template <typename T> node<T> *RBTree<T>::nil = new node<T>;//定义静态成员nil

template <typename T>
void RBTree<T>::leftRotate(node<T> *curr)
{
	if (curr->right != nil)
	{//存在右孩子时才能左旋
		node<T> *rchild = curr->right;
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
	}
}

template <typename T>
void RBTree<T>::rightRotate(node<T> *curr)
{
	if (curr->left != nil)
	{//存在左孩子时才能右旋
		node<T> *lchild = curr->left;
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
	}
}

template <typename T>
void RBTree<T>::insert(const T &k)
{
	node<T> *pkey = new node<T>(k),
		*p = nil, *curr = root;
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
	insertFixup(pkey);//调整
}

template <typename T>
void RBTree<T>::insertFixup(node<T> *curr)
{
	while (curr->parent->color == red)
	{//父亲为红节点时才需要进入循环调整
		if (curr->parent == curr->parent->parent->left)
		{//父亲是祖父左孩子
			node<T> *uncle = curr->parent->parent->right;
			if (uncle != nil && uncle->color == red)
			{//情况1，叔叔节点存在且为红色
				curr->parent->color = black;
				uncle->color = black;
				curr->parent->parent->color = red;
				curr = curr->parent->parent;
				++curr->bh;
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
			node<T> *uncle = curr->parent->parent->left;
			if (uncle != nil && uncle->color == red)
			{//情况1
				curr->parent->color = black;
				uncle->color = black;
				curr->parent->parent->color = red;
				curr = curr->parent->parent;
				++curr->bh;
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

template <typename T>
void RBTree<T>::create()
{
	T k;
	cout << "Enter element(s),CTRL+Z to end" << endl;//换行后CTRL+Z结束输入
	while (cin >> k)
		insert(k);
	cin.clear();
}

template <typename T>
void RBTree<T>::preTraversal()const
{
	node<T> *curr = root;
	if (curr != nil)
	{
		cout << curr->key << " : ";
		if (curr->color == red) cout << setw(12) << "red";
		else cout << setw(12) << "black";
		cout << "black height: " << curr->bh << endl;
		RBTree LEFT(curr->left);//继续左子树先根遍历
		LEFT.preTraversal();
		RBTree RIGHT(curr->right);
		RIGHT.preTraversal();
	}
}

template <typename T>
void RBTree<T>::inTraversal()const
{
	node<T> *curr = root;
	if (curr != nil)
	{
		RBTree LEFT(curr->left);
		LEFT.inTraversal();
		cout << left << curr->key << " : ";
		if (curr->color == red) cout << left << setw(12) << "red";
		else cout << left << setw(12) << "black";
		cout << "black height: " << curr->bh << endl;
		RBTree RIGHT(curr->right);//继续右子树中根遍历
		RIGHT.inTraversal();
	}
}

template <typename T>
node<T>* RBTree<T>::successor(const T &k)const
{
	node<T> *curr = locate(k);
	if (curr->right != nil)
	{//若右子树不为空，则后继为右子树最小值
		RBTree RIGHT(curr->right);
		return RIGHT.minMum();
	}
	node<T> *p = curr->parent;
	while (p != nil && curr == p->right)
	{//否则为沿右指针一直向上直到第一个拐弯处节点
		curr = p;
		p = p->parent;
	}
	return p;
}

template <typename T>
node<T>* RBTree<T>::minMum()const
{
	node<T> *curr = root;
	while (curr->left != nil)
		curr = curr->left;
	return curr;
}

template <typename T>
node<T>* RBTree<T>::maxMum()const
{
	node<T> *curr = root;
	while (curr->right != nil)
		curr = curr->right;
	return curr;
}

template <typename T>
node<T>* RBTree<T>::predecessor(const T &k)const
{
	node<T> *curr = locate(k);
	if (curr->left != nil)
	{//若左子树不为空，则前驱为左子树最大值
		RBTree LEFT(curr->left);
		return LEFT.maxMum();
	}
	node<T> *p = curr->parent;
	while (p != nil && curr == p->left)
	{//否则为沿左指针一直往上的第一个拐弯处节点
		curr = p;
		p = p->parent;
	}
	return p;
}

template <typename T>
void RBTree<T>::erase(const T &k)
{
	node<T> *curr = locate(k), *pdel, *child;
	if (curr->left == nil || curr->right == nil)//决定删除节点
		pdel = curr;//若当前节点至多有一个孩子，则删除它
	else pdel = successor(k);//否则若有两孩子，则删除其后继
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
		curr->key = pdel->key;//若被删的是后继，则将后继值赋给当前节点
	if (pdel->color == black)//被删节点为黑色时才调整
		eraseFixup(child);
	delete pdel;//释放所占内存
}

template <typename T>
void RBTree<T>::eraseFixup(node<T> *curr)
{
	while (curr != root && curr->color == black)
	{//当前不为根，且为黑色
		if (curr == curr->parent->left)
		{//若其是父亲左孩子
			node<T> *brother = curr->parent->right;//兄弟节点肯定存在
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
				--curr->bh;
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
				++brother->bh;
				--brother->left->bh;
				curr = root;
			}
		}
		else
		{//若其是父亲右孩子，与上面四中情况对称
			node<T> *brother = curr->parent->left;
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
				--curr->bh;
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
				++brother->bh;
				--brother->left->bh;
				curr = root;
			}
		}
	}
	curr->color = black;//结束循环时将当前节点置为黑色
}

template <typename T>
node<T>* RBTree<T>::locate(const T &k)const
{
	node<T> *curr = root;
	while (curr != nil && curr->key != k)
	{
		if (k < curr->key)curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}

template <typename T>
node<T>* RBTree<T>::locateMaxNodeOfBh(int bh)const
{//查找红黑树上黑高度为bh的最大值节点
	node<T> *curr = root;
	if (curr == nil || curr->bh < bh) return nil;//树为空或者此树黑高度太低
	while (curr->bh != bh)
		curr = curr->right;
	return curr;
}

template <typename T>
node<T>* RBTree<T>::locateMinNodeOfBh(int bh)const
{//查找红黑树上黑高度为bh的最小值节点，与locateMaxNodeOfBh对称
	node<T> *curr = root;
	if (curr == nil || curr->bh < bh) return nil;
	while (curr->bh != bh)
		curr = curr->left;
	return curr;
}

template <typename T>
void RBTree<T>::joinRight(const T &k, RBTree *rbt)
{//将较矮的红黑树rbt以及节点k合并到较高红黑树T上,其中key[T]<=k<=key[rbt]
	node<T> *r = locateMaxNodeOfBh(rbt->root->bh);//找到链接点
	node<T> *curr = new node<T>(k);//默认赋予红色
	node<T> *pr = r->parent;
	curr->left = r;
	r->parent = curr;
	curr->right = rbt->root;
	rbt->root->parent = curr;
	curr->parent = pr;
	curr->bh = rbt->root->bh + 1;//设置节点k的黑高度
	if (pr != nil)//如果两棵树黑高度不相同
		pr->right = curr;
	else//否则curr为新根
		root = curr;
	insertFixup(curr);//调整
}

template <typename T>
void RBTree<T>::joinLeft(const T &k, RBTree *rbt)
{//将较矮的红黑树rbt以及节点k合并到较高红黑树T上,其中key[T]>=k>=key[rbt],与joinRight对称
	node<T> *r = locateMinNodeOfBh(rbt->root->bh);
	node<T> *curr = new node<T>(k);//默认赋予红色
	node<T> *pr = r->parent;
	curr->right = r;
	r->parent = curr;
	curr->left = rbt->root;
	rbt->root->parent = curr;
	curr->parent = pr;
	curr->bh = rbt->root->bh + 1;
	if (pr != nil)//如果两棵树黑高度不相同
		pr->left = curr;
	else
		root = curr;
	insertFixup(curr);
}

template <typename T>
void RBTree<T>::destroy()
{
	while (root != nil)
	{
		cout << "erase: " << root->key << endl;
		erase(root->key);
	}
	delete nil;
}

int main()
{//26 17 41 14 21 30 47 10 16 19 23 28 38 7 12 15 20 35 39 3该例子可以很好的囊括删除case1234
	//11 2 14 1 7 15 5 8 4该例子可以很好的囊括插入case123  
	//58 89 767 67 79 779 3787 88 89
	RBTree<int> rbt1;
	cout << "-------------create rbt1------------" << endl;
	rbt1.create();
	cout << "------------inTraversal rbt1--------" << endl;
	rbt1.inTraversal();
	cout << "------------preTraversal rbt1-------" << endl;
	rbt1.preTraversal();


	RBTree<int> rbt2;
	cout << "-------------create rbt2------------" << endl;
	rbt2.create();
	cout << "------------inTraversal rbt2--------" << endl;
	rbt2.inTraversal();
	cout << "------------preTraversal rbt2-------" << endl;
	rbt2.preTraversal();


	rbt1.joinRight(50, &rbt2);
	cout << "------------inTraversal joinRight--------" << endl;
	rbt1.inTraversal();
	cout << "------------preTraversal joinRight-------" << endl;
	rbt1.preTraversal();

	rbt1.destroy();
	getchar();
	return 0;
}
