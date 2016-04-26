
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//treap数据结构

#include<iostream>
#include<assert.h>
#include<random>

using namespace std;

template <typename Key,typename Value>
struct node
{//节点
	Key key;//键
	Value value;//值
	int priority;//优先级
	node *parent = nullptr;
	node *left = nullptr;
	node *right = nullptr;
	node(const Key &k,const Value &v, int p) :key(k), value(v),priority(p){}
	void print()const
	{
		cout << key << '\t' << value/* << '\t' << priority*/ << endl;
	}
};

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
class treap//MaxPri表示优先级范围，默认是1~10000
{//treap结构
public:
	typedef node<Key,Value>					node;
private:
	node *root;
	CompareKey compare;
	void destroy(node*);//销毁treap
	void leftRotate(node*, node*);//左旋
	void rightRotate(node*, node*);//右旋
	void insertFixup(node*);//插入后调整
	void eraseFixup(node*);//删除后调整
	node* successor(node*)const;//求后继
	int getRandomPriority()const;//取得一个随机优先级

	treap& operator=(const treap&);
	treap(const treap&);
public:
	treap() :root(nullptr){}
	treap(node *r) :root(r){}
	bool empty()const { return root == nullptr; }
	bool insert(const Key&,const Value&);
	bool erase(const Key&);
	bool edit(const Key&, const Value&);
	node* locate(const Key&)const;
	void inTraversal()const;
	void levelTraversal()const;//层次遍历
	void clear()
	{
		destroy(root);
		root = nullptr;
	}
};

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
void treap<Key,Value,CompareKey,MaxPri>::leftRotate(node *x, node *y)
{//x绕着y左旋转
	assert(x != nullptr && y != nullptr);
	y->parent = x->parent;
	if (x->parent != nullptr)
	{//更新x的父节点的孩子
		if (x->parent->left == x)
			x->parent->left = y;
		else x->parent->right = y;
	}
	x->right = y->left;
	if (y->left != nullptr)
		y->left->parent = x;
	y->left = x;
	x->parent = y;
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
void treap<Key,Value,CompareKey,MaxPri>::rightRotate(node *y, node *x)
{//y绕着x右旋
	assert(y != nullptr && x != nullptr);
	x->parent = y->parent;
	if (y->parent != nullptr)
	{//更新y的父节点的孩子
		if (y == y->parent->left)
			y->parent->left = x;
		else y->parent->right = x;
	}
	y->left = x->right;
	if (x->right != nullptr)
		x->right->parent = y;
	x->right = y;
	y->parent = x;
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
void treap<Key,Value,CompareKey,MaxPri>::insertFixup(node *curr)
{//插入后调整
	node *par = curr->parent;
	while (par != nullptr && curr->priority < par->priority)
	{//如果未到根节点且当前节点的优先级比父节点小
		if (curr == par->right)//若是右孩子
			leftRotate(par, curr);//则左旋
		else rightRotate(par, curr);//否则右旋
		par = curr->parent;
	}
	if (par == nullptr) root = curr;
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
void treap<Key,Value,CompareKey,MaxPri>::eraseFixup(node *curr)
{//删除后调整
	node *p_min = curr;
	//找到三者中的优先级最小者
	if (curr->left != nullptr && curr->left->priority < p_min->priority)
		p_min = curr->left;
	if (curr->right != nullptr && curr->right->priority < p_min->priority)
		p_min = curr->right;
	if (p_min != curr)
	{
		if (p_min == curr->left)
		{//若左孩子优先级较小
			rightRotate(curr, p_min);//则右旋
			if (curr->right != nullptr)
				//然后以右孩子开始向上调整，因为右孩子有可能是第二小，这个过程也就是insertFixup
				//但是最多会调整一次
				insertFixup(curr->right);
		}
		else
		{//同上理
			leftRotate(curr, p_min);
			if (curr->left != nullptr)
				insertFixup(curr->left);
		}
	}
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
node<Key,Value>* treap<Key,Value,CompareKey,MaxPri>::successor(node *curr)const
{//求后继
	if (curr->right != nullptr)
	{//如果右子树不为空
		node *p = curr->right;
		while (p->left != nullptr)
			p = p->left;//则后继是右子树的最小值节点
		return p;
	}
	node *par = curr->parent;
	while (par != nullptr && curr == par->right)
	{//否则，一直沿右侧向上，直到第一个拐弯处，即为后继
		curr = par;
		par = curr->parent;
	}
	return par;
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
void treap<Key,Value,CompareKey,MaxPri>::destroy(node *root)
{
	if (root == nullptr) return;
	if (root->left != nullptr)
		destroy(root->left);
	if (root->right != nullptr)
		destroy(root->right);
	delete root;
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
inline int treap<Key,Value,CompareKey,MaxPri>::getRandomPriority()const
{
	random_device rd;//种子
	mt19937 gen(rd());//构造梅森旋转随机数生成器
	uniform_int_distribution<int> dist(1, MaxPri);//整数均匀分布范围
	return dist(gen);//获得一个随机数
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
bool treap<Key,Value,CompareKey,MaxPri>::insert(const Key &k,const Value &v)
{
	if (root == nullptr)//若插入的是第一个节点
		root = new node(k,v, getRandomPriority());
	else
	{
		node *curr = root,*par = nullptr;
		while (curr != nullptr)
		{//找寻插入点
			par = curr;
			if (compare(k,curr->key))
				curr = curr->left;
			else curr = curr->right;
		}
		if (par->key == k) return false;//如果该关键字已经存在，则插入失败
		curr = new node(k, v,getRandomPriority());
		curr->parent = par;
		if (compare(k, par->key)) par->left = curr;
		else par->right = curr;
		insertFixup(curr);//调整
	}
	return true;
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
bool treap<Key,Value,CompareKey,MaxPri>::erase(const Key &k)
{//删除
	node *curr = locate(k);//定位删除节点
	if (curr == nullptr) return false;
	node *del, *child;
	if (curr->left == nullptr || curr->right == nullptr)//确定实际将被删除的节点
		del = curr;
	else del = successor(curr);
	if (del->left != nullptr)//记下被删节点的孩子
		child = del->left;
	else child = del->right;
	if (child != nullptr)
		child->parent = del->parent;//修改孩子父节点
	if (del == del->parent->left)//修改被删结点父母的孩子
		del->parent->left = child;
	else del->parent->right = child;
	if (del != curr)
	{//复制数据
		curr->key = del->key;
		curr->priority = del->priority;
		eraseFixup(curr);//调整
	}
	delete del;
	return true;
}

template <typename Key, typename Value, class CompareKey, int MaxPri = 10000>
bool treap<Key, Value, CompareKey, MaxPri>::edit(const Key &k, const Value &new_value)
{//修改某管子所关联的节点值
	node *curr = locate(k);//定位
	if (curr == nullptr) return false;//若不存在
	curr->value = new_value;
	return true;
}

template <typename Key,typename Value,class CompareKey,int MaxPri = 10000>
node<Key,Value>* treap<Key, Value, CompareKey, MaxPri>::locate(const Key &k)const
{//查找
	node *curr = root;
	while (curr != nullptr && !(!compare(k, curr->key) && !compare(curr->key, k)))
	{//如果当前节点不是要找的节点
		if (compare(k, curr->key)) curr = curr->left;
		else curr = curr->right;//则继续迭代
	}
	return curr;
}

template <typename Key, typename Value, class CompareKey, int MaxPri = 10000>
void treap<Key, Value, CompareKey, MaxPri>::inTraversal()const
{
	if (root == nullptr) return;
	if (root->left != nullptr)
	{
		treap L(root->left);
		L.inTraversal();
	}
	root->print();
	if (root->right != nullptr)
	{
		treap R(root->right);
		R.inTraversal();
	}
}

template <typename Key, typename Value, class CompareKey, int MaxPri = 10000>
void treap<Key, Value, CompareKey, MaxPri>::levelTraversal()const
{//层次遍历解法3
	if (root == nullptr) return;
	root->print(); cout << endl;
	vector<node*> pvec;//利用一个数组（或者队列）存储已访问过的节点
	pvec.push_back(root);
	size_t curr = 0, last = 1;
	while (curr != pvec.size())
	{//只要数组中还存在元素
		size_t num = 0;//记录新添加的元素数
		for (; curr != last; ++curr)
		{//遍历当前层的元素
			if (pvec[curr]->left != nullptr)
			{//左孩子
				pvec[curr]->left->print();
				pvec.push_back(pvec[curr]->left);
				++num;
			}
			if (pvec[curr]->right != nullptr)
			{//右孩子
				pvec[curr]->right->print();
				pvec.push_back(pvec[curr]->right);
				++num;
			}
		}
		cout << endl;
		last += num;
	}
}

#include<vector>

int main()
{
	/*treap<char, int> t;
	vector<char> cvec = { 'G', 'B', 'H', 'A', 'E', 'K', 'I' };
	vector<int> vec = { 1, 2, 3, 4, 5, 6, 7 };
	for (size_t i = 0; i != cvec.size(); ++i)
		t.insert(cvec[i], vec[i]);
	t.inTraversal();
	cout << "---------------------------" << endl;
	t.levelTraversal();
	t.insert('C', 25);
	t.insert('D', 9);
	t.insert('F', 2);
	cout << "---------------------------" << endl;
	t.inTraversal();
	cout << "---------------------------" << endl;
	t.levelTraversal();*/
	//t.locate('C')->print();
	/*t.erase('C');
	t.erase('D');
	t.erase('F');*/
	treap<int, int> t;
	for (int i = 0; i != 100; ++i)
		t.insert(i, i * i);
	t.inTraversal();
	cout << "---------------------------" << endl;
	t.levelTraversal();
	t.clear();
	getchar();
	return 0;
}