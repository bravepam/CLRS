
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

/*****顺序统计树，是一般红黑树的扩张
*增加了size域，存储以当前节点为根的统计树的规模
*增加了rank,keyRank,select以及I_successor四个成员函数
*修改了insert和erase以及insertFixup和eraseFixup函数，
*以支持在节点删除和插入时对size域的维护
*/
#include<iostream>
#include<iomanip>

using namespace std;
enum COLOR { red, black };//枚举，定义颜色

template <typename T> class OSTree;

template <typename T>
class node
{
private:
	friend class OSTree<T>;
	node *parent;
	node *left;
	node *right;
	T key;
	size_t size;
	COLOR color;
	node(){}//默认构造函数，只供创建nil时调用
public:
	node(const T &k, COLOR c = red) :key(k), color(c), size(1),
		parent(NULL), left(NULL), right(NULL){}
	T& getKey(){ return key; }
	const T& getKey()const { return key; }
	//省略指针域的getter和setter
};

template <typename T>
class OSTree
{
private:
	static node<T> *nil;//哨兵，静态成员，被整个OSTree类所共有
	node<T> *root;
	OSTree(const OSTree&);//只声明不定义，以禁止复制构造
	OSTree operator=(const OSTree&);//禁止赋值
	void leftRotate(node<T>*);//左旋
	void rightRotate(node<T>*);//右旋
	void insertFixup(node<T>*);//插入节点后红黑性质调整
	void eraseFixup(node<T>*);//删除节点后红黑性质调整
public:
	OSTree() :root(nil)
	{
		nil->parent = nil;
		nil->left = nil;
		nil->right = nil;
		nil->color = black;
		nil->size = 0;//nil的size域为0
	}
	OSTree(node<T> *rbt) :root(rbt){}//复制构造函数，用于创建子红黑树对象
	void insert(const T&);//插入
	void create();//创建红黑树
	void erase(const T&);//删除
	size_t rank(node<T> *curr)const;//返回curr节点的排序数
	size_t keyRank(const T &k)const;//返回关键字为k在此树中的排序数
	node<T>* I_successor(node<T> *curr, size_t i)const;//查找curr节点的第i个后继
	node<T>* select(size_t i)const;//在树中查找中序遍历第i个节点
	node<T>* locate(const T&)const;//查找
	node<T>* minimum()const;//最小值
	node<T>* maximum()const;//最大值
	node<T>* successor(const T&)const;//找后继
	node<T>* predecessor(const T&)const;//前驱
	void preTraversal()const;//先根遍历
	void inTraversal()const;//中根遍历
	void destroy();//销毁红黑树
	bool empty()const{ return root == nil; }//判空
};

template <typename T> node<T> *OSTree<T>::nil = new node<T>;//定义静态成员nil

template <typename T>
void OSTree<T>::leftRotate(node<T> *curr)
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
		rchild->size = curr->size;
		curr->size = curr->left->size + curr->right->size + 1;
	}
}

template <typename T>
void OSTree<T>::rightRotate(node<T> *curr)
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
		lchild->size = curr->size;
		curr->size = curr->left->size + curr->right->size + 1;
	}
}

template <typename T>
void OSTree<T>::insert(const T &k)
{
	node<T> *pkey = new node<T>(k),
		*p = nil, *curr = root;
	while (curr != nil)
	{//找插入位置
		++curr->size;
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
void OSTree<T>::insertFixup(node<T> *curr)
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
void OSTree<T>::create()
{
	T k;
	cout << "Enter element(s),CTRL+Z to end" << endl;//换行后CTRL+Z结束输入
	while (cin >> k)
		insert(k);
	cin.clear();
}

template <typename T>
void OSTree<T>::preTraversal()const
{
	node<T> *curr = root;
	if (curr != nil)
	{
		cout << curr->key << " : ";
		if (curr->color == red) cout << left << setw(12) << "red";
		else cout << left << setw(12) << "black";
		cout << "size: " << curr->size << endl;
		OSTree LEFT(curr->left);//继续左子树先根遍历
		LEFT.preTraversal();
		OSTree RIGHT(curr->right);
		RIGHT.preTraversal();
	}
}

template <typename T>
void OSTree<T>::inTraversal()const
{
	node<T> *curr = root;
	if (curr != nil)
	{
		OSTree LEFT(curr->left);
		LEFT.inTraversal();
		cout << curr->key << " : ";
		if (curr->color == red) cout << left << setw(12) << "red";
		else cout << left << setw(12) << "black";
		cout << "size: " << curr->size << endl;
		OSTree RIGHT(curr->right);//继续右子树中根遍历
		RIGHT.inTraversal();
	}
}

template <typename T>
node<T>* OSTree<T>::successor(const T &k)const
{
	node<T> *curr = locate(k);
	if (curr->right != nil)
	{//若右子树不为空，则后继为右子树最小值
		OSTree RIGHT(curr->right);
		return RIGHT.minimum();
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
node<T>* OSTree<T>::minimum()const
{
	node<T> *curr = root;
	while (curr->left != nil)
		curr = curr->left;
	return curr;
}

template <typename T>
node<T>* OSTree<T>::maximum()const
{
	node<T> *curr = root;
	while (curr->right != nil)
		curr = curr->right;
	return curr;
}

template <typename T>
node<T>* OSTree<T>::predecessor(const T &k)const
{
	node<T> *curr = locate(k);
	if (curr->left != nil)
	{//若左子树不为空，则前驱为左子树最大值
		OSTree LEFT(curr->left);
		return LEFT.maximum();
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
void OSTree<T>::erase(const T &k)
{
	node<T> *curr = locate(k), *pdel, *child;
	if (curr->left == nil || curr->right == nil)//决定删除节点
		pdel = curr;//若当前节点至多有一个孩子，则删除它
	else pdel = successor(k);//否则若有两孩子，则删除其后继
	node<T> *par = pdel->parent;
	while (par != nil)
	{
		--par->size;
		par = par->parent;
	}
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
void OSTree<T>::eraseFixup(node<T> *curr)
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

template <typename T>
node<T>* OSTree<T>::locate(const T &k)const
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
node<T>* OSTree<T>::select(size_t i)const
{
	node<T> *curr = root;
	size_t rank = curr->left->size + 1;
	while (i != rank && curr != nil)
	{
		if (i < rank) curr = curr->left;
		else
		{//在右子树中
			curr = curr->right;
			i = i - rank;
		}
		rank = curr->left->size + 1;
	}
	return curr;
}

template <typename T>
size_t OSTree<T>::rank(node<T> *curr)const
{
	if (curr == nil) return 0;
	size_t rank = curr->left->size + 1;
	while (curr->parent != nil)
	{
		if (curr == curr->parent->right) //如果是父亲右孩子，则排序数还要加上左兄弟树节点数及其根
			rank = rank + curr->parent->left->size + 1;
		curr = curr->parent;
	}
	return rank;
}

template <typename T>
size_t OSTree<T>::keyRank(const T &k)const
{
	if (root == nil)
	{
		cout << "Error,no the key!" << endl;
		exit(0);
	}
	node<T> *curr = root;
	if (k == curr->key) return curr->left->size + 1;
	else if (k < curr->key)
	{//在左子树找
		OSTree<T> LEFT(curr->left);
		return LEFT.keyRank(k);
	}
	else
	{//右子树找
		OSTree<T> RIGHT(curr->right);
		return RIGHT.keyRank(k) + curr->left->size + 1;
	}
}

template <typename T>
node<T>* OSTree<T>::I_successor(node<T> *curr, size_t i)const
{
	size_t r = rank(curr);//得到节点curr的排序数
	return select(r + i);//查找第r + i个节点，即curr节点的第i个后继
}

template <typename T>
void OSTree<T>::destroy()
{
	while (root != nil)
	{
		cout << "erase: " << root->key << endl;
		erase(root->key);
	}
	delete nil;
}

int main()
{//26 17 41 14 21 30 47 10 16 19 23 28 38 7 12 15 20 35 39 3
	//11 2 14 1 7 15 5 8 4,以下按本组数据测试
	OSTree<int> ost;
	cout << "create----------" << endl;
	ost.create();
	cout << "inTraversal-----" << endl;
	ost.inTraversal();
	cout << "preTraversal----" << endl;
	ost.preTraversal();
	node<int> *curr = ost.select(5);
	cout << curr->getKey() << endl;
	cout << ost.rank(curr) << endl;
	cout << ost.I_successor(curr, 3)->getKey() << endl;
	cout << ost.keyRank(14) << endl;
	ost.destroy();
	getchar();
	return 0;
}

