
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>

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
	COLOR color;
	node(){}//默认构造函数，只供创建nil时调用
public:
	node(const T &k, COLOR c = red) :key(k), color(c),
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
	}
	RBTree(node<T> *rbt) :root(rbt){}//复制构造函数，用于创建子红黑树对象
	void insert(const T&);//插入
	void create();//创建红黑树
	void erase(const T&);//删除
	node<T>* locate(const T&)const;//查找
	node<T>* minMum()const;//最小值
	node<T>* maxMum()const;//最大值
	node<T>* successor(const T&)const;//找后继
	node<T>* predecessor(const T&)const;//前驱
	void preTraversal()const;//先根遍历
	void inTraversal()const;//中根遍历
	void destroy();//销毁红黑树
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
		if (curr->color == red) cout << "red" << endl;
		else cout << "black" << endl;
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
		cout << curr->key << " : ";
		if (curr->color == red) cout << "red" << endl;
		else cout << "black" << endl;
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
{//12 5 18 2 9 15 19 17
	RBTree<int> rbt;
	cout << "-------------create------------" << endl;
	rbt.create();
	cout << "------------inTraversal--------" << endl;
	rbt.inTraversal();
	rbt.insert(10);
	rbt.insert(-12);
	rbt.insert(100);
	cout << "---after insert inTraversal----" << endl;
	rbt.inTraversal();
	cout << "--------------min_max----------" << endl;
	cout << "max: " << rbt.maxMum()->getKey() << endl;
	cout << "min: " << rbt.minMum()->getKey() << endl;
	cout << "--------------suc_prede----------" << endl;
	cout << "12_suc: " << rbt.successor(12)->getKey() << endl;
	cout << "12_prede: " << rbt.predecessor(12)->getKey() << endl;
	cout << "------------preTraversal-------" << endl;
	rbt.preTraversal();
	cout << "--------------locate-----------" << endl;
	cout << "17_locate: " << rbt.locate(17)->getKey() << endl;
	rbt.erase(100);
	cout << "---after erase inTraversal----" << endl;
	rbt.inTraversal();
	cout << "------------destroy-----------" << endl;
	rbt.destroy();
	getchar();
	return 0;
}

insertFixup(node<T> *pkey)
{
	while (pkey != root && pkey->priority < pkey->parent->priority)
	{
		if (pkey == pkey->parent->right)
			leftRotate(pkey->parent);
		if (pkey == pkey->parent->left)
			rightRoate(pkey->parent);
	}
}