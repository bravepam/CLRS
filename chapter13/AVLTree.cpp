/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/
//AVL平衡二叉树

#include<iostream>

using namespace std;

#define MAX(i,j) (i > j ? i : j)

template <typename Key,typename Value>
struct node
{//树节点
	Key key;
	Value value;
	int height = 0;
	node *left = nullptr;
	node *right = nullptr;
	node(const Key &k, const Value &v) :key(k), value(v){}
	void print()const
	{
		cout << "key: " << key << "\t\t" << "value: " << value << '\t' << "height: " << height << endl;
	}
};

template <typename Key,typename Value,class CompareKey = less<Key>>
class AVLTree
{//AVL树
private:
	typedef node<Key, Value>								node;
	node *root;
	CompareKey compare;//比较器
private:
	AVLTree(node *r, const CompareKey &c) :root(r), compare(c){}//该构造函数只供内部使用
	node* insert_aux(node*, const Key&, const Value&);//插入辅助
	node* erase_aux(node*, const Key&);
	node* erase_successor(node*,node*&);//当删除节点有两个孩子时，将删除其后继代替
	node* locate_aux(node*, const Key&)const;
	node* leftRotate(node*, node*);//左旋
	node* rightRotate(node*, node*);
	node* leftRightRotate(node*, node*);//先左旋，再右旋
	node* rightLeftRotate(node*, node*);
	void destroy_aux(node*);
	int getHeight(node *p)const
	{//获得当前节点的高度
		if (p == nullptr) return -1;
		else return p->height;
	}
public:
	AVLTree() :root(nullptr), compare(){}
	AVLTree(const CompareKey &c) :root(nullptr), compare(c){}
	bool empty()const
	{
		return root == nullptr; 
	}
	void insert(const Key &k, const Value &v)
	{
		root = insert_aux(root, k, v);
	}
	void erase(const Key &k)
	{
		erase_aux(root, k);
	}
	bool locate(const Key &k)const
	{
		return (locate_aux(root, k) != nullptr);
	}
	bool edit(const Key &k, const Value &new_value)
	{
		node *p = locate_aux(root,k);
		if (p != nullptr) p->value = new_value;
		return (p != nullptr);
	}
	void clear()
	{//清空树
		destroy_aux(root);
		root = nullptr;
	}
	void inTraversal()const;
	void preTraversal()const;
	~AVLTree(){}
};

template <typename K,typename V,class CK>
node<K, V>* AVLTree<K, V, CK>::insert_aux(node *r,const K &k, const V &v)
{//插入辅助函数
	if (r == nullptr)//若已到叶子
		r = new node(k, v);
	else if (compare(k, r->key))
	{//否则，若插入关键字较当前关键字小
		r->left = insert_aux(r->left, k, v);//递归向左子树插入
		if (getHeight(r->left) - getHeight(r->right) == 2)
		{//若插入后，左子树过高，失去平衡
			if (compare(k, r->left->key))//1、在当前树的左子树的左边插入
				r = rightRotate(r, r->left);//则一次右旋即可，并更新新根
			else r = leftRightRotate(r, r->left);//2、在当前树的左子树的右边插入，则先左旋，再右旋
		}
	}
	else if (compare(r->key,k))
	{//若插入关键字较大
		r->right = insert_aux(r->right, k, v);//则递归右子树插入
		if (getHeight(r->right) - getHeight(r->left) == 2)
		{//若右子树过高
			if (compare(r->right->key, k))//3、在右子树的右边插入
				r = leftRotate(r, r->right);
			else r = rightLeftRotate(r, r->right);//4、在右子树的左边插入
		}
	}
	r->height = MAX(getHeight(r->left), getHeight(r->right)) + 1;//最后更新该子树根的高度
	return r;
}

template <typename K,typename V,class CK>
node<K, V>* AVLTree<K, V, CK>::erase_aux(node *r, const K &k)
{//删除辅助
	if (r == nullptr) return nullptr;
	else if (compare(k, r->key))
	{//若关键字k较小
		r->left = erase_aux(r->left, k);//则递归左子树删除
		if (getHeight(r->right) - getHeight(r->left) == 2)
		{//若删除后，左子树过低
			if (r->right->right != nullptr)//a、若当前子树根有右孙子
				r = leftRotate(r, r->right);//则左旋
			else r = rightLeftRotate(r, r->right);//b、否则，先右旋，再左旋
		}
	}
	else if (compare(r->key, k))
	{//若关键字较大，与上述情况对称，不赘述
		r->right = erase_aux(r->right, k);
		if (getHeight(r->left) - getHeight(r->right) == 2)
		{
			if (r->left->left != nullptr)
				r = rightRotate(r, r->left);
			else r = leftRightRotate(r, r->left);
		}
	}
	else
	{//若正是要删除的关键字
		if (!(r->left == nullptr || r->right == nullptr))
		{//若该子树有两个孩子
			node *del;
			r->right = erase_successor(r->right,del);//则删除后继，del记下将要被删除的节点
			r->key = del->key;
			r->value = del->value;
			delete del;
		}
		else
		{//若至多有一个，则删除其本身
			node *child;
			if (r->left != nullptr)
				child = r->left;
			else child = r->right;
			delete r;
			return child;//并直接返回孩子
		}
	}
	r->height = MAX(getHeight(r->left), getHeight(r->right)) + 1;//更新当前根的高度
	return r;
}

template <typename K,typename V,class CK>
node<K, V>* AVLTree<K, V, CK>::erase_successor(node *curr,node *&del)
{//删除某节点后继，即curr子树的最小关键字节点
	if (curr->left == nullptr)
	{//若当前正是最小值节点
		del = curr;//则将删除它
		return curr->right;//返回其右孩子
	}
	else
	{//否则
		curr->left = erase_successor(curr->left,del);//递归删除最小值节点
		if (getHeight(curr->right) - getHeight(curr->left) == 2)
		{//若删除后左子树过低
			if (curr->right->right != nullptr)//则进行如下调整
				curr = leftRotate(curr, curr->right);
			else curr = rightLeftRotate(curr, curr->right);
		}
		return curr;
	}
}

template <typename K,typename V,class CK>
node<K, V>* AVLTree<K, V, CK>::locate_aux(node *r,const K &k)const
{
	if (r == nullptr)//若没有该关键字
		return nullptr;
	else if (compare(k, r->key))//小于则往左找
		return locate_aux(r->left, k);
	else if (compare(r->key, k))//大于则往右找
		return locate_aux(r->right, k);
	else return r;//等于则直接返回
}

template <typename K,typename V,class CK>
void AVLTree<K, V, CK>::destroy_aux(node *r)
{
	if (r == nullptr) return;
	if (r->left != nullptr)
		destroy_aux(r->left);
	if (r->right != nullptr)
		destroy_aux(r->right);
	delete r;
}

template <typename K,typename V,class CK>
inline node<K, V>* AVLTree<K, V, CK>::leftRotate(node *par, node *child)
{//左旋
	par->right = child->left;
	child->left = par;
	par->height = MAX(getHeight(par->left), getHeight(par->right)) + 1;//更新高度
	child->height = MAX(getHeight(child->left), getHeight(child->right)) + 1;
	return child;//返回新根
}

template <typename K,typename V,class CK>
inline node<K, V>* AVLTree<K, V, CK>::rightRotate(node *par, node *child)
{//右旋
	par->left = child->right;
	child->right = par;
	par->height = MAX(getHeight(par->left), getHeight(par->right)) + 1;
	child->height = MAX(getHeight(child->left), getHeight(child->right)) + 1;
	return child;
}

template <typename K,typename V,class CK>
inline node<K, V>* AVLTree<K, V, CK>::leftRightRotate(node *par, node *child)
{//左、右旋
	par->left = leftRotate(child, child->right);//先将左孩子绕右孙子左旋，并更新左孩子
	return rightRotate(par, par->left);//再将其绕新左孩子右旋
}

template <typename K,typename V,class CK>
inline node<K, V>* AVLTree<K, V, CK>::rightLeftRotate(node *par, node *child)
{//右、左旋
	par->right = rightRotate(child, child->left);
	return leftRotate(par, par->right);
}

template <typename K,typename V,class CK>
void AVLTree<K, V, CK>::inTraversal()const
{//中序遍历
	if (root == nullptr) return;
	AVLTree L(root->left,compare);
	L.inTraversal();
	root->print();
	AVLTree R(root->right, compare);
	R.inTraversal();
}

template <typename K,typename V,class CK>
void AVLTree<K, V, CK>::preTraversal()const
{//先序遍历
	if (root == nullptr) return;
	root->print();
	AVLTree L(root->left, compare);
	L.preTraversal();
	AVLTree R(root->right, compare);
	R.preTraversal();
}

#include<vector>

int main()
{
	AVLTree<int, int> t;
	for (int i = 1; i <= 20; i += 2)
		t.insert(i, i * i);//键、值
	t.insert(10, 100);
	t.inTraversal();
	cout << "------------------------" << endl;
	t.preTraversal();
	cout << "------------------------" << endl;
	t.erase(1);
	t.erase(3);
	t.erase(5);
	t.erase(9);
	t.erase(11);
	t.erase(13);
	t.erase(100);//删除不存在的键
	t.inTraversal();
	cout << "------------------------" << endl;
	t.preTraversal();
	getchar();
	t.clear();
	return 0;
}