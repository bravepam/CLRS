
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

/*****˳��ͳ��������һ������������
*������size�򣬴洢�Ե�ǰ�ڵ�Ϊ����ͳ�����Ĺ�ģ
*������rank,keyRank,select�Լ�I_successor�ĸ���Ա����
*�޸���insert��erase�Լ�insertFixup��eraseFixup������
*��֧���ڽڵ�ɾ���Ͳ���ʱ��size���ά��
*/
#include<iostream>
#include<iomanip>

using namespace std;
enum COLOR { red, black };//ö�٣�������ɫ

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
	node(){}//Ĭ�Ϲ��캯����ֻ������nilʱ����
public:
	node(const T &k, COLOR c = red) :key(k), color(c), size(1),
		parent(NULL), left(NULL), right(NULL){}
	T& getKey(){ return key; }
	const T& getKey()const { return key; }
	//ʡ��ָ�����getter��setter
};

template <typename T>
class OSTree
{
private:
	static node<T> *nil;//�ڱ�����̬��Ա��������OSTree��������
	node<T> *root;
	OSTree(const OSTree&);//ֻ���������壬�Խ�ֹ���ƹ���
	OSTree operator=(const OSTree&);//��ֹ��ֵ
	void leftRotate(node<T>*);//����
	void rightRotate(node<T>*);//����
	void insertFixup(node<T>*);//����ڵ�������ʵ���
	void eraseFixup(node<T>*);//ɾ���ڵ�������ʵ���
public:
	OSTree() :root(nil)
	{
		nil->parent = nil;
		nil->left = nil;
		nil->right = nil;
		nil->color = black;
		nil->size = 0;//nil��size��Ϊ0
	}
	OSTree(node<T> *rbt) :root(rbt){}//���ƹ��캯�������ڴ����Ӻ��������
	void insert(const T&);//����
	void create();//���������
	void erase(const T&);//ɾ��
	size_t rank(node<T> *curr)const;//����curr�ڵ��������
	size_t keyRank(const T &k)const;//���عؼ���Ϊk�ڴ����е�������
	node<T>* I_successor(node<T> *curr, size_t i)const;//����curr�ڵ�ĵ�i�����
	node<T>* select(size_t i)const;//�����в������������i���ڵ�
	node<T>* locate(const T&)const;//����
	node<T>* minimum()const;//��Сֵ
	node<T>* maximum()const;//���ֵ
	node<T>* successor(const T&)const;//�Һ��
	node<T>* predecessor(const T&)const;//ǰ��
	void preTraversal()const;//�ȸ�����
	void inTraversal()const;//�и�����
	void destroy();//���ٺ����
	bool empty()const{ return root == nil; }//�п�
};

template <typename T> node<T> *OSTree<T>::nil = new node<T>;//���徲̬��Աnil

template <typename T>
void OSTree<T>::leftRotate(node<T> *curr)
{
	if (curr->right != nil)
	{//�����Һ���ʱ��������
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
	{//��������ʱ��������
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
	{//�Ҳ���λ��
		++curr->size;
		p = curr;//��ס��ǰ�ڵ㸸��
		if (k < curr->key)//������
			curr = curr->left;
		else curr = curr->right;//������
	}
	pkey->parent = p;
	if (p == nil)//������ǵ�һ���ڵ�
		root = pkey;
	else if (k < p->key)
		p->left = pkey;
	else p->right = pkey;
	pkey->left = pkey->right = nil;
	insertFixup(pkey);//����
}

template <typename T>
void OSTree<T>::insertFixup(node<T> *curr)
{
	while (curr->parent->color == red)
	{//����Ϊ��ڵ�ʱ����Ҫ����ѭ������
		if (curr->parent == curr->parent->parent->left)
		{//�������游����
			node<T> *uncle = curr->parent->parent->right;
			if (uncle != nil && uncle->color == red)
			{//���1������ڵ������Ϊ��ɫ
				curr->parent->color = black;
				uncle->color = black;
				curr->parent->parent->color = red;
				curr = curr->parent->parent;
			}
			else if (curr == curr->parent->right)
			{//���2������ڵ�Ϊ��ɫ���ҵ�ǰ�ڵ��Ǹ����Һ���
				curr = curr->parent;
				leftRotate(curr);//�����ڵ���������ת��Ϊ���3
			}
			else
			{//���3������ڵ�Ϊ��ɫ���ҵ�ǰ�ڵ��Ǹ�������
				curr->parent->color = black;
				curr->parent->parent->color = red;
				rightRotate(curr->parent->parent);
			}
		}
		else
		{//�������游�Һ��ӣ���������������Գ�
			node<T> *uncle = curr->parent->parent->left;
			if (uncle != nil && uncle->color == red)
			{//���1
				curr->parent->color = black;
				uncle->color = black;
				curr->parent->parent->color = red;
				curr = curr->parent->parent;
			}
			else if (curr == curr->parent->left)
			{//���2
				curr = curr->parent;
				rightRotate(curr);
			}
			else
			{//���3
				curr->parent->color = black;
				curr->parent->parent->color = red;
				leftRotate(curr->parent->parent);
			}
		}
	}
	root->color = black;//����ѭ��ʱ�����ڵ���Ϊ��ɫ
}

template <typename T>
void OSTree<T>::create()
{
	T k;
	cout << "Enter element(s),CTRL+Z to end" << endl;//���к�CTRL+Z��������
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
		OSTree LEFT(curr->left);//�����������ȸ�����
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
		OSTree RIGHT(curr->right);//�����������и�����
		RIGHT.inTraversal();
	}
}

template <typename T>
node<T>* OSTree<T>::successor(const T &k)const
{
	node<T> *curr = locate(k);
	if (curr->right != nil)
	{//����������Ϊ�գ�����Ϊ��������Сֵ
		OSTree RIGHT(curr->right);
		return RIGHT.minimum();
	}
	node<T> *p = curr->parent;
	while (p != nil && curr == p->right)
	{//����Ϊ����ָ��һֱ����ֱ����һ�����䴦�ڵ�
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
	{//����������Ϊ�գ���ǰ��Ϊ���������ֵ
		OSTree LEFT(curr->left);
		return LEFT.maximum();
	}
	node<T> *p = curr->parent;
	while (p != nil && curr == p->left)
	{//����Ϊ����ָ��һֱ���ϵĵ�һ�����䴦�ڵ�
		curr = p;
		p = p->parent;
	}
	return p;
}

template <typename T>
void OSTree<T>::erase(const T &k)
{
	node<T> *curr = locate(k), *pdel, *child;
	if (curr->left == nil || curr->right == nil)//����ɾ���ڵ�
		pdel = curr;//����ǰ�ڵ�������һ�����ӣ���ɾ����
	else pdel = successor(k);//�������������ӣ���ɾ������
	node<T> *par = pdel->parent;
	while (par != nil)
	{
		--par->size;
		par = par->parent;
	}
	if (pdel->left != nil)//���²�Ϊ�յĺ���
		child = pdel->left;
	else child = pdel->right;
	child->parent = pdel->parent;
	if (pdel->parent == nil)//��ɾ�����Ǹ��ڵ�
		root = child;
	else if (pdel == pdel->parent->left)//��������ɾ�ڵ����丸������
		pdel->parent->left = child;
	else pdel->parent->right = child;
	if (curr != pdel)
		curr->key = pdel->key;//����ɾ���Ǻ�̣��򽫺��ֵ������ǰ�ڵ�
	if (pdel->color == black)//��ɾ�ڵ�Ϊ��ɫʱ�ŵ���
		eraseFixup(child);
	delete pdel;//�ͷ���ռ�ڴ�
}

template <typename T>
void OSTree<T>::eraseFixup(node<T> *curr)
{
	while (curr != root && curr->color == black)
	{//��ǰ��Ϊ������Ϊ��ɫ
		if (curr == curr->parent->left)
		{//�����Ǹ�������
			node<T> *brother = curr->parent->right;//�ֵܽڵ�϶�����
			if (brother->color == red)
			{//���1���ֵ��Ǻ�ɫ��ת��Ϊ���2,3,4
				brother->color = black;
				curr->parent->color = red;
				leftRotate(curr->parent);
				brother = curr->parent->right;
			}
			if (brother->left->color == black && brother->right->color == black)
			{//���2���ֵ��Ǻ�ɫ����������Ҳ�Ǻ�ɫ������ǰ�ڵ���ֵ�ȥһ�غ�ɫ
				brother->color = red;
				curr = curr->parent;
			}
			else if (brother->right->color == black)
			{//���3���ֵ�����Ϊ�죬�Һ���Ϊ�ڣ�ת��Ϊ���4
				brother->color = red;
				brother->left->color = black;
				rightRotate(brother);
				brother = curr->parent->right;
			}
			else
			{//���4���Һ���Ϊ��ɫ����������
				brother->color = curr->parent->color;
				curr->parent->color = black;
				brother->right->color = black;
				leftRotate(curr->parent);
				curr = root;
			}
		}
		else
		{//�����Ǹ����Һ��ӣ���������������Գ�
			node<T> *brother = curr->parent->left;
			if (brother->color == red)
			{//���1
				brother->color = black;
				curr->parent->color = red;
				rightRotate(curr->parent);
				brother = curr->parent->left;
			}
			if (brother->right->color == black && brother->left->color == black)
			{//���2
				brother->color = red;
				curr = curr->parent;
			}
			else if (brother->left->color == black)
			{//���3
				brother->color = red;
				brother->right->color = black;
				leftRotate(brother);
				brother = curr->parent->left;
			}
			else
			{//���4
				brother->color = curr->parent->color;
				curr->parent->color = black;
				brother->left->color = black;
				rightRotate(curr->parent);
				curr = root;
			}
		}
	}
	curr->color = black;//����ѭ��ʱ����ǰ�ڵ���Ϊ��ɫ
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
		{//����������
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
		if (curr == curr->parent->right) //����Ǹ����Һ��ӣ�����������Ҫ�������ֵ����ڵ��������
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
	{//����������
		OSTree<T> LEFT(curr->left);
		return LEFT.keyRank(k);
	}
	else
	{//��������
		OSTree<T> RIGHT(curr->right);
		return RIGHT.keyRank(k) + curr->left->size + 1;
	}
}

template <typename T>
node<T>* OSTree<T>::I_successor(node<T> *curr, size_t i)const
{
	size_t r = rank(curr);//�õ��ڵ�curr��������
	return select(r + i);//���ҵ�r + i���ڵ㣬��curr�ڵ�ĵ�i�����
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
	//11 2 14 1 7 15 5 8 4,���°��������ݲ���
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

