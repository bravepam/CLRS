
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//treap���ݽṹ

#include<iostream>
#include<assert.h>
#include<random>

using namespace std;

template <typename Key,typename Value>
struct node
{//�ڵ�
	Key key;//��
	Value value;//ֵ
	int priority;//���ȼ�
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
class treap//MaxPri��ʾ���ȼ���Χ��Ĭ����1~10000
{//treap�ṹ
public:
	typedef node<Key,Value>					node;
private:
	node *root;
	CompareKey compare;
	void destroy(node*);//����treap
	void leftRotate(node*, node*);//����
	void rightRotate(node*, node*);//����
	void insertFixup(node*);//��������
	void eraseFixup(node*);//ɾ�������
	node* successor(node*)const;//����
	int getRandomPriority()const;//ȡ��һ��������ȼ�

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
	void levelTraversal()const;//��α���
	void clear()
	{
		destroy(root);
		root = nullptr;
	}
};

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
void treap<Key,Value,CompareKey,MaxPri>::leftRotate(node *x, node *y)
{//x����y����ת
	assert(x != nullptr && y != nullptr);
	y->parent = x->parent;
	if (x->parent != nullptr)
	{//����x�ĸ��ڵ�ĺ���
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
{//y����x����
	assert(y != nullptr && x != nullptr);
	x->parent = y->parent;
	if (y->parent != nullptr)
	{//����y�ĸ��ڵ�ĺ���
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
{//��������
	node *par = curr->parent;
	while (par != nullptr && curr->priority < par->priority)
	{//���δ�����ڵ��ҵ�ǰ�ڵ�����ȼ��ȸ��ڵ�С
		if (curr == par->right)//�����Һ���
			leftRotate(par, curr);//������
		else rightRotate(par, curr);//��������
		par = curr->parent;
	}
	if (par == nullptr) root = curr;
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
void treap<Key,Value,CompareKey,MaxPri>::eraseFixup(node *curr)
{//ɾ�������
	node *p_min = curr;
	//�ҵ������е����ȼ���С��
	if (curr->left != nullptr && curr->left->priority < p_min->priority)
		p_min = curr->left;
	if (curr->right != nullptr && curr->right->priority < p_min->priority)
		p_min = curr->right;
	if (p_min != curr)
	{
		if (p_min == curr->left)
		{//���������ȼ���С
			rightRotate(curr, p_min);//������
			if (curr->right != nullptr)
				//Ȼ�����Һ��ӿ�ʼ���ϵ�������Ϊ�Һ����п����ǵڶ�С���������Ҳ����insertFixup
				//�����������һ��
				insertFixup(curr->right);
		}
		else
		{//ͬ����
			leftRotate(curr, p_min);
			if (curr->left != nullptr)
				insertFixup(curr->left);
		}
	}
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
node<Key,Value>* treap<Key,Value,CompareKey,MaxPri>::successor(node *curr)const
{//����
	if (curr->right != nullptr)
	{//�����������Ϊ��
		node *p = curr->right;
		while (p->left != nullptr)
			p = p->left;//����������������Сֵ�ڵ�
		return p;
	}
	node *par = curr->parent;
	while (par != nullptr && curr == par->right)
	{//����һֱ���Ҳ����ϣ�ֱ����һ�����䴦����Ϊ���
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
	random_device rd;//����
	mt19937 gen(rd());//����÷ɭ��ת�����������
	uniform_int_distribution<int> dist(1, MaxPri);//�������ȷֲ���Χ
	return dist(gen);//���һ�������
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
bool treap<Key,Value,CompareKey,MaxPri>::insert(const Key &k,const Value &v)
{
	if (root == nullptr)//��������ǵ�һ���ڵ�
		root = new node(k,v, getRandomPriority());
	else
	{
		node *curr = root,*par = nullptr;
		while (curr != nullptr)
		{//��Ѱ�����
			par = curr;
			if (compare(k,curr->key))
				curr = curr->left;
			else curr = curr->right;
		}
		if (par->key == k) return false;//����ùؼ����Ѿ����ڣ������ʧ��
		curr = new node(k, v,getRandomPriority());
		curr->parent = par;
		if (compare(k, par->key)) par->left = curr;
		else par->right = curr;
		insertFixup(curr);//����
	}
	return true;
}

template <typename Key,typename Value,class CompareKey = less<Key>,int MaxPri = 10000>
bool treap<Key,Value,CompareKey,MaxPri>::erase(const Key &k)
{//ɾ��
	node *curr = locate(k);//��λɾ���ڵ�
	if (curr == nullptr) return false;
	node *del, *child;
	if (curr->left == nullptr || curr->right == nullptr)//ȷ��ʵ�ʽ���ɾ���Ľڵ�
		del = curr;
	else del = successor(curr);
	if (del->left != nullptr)//���±�ɾ�ڵ�ĺ���
		child = del->left;
	else child = del->right;
	if (child != nullptr)
		child->parent = del->parent;//�޸ĺ��Ӹ��ڵ�
	if (del == del->parent->left)//�޸ı�ɾ��㸸ĸ�ĺ���
		del->parent->left = child;
	else del->parent->right = child;
	if (del != curr)
	{//��������
		curr->key = del->key;
		curr->priority = del->priority;
		eraseFixup(curr);//����
	}
	delete del;
	return true;
}

template <typename Key, typename Value, class CompareKey, int MaxPri = 10000>
bool treap<Key, Value, CompareKey, MaxPri>::edit(const Key &k, const Value &new_value)
{//�޸�ĳ�����������Ľڵ�ֵ
	node *curr = locate(k);//��λ
	if (curr == nullptr) return false;//��������
	curr->value = new_value;
	return true;
}

template <typename Key,typename Value,class CompareKey,int MaxPri = 10000>
node<Key,Value>* treap<Key, Value, CompareKey, MaxPri>::locate(const Key &k)const
{//����
	node *curr = root;
	while (curr != nullptr && !(!compare(k, curr->key) && !compare(curr->key, k)))
	{//�����ǰ�ڵ㲻��Ҫ�ҵĽڵ�
		if (compare(k, curr->key)) curr = curr->left;
		else curr = curr->right;//���������
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
{//��α����ⷨ3
	if (root == nullptr) return;
	root->print(); cout << endl;
	vector<node*> pvec;//����һ�����飨���߶��У��洢�ѷ��ʹ��Ľڵ�
	pvec.push_back(root);
	size_t curr = 0, last = 1;
	while (curr != pvec.size())
	{//ֻҪ�����л�����Ԫ��
		size_t num = 0;//��¼����ӵ�Ԫ����
		for (; curr != last; ++curr)
		{//������ǰ���Ԫ��
			if (pvec[curr]->left != nullptr)
			{//����
				pvec[curr]->left->print();
				pvec.push_back(pvec[curr]->left);
				++num;
			}
			if (pvec[curr]->right != nullptr)
			{//�Һ���
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