/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/
//AVLƽ�������

#include<iostream>

using namespace std;

#define MAX(i,j) (i > j ? i : j)

template <typename Key,typename Value>
struct node
{//���ڵ�
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
{//AVL��
private:
	typedef node<Key, Value>								node;
	node *root;
	CompareKey compare;//�Ƚ���
private:
	AVLTree(node *r, const CompareKey &c) :root(r), compare(c){}//�ù��캯��ֻ���ڲ�ʹ��
	node* insert_aux(node*, const Key&, const Value&);//���븨��
	node* erase_aux(node*, const Key&);
	node* erase_successor(node*,node*&);//��ɾ���ڵ�����������ʱ����ɾ�����̴���
	node* locate_aux(node*, const Key&)const;
	node* leftRotate(node*, node*);//����
	node* rightRotate(node*, node*);
	node* leftRightRotate(node*, node*);//��������������
	node* rightLeftRotate(node*, node*);
	void destroy_aux(node*);
	int getHeight(node *p)const
	{//��õ�ǰ�ڵ�ĸ߶�
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
	{//�����
		destroy_aux(root);
		root = nullptr;
	}
	void inTraversal()const;
	void preTraversal()const;
	~AVLTree(){}
};

template <typename K,typename V,class CK>
node<K, V>* AVLTree<K, V, CK>::insert_aux(node *r,const K &k, const V &v)
{//���븨������
	if (r == nullptr)//���ѵ�Ҷ��
		r = new node(k, v);
	else if (compare(k, r->key))
	{//����������ؼ��ֽϵ�ǰ�ؼ���С
		r->left = insert_aux(r->left, k, v);//�ݹ�������������
		if (getHeight(r->left) - getHeight(r->right) == 2)
		{//����������������ߣ�ʧȥƽ��
			if (compare(k, r->left->key))//1���ڵ�ǰ��������������߲���
				r = rightRotate(r, r->left);//��һ���������ɣ��������¸�
			else r = leftRightRotate(r, r->left);//2���ڵ�ǰ�������������ұ߲��룬����������������
		}
	}
	else if (compare(r->key,k))
	{//������ؼ��ֽϴ�
		r->right = insert_aux(r->right, k, v);//��ݹ�����������
		if (getHeight(r->right) - getHeight(r->left) == 2)
		{//������������
			if (compare(r->right->key, k))//3�������������ұ߲���
				r = leftRotate(r, r->right);
			else r = rightLeftRotate(r, r->right);//4��������������߲���
		}
	}
	r->height = MAX(getHeight(r->left), getHeight(r->right)) + 1;//�����¸��������ĸ߶�
	return r;
}

template <typename K,typename V,class CK>
node<K, V>* AVLTree<K, V, CK>::erase_aux(node *r, const K &k)
{//ɾ������
	if (r == nullptr) return nullptr;
	else if (compare(k, r->key))
	{//���ؼ���k��С
		r->left = erase_aux(r->left, k);//��ݹ�������ɾ��
		if (getHeight(r->right) - getHeight(r->left) == 2)
		{//��ɾ��������������
			if (r->right->right != nullptr)//a������ǰ��������������
				r = leftRotate(r, r->right);//������
			else r = rightLeftRotate(r, r->right);//b��������������������
		}
	}
	else if (compare(r->key, k))
	{//���ؼ��ֽϴ�����������Գƣ���׸��
		r->right = erase_aux(r->right, k);
		if (getHeight(r->left) - getHeight(r->right) == 2)
		{
			if (r->left->left != nullptr)
				r = rightRotate(r, r->left);
			else r = leftRightRotate(r, r->left);
		}
	}
	else
	{//������Ҫɾ���Ĺؼ���
		if (!(r->left == nullptr || r->right == nullptr))
		{//������������������
			node *del;
			r->right = erase_successor(r->right,del);//��ɾ����̣�del���½�Ҫ��ɾ���Ľڵ�
			r->key = del->key;
			r->value = del->value;
			delete del;
		}
		else
		{//��������һ������ɾ���䱾��
			node *child;
			if (r->left != nullptr)
				child = r->left;
			else child = r->right;
			delete r;
			return child;//��ֱ�ӷ��غ���
		}
	}
	r->height = MAX(getHeight(r->left), getHeight(r->right)) + 1;//���µ�ǰ���ĸ߶�
	return r;
}

template <typename K,typename V,class CK>
node<K, V>* AVLTree<K, V, CK>::erase_successor(node *curr,node *&del)
{//ɾ��ĳ�ڵ��̣���curr��������С�ؼ��ֽڵ�
	if (curr->left == nullptr)
	{//����ǰ������Сֵ�ڵ�
		del = curr;//��ɾ����
		return curr->right;//�������Һ���
	}
	else
	{//����
		curr->left = erase_successor(curr->left,del);//�ݹ�ɾ����Сֵ�ڵ�
		if (getHeight(curr->right) - getHeight(curr->left) == 2)
		{//��ɾ��������������
			if (curr->right->right != nullptr)//��������µ���
				curr = leftRotate(curr, curr->right);
			else curr = rightLeftRotate(curr, curr->right);
		}
		return curr;
	}
}

template <typename K,typename V,class CK>
node<K, V>* AVLTree<K, V, CK>::locate_aux(node *r,const K &k)const
{
	if (r == nullptr)//��û�иùؼ���
		return nullptr;
	else if (compare(k, r->key))//С����������
		return locate_aux(r->left, k);
	else if (compare(r->key, k))//������������
		return locate_aux(r->right, k);
	else return r;//������ֱ�ӷ���
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
{//����
	par->right = child->left;
	child->left = par;
	par->height = MAX(getHeight(par->left), getHeight(par->right)) + 1;//���¸߶�
	child->height = MAX(getHeight(child->left), getHeight(child->right)) + 1;
	return child;//�����¸�
}

template <typename K,typename V,class CK>
inline node<K, V>* AVLTree<K, V, CK>::rightRotate(node *par, node *child)
{//����
	par->left = child->right;
	child->right = par;
	par->height = MAX(getHeight(par->left), getHeight(par->right)) + 1;
	child->height = MAX(getHeight(child->left), getHeight(child->right)) + 1;
	return child;
}

template <typename K,typename V,class CK>
inline node<K, V>* AVLTree<K, V, CK>::leftRightRotate(node *par, node *child)
{//������
	par->left = leftRotate(child, child->right);//�Ƚ�����������������������������
	return rightRotate(par, par->left);//�ٽ���������������
}

template <typename K,typename V,class CK>
inline node<K, V>* AVLTree<K, V, CK>::rightLeftRotate(node *par, node *child)
{//�ҡ�����
	par->right = rightRotate(child, child->left);
	return leftRotate(par, par->right);
}

template <typename K,typename V,class CK>
void AVLTree<K, V, CK>::inTraversal()const
{//�������
	if (root == nullptr) return;
	AVLTree L(root->left,compare);
	L.inTraversal();
	root->print();
	AVLTree R(root->right, compare);
	R.inTraversal();
}

template <typename K,typename V,class CK>
void AVLTree<K, V, CK>::preTraversal()const
{//�������
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
		t.insert(i, i * i);//����ֵ
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
	t.erase(100);//ɾ�������ڵļ�
	t.inTraversal();
	cout << "------------------------" << endl;
	t.preTraversal();
	getchar();
	t.clear();
	return 0;
}