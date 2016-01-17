
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

/*******�������ǿ��********
#�ڵ��Ա����˺ڸ߶�bh���ݳ�Ա
#��Ӧ��insertFixup��eraseFixup��������˶Ժڸ߶ȵĵ���
#����˺�������Ӳ���jion(Left/Right)��Ա�����Լ����׵Ĺ��ܺ���
*/
#include<iostream>
#include<iomanip>

using namespace std;
enum COLOR { red, black };//ö�٣�������ɫ

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
	int bh;//�ڸ߶�
	COLOR color;
	node(){}//Ĭ�Ϲ��캯����ֻ������nilʱ����
public:
	node(const T &k, COLOR c = red) :key(k), color(c), bh(1),
		parent(NULL), left(NULL), right(NULL){}
	T& getKey(){ return key; }
	const T& getKey()const { return key; }
	//ʡ��ָ�����getter��setter
};

template <typename T>
class RBTree
{
private:
	static node<T> *nil;//�ڱ�����̬��Ա��������RBTree��������
	node<T> *root;
	RBTree(const RBTree&);//��ֹ���ƹ���
	RBTree operator=(const RBTree&);//��ֹ��ֵ
	void leftRotate(node<T>*);//����
	void rightRotate(node<T>*);//����
	void insertFixup(node<T>*);//����ڵ�������ʵ���
	void eraseFixup(node<T>*);//ɾ���ڵ�������ʵ���
public:
	RBTree() :root(nil)
	{
		root->parent = nil;
		root->left = nil;
		root->right = nil;
		root->color = black;
		root->bh = -1;//�ڱ��ĺڸ߶���Ϊ-1.
	}
	RBTree(node<T> *rbt) :root(rbt){}//���ƹ��캯�������ڴ����Ӻ��������
	void insert(const T&);//����
	void create();//���������
	void erase(const T&);//ɾ��
	//��������(T & rbt)��һ��ֵk���ӣ��������ұߣ�����key[T]<=k<=key[rbt]
	void joinRight(const T &k, RBTree *rbt);
	//��������(T & rbt)��һ��ֵk���ӣ���������ߣ�����key[T]>=k>=key[rbt]
	void joinLeft(const T &k, RBTree *rbt);
	void preTraversal()const;//�ȸ�����
	void inTraversal()const;//�и�����
	void destroy();//���ٺ����
	node<T>* locateMaxNodeOfBh(int bh)const;//�ں�����в���ĳһ�ڸ߶ȵ����ֵ�ڵ�
	node<T>* locateMinNodeOfBh(int bh)const;//�ں�����в���ĳһ�ڸ߶ȵ���Сֵ�ڵ�
	node<T>* locate(const T&)const;//����
	node<T>* minMum()const;//��Сֵ
	node<T>* maxMum()const;//���ֵ
	node<T>* successor(const T&)const;//�Һ��
	node<T>* predecessor(const T&)const;//ǰ��
	bool empty()const{ return root == nil; }//�п�
};

template <typename T> node<T> *RBTree<T>::nil = new node<T>;//���徲̬��Աnil

template <typename T>
void RBTree<T>::leftRotate(node<T> *curr)
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
	}
}

template <typename T>
void RBTree<T>::rightRotate(node<T> *curr)
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
	}
}

template <typename T>
void RBTree<T>::insert(const T &k)
{
	node<T> *pkey = new node<T>(k),
		*p = nil, *curr = root;
	while (curr != nil)
	{//�Ҳ���λ��
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
void RBTree<T>::insertFixup(node<T> *curr)
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
				++curr->bh;
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
				++curr->bh;
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
void RBTree<T>::create()
{
	T k;
	cout << "Enter element(s),CTRL+Z to end" << endl;//���к�CTRL+Z��������
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
		RBTree LEFT(curr->left);//�����������ȸ�����
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
		RBTree RIGHT(curr->right);//�����������и�����
		RIGHT.inTraversal();
	}
}

template <typename T>
node<T>* RBTree<T>::successor(const T &k)const
{
	node<T> *curr = locate(k);
	if (curr->right != nil)
	{//����������Ϊ�գ�����Ϊ��������Сֵ
		RBTree RIGHT(curr->right);
		return RIGHT.minMum();
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
	{//����������Ϊ�գ���ǰ��Ϊ���������ֵ
		RBTree LEFT(curr->left);
		return LEFT.maxMum();
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
void RBTree<T>::erase(const T &k)
{
	node<T> *curr = locate(k), *pdel, *child;
	if (curr->left == nil || curr->right == nil)//����ɾ���ڵ�
		pdel = curr;//����ǰ�ڵ�������һ�����ӣ���ɾ����
	else pdel = successor(k);//�������������ӣ���ɾ������
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
void RBTree<T>::eraseFixup(node<T> *curr)
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
				--curr->bh;
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
				++brother->bh;
				--brother->left->bh;
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
				--curr->bh;
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
				++brother->bh;
				--brother->left->bh;
				curr = root;
			}
		}
	}
	curr->color = black;//����ѭ��ʱ����ǰ�ڵ���Ϊ��ɫ
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
{//���Һ�����Ϻڸ߶�Ϊbh�����ֵ�ڵ�
	node<T> *curr = root;
	if (curr == nil || curr->bh < bh) return nil;//��Ϊ�ջ��ߴ����ڸ߶�̫��
	while (curr->bh != bh)
		curr = curr->right;
	return curr;
}

template <typename T>
node<T>* RBTree<T>::locateMinNodeOfBh(int bh)const
{//���Һ�����Ϻڸ߶�Ϊbh����Сֵ�ڵ㣬��locateMaxNodeOfBh�Գ�
	node<T> *curr = root;
	if (curr == nil || curr->bh < bh) return nil;
	while (curr->bh != bh)
		curr = curr->left;
	return curr;
}

template <typename T>
void RBTree<T>::joinRight(const T &k, RBTree *rbt)
{//���ϰ��ĺ����rbt�Լ��ڵ�k�ϲ����ϸߺ����T��,����key[T]<=k<=key[rbt]
	node<T> *r = locateMaxNodeOfBh(rbt->root->bh);//�ҵ����ӵ�
	node<T> *curr = new node<T>(k);//Ĭ�ϸ����ɫ
	node<T> *pr = r->parent;
	curr->left = r;
	r->parent = curr;
	curr->right = rbt->root;
	rbt->root->parent = curr;
	curr->parent = pr;
	curr->bh = rbt->root->bh + 1;//���ýڵ�k�ĺڸ߶�
	if (pr != nil)//����������ڸ߶Ȳ���ͬ
		pr->right = curr;
	else//����currΪ�¸�
		root = curr;
	insertFixup(curr);//����
}

template <typename T>
void RBTree<T>::joinLeft(const T &k, RBTree *rbt)
{//���ϰ��ĺ����rbt�Լ��ڵ�k�ϲ����ϸߺ����T��,����key[T]>=k>=key[rbt],��joinRight�Գ�
	node<T> *r = locateMinNodeOfBh(rbt->root->bh);
	node<T> *curr = new node<T>(k);//Ĭ�ϸ����ɫ
	node<T> *pr = r->parent;
	curr->right = r;
	r->parent = curr;
	curr->left = rbt->root;
	rbt->root->parent = curr;
	curr->parent = pr;
	curr->bh = rbt->root->bh + 1;
	if (pr != nil)//����������ڸ߶Ȳ���ͬ
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
{//26 17 41 14 21 30 47 10 16 19 23 28 38 7 12 15 20 35 39 3�����ӿ��Ժܺõ�����ɾ��case1234
	//11 2 14 1 7 15 5 8 4�����ӿ��Ժܺõ���������case123  
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
