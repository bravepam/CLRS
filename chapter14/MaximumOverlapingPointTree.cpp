
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

/*****�������������ص��㣬���ҳ�Ϊ����ص����������������
*������p,p_sum,max,pom�����ݳ�Ա
*������update��keep����
*΢��insert���������Լ�erase����
*/
#include<iostream>
//#include<cstdlib>

using namespace std;
enum COLOR { red, black };//ö�٣�������ɫ
#define Max(i,j,k) (i > j ? (i > k ? i : k) : (j > k) ? j : k);
const int MAX = 0x7fffffff;

class node
{
private:
	friend class POMTree;
	node *parent;
	node *left;
	node *right;
	int key;
	int p;//��˵�Ϊ1���Ҷ˵�Ϊ-1
	int p_sum;//�Ե�ǰ�ڵ�Ϊ����������p�ĺ�
	int max;//�Ե�ǰ�ڵ�Ϊ����������p�ĺ͵����ֵ
	int pom;//ʹ��ȡ�����ֵ�Ĺؼ���,��������������ص���
	COLOR color;
	node(){}//Ĭ�Ϲ��캯����ֻ������nilʱ����
public:
	node(int k, COLOR c = red) :key(k), p_sum(0), max(0), pom(k), color(c),
		parent(NULL), left(NULL), right(NULL){}
	void print()const
	{
		printf("key:%-10d POM:%-10d max:%-10d p_sum:%-10d p:%-10d", key, pom, max, p_sum, p);
		if (color == red) printf("red\n");
		else printf("black\n");
	}
	//ʡ��ָ�����getter��setter
};


class POMTree
{
private:
	static node *nil;//�ڱ�����̬��Ա��������POMTree��������
	node *root;
	POMTree(const POMTree&);//��ֹ���ƹ���
	POMTree operator=(const POMTree&);//��ֹ��ֵ
	void leftRotate(node*);//����
	void rightRotate(node*);//����
	void insertFixup(node*);//����ڵ�������ʵ���
	void eraseFixup(node*);//ɾ���ڵ�������ʵ���
	void update(node*);//���½ڵ���Ϣ
	void keep(node*);//�Ե����ϸ���·����Ϣ
public:
	POMTree() :root(nil)
	{
		root->parent = nil;
		root->left = nil;
		root->right = nil;
		root->color = black;
		root->key = MAX; root->p = 0;
		root->p_sum = 0; root->max = 0;
		root->pom = MAX;
	}
	POMTree(node *rbt) :root(rbt){}//���ƹ��캯�������ڴ����Ӻ��������
	void insert(int, bool);//����
	void create();//���������
	void erase(int);//ɾ��
	node* locate(int)const;//����
	node* minMum()const;//��Сֵ
	node* maxMum()const;//���ֵ
	node* successor(int)const;//�Һ��
	node* predecessor(int)const;//ǰ��
	void preTraversal()const;//�ȸ�����
	void inTraversal()const;//�и�����
	void destroy();//���ٺ����
	void findPom()const { root->print(); }
	bool empty()const{ return root == nil; }//�п�
};

node *POMTree::nil = new node;//���徲̬��Աnil

void POMTree::update(node *curr)
{
	curr->p_sum = curr->left->p_sum + curr->p + curr->right->p_sum;
	curr->max = Max(curr->left->max, curr->left->p_sum + curr->p,
		curr->left->p_sum + curr->p + curr->right->max);
	if (curr->left != nil && curr->max == curr->left->max) curr->pom = curr->left->pom;
	else if (curr->right != nil &&
		curr->max == curr->left->p_sum + curr->p + curr->right->max)curr->pom = curr->right->pom;
	else curr->pom = curr->key;
}

void POMTree::keep(node *curr)
{
	while (curr != nil)
	{
		update(curr);
		curr = curr->parent;
	}
}

void POMTree::leftRotate(node *curr)
{
	if (curr->right != nil)
	{//�����Һ���ʱ��������
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
		rchild->p_sum = curr->p_sum;
		rchild->max = curr->max;
		rchild->pom = curr->pom;
		update(curr);
	}
}

void POMTree::rightRotate(node *curr)
{
	if (curr->left != nil)
	{//��������ʱ��������
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
		lchild->p_sum = curr->p_sum;
		lchild->max = curr->max;
		lchild->pom = curr->pom;
		update(curr);
	}
}


void POMTree::insert(int k, bool start)
{
	node *pkey = new node(k),
		*p = nil, *curr = root;
	if (start)
	{
		pkey->p = 1;
		pkey->p_sum = 1;
		pkey->max = 1;
		pkey->pom = k;
	}
	else
	{
		pkey->p = -1;
		pkey->p_sum = -1;
		pkey->max = 0;
		pkey->pom = k;
	}
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
	keep(pkey->parent);
	insertFixup(pkey);//����
}


void POMTree::insertFixup(node *curr)
{
	while (curr->parent->color == red)
	{//����Ϊ��ڵ�ʱ����Ҫ����ѭ������
		if (curr->parent == curr->parent->parent->left)
		{//�������游����
			node *uncle = curr->parent->parent->right;
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
			node *uncle = curr->parent->parent->left;
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


void POMTree::create()
{
	int low, high;
	cout << "Enter element(s),CTRL+Z to end" << endl;//���к�CTRL+Z��������
	while (cin >> low >> high)
	{
		insert(low, true);
		insert(high, false);
	}
	cin.clear();
}


void POMTree::preTraversal()const
{
	node *curr = root;
	if (curr != nil)
	{
		curr->print();
		POMTree LEFT(curr->left);//�����������ȸ�����
		LEFT.preTraversal();
		POMTree RIGHT(curr->right);
		RIGHT.preTraversal();
	}
}


void POMTree::inTraversal()const
{
	node *curr = root;
	if (curr != nil)
	{
		POMTree LEFT(curr->left);
		LEFT.inTraversal();
		curr->print();
		POMTree RIGHT(curr->right);//�����������и�����
		RIGHT.inTraversal();
	}
}


node* POMTree::successor(int k)const
{
	node *curr = locate(k);
	if (curr->right != nil)
	{//����������Ϊ�գ�����Ϊ��������Сֵ
		POMTree RIGHT(curr->right);
		return RIGHT.minMum();
	}
	node *p = curr->parent;
	while (p != nil && curr == p->right)
	{//����Ϊ����ָ��һֱ����ֱ����һ�����䴦�ڵ�
		curr = p;
		p = p->parent;
	}
	return p;
}


node* POMTree::minMum()const
{
	node *curr = root;
	while (curr->left != nil)
		curr = curr->left;
	return curr;
}


node* POMTree::maxMum()const
{
	node *curr = root;
	while (curr->right != nil)
		curr = curr->right;
	return curr;
}


node* POMTree::predecessor(int k)const
{
	node *curr = locate(k);
	if (curr->left != nil)
	{//����������Ϊ�գ���ǰ��Ϊ���������ֵ
		POMTree LEFT(curr->left);
		return LEFT.maxMum();
	}
	node *p = curr->parent;
	while (p != nil && curr == p->left)
	{//����Ϊ����ָ��һֱ���ϵĵ�һ�����䴦�ڵ�
		curr = p;
		p = p->parent;
	}
	return p;
}


void POMTree::erase(int k)
{
	node *curr = locate(k), *pdel, *child;
	if (curr == nil)
	{
		cout << "Error:no data" << endl;
		return;
	}
	if (curr->left == nil || curr->right == nil)//����ɾ���ڵ�
		pdel = curr;//����ǰ�ڵ�������һ�����ӣ���ɾ����
	else pdel = successor(k);//�������������ӣ���ɾ������
	node *pdel_parent = pdel->parent;//���±�ɾ�ڵ㸸��
	if (pdel->left != nil)//���²�Ϊ�յĺ���
		child = pdel->left;
	else child = pdel->right;
	child->parent = pdel_parent;
	if (pdel_parent == nil)//��ɾ�����Ǹ��ڵ�
		root = child;
	else if (pdel == pdel_parent->left)//��������ɾ�ڵ����丸������
		pdel_parent->left = child;
	else pdel_parent->right = child;
	if (curr != pdel)
		curr->key = pdel->key;//����ɾ���Ǻ�̣��򽫺��ֵ������ǰ�ڵ�
	keep(pdel_parent);
	if (pdel->color == black)//��ɾ�ڵ�Ϊ��ɫʱ�ŵ���
		eraseFixup(child);
	delete pdel;//�ͷ���ռ�ڴ�
}


void POMTree::eraseFixup(node *curr)
{
	while (curr != root && curr->color == black)
	{//��ǰ��Ϊ������Ϊ��ɫ
		if (curr == curr->parent->left)
		{//�����Ǹ�������
			node *brother = curr->parent->right;//�ֵܽڵ�϶�����
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
			node *brother = curr->parent->left;
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


node* POMTree::locate(int k)const
{
	node *curr = root;
	while (curr != nil && curr->key != k)
	{
		if (k < curr->key)curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}


void POMTree::destroy()
{
	while (root != nil)
	{
		//cout << "erase: " << root->key << endl;
		erase(root->key);
	}
	delete nil;
}

int main()
{//16 21   8 9   25 30   5 8   15 23   19 20   17 19   26 26   0 3   6 10
	//16 21   8 9   25 30   5 8   15 23  17 19   26 26   0 3   6 10   19 20 
	//7 9   5 8   0 3   6 10  ��ʱ�ò�����ȷ�𰸣�ʵ�ڵ��������ˣ����£�
	POMTree ptree;
	ptree.create();
	cout << "inTraversal-----" << endl;
	ptree.inTraversal();
	cout << "preTraversal----" << endl;
	ptree.preTraversal();
	int low,high,choice;
	cout << "Enter your choice,1-insert,2-delete,3-POM,4-print,5-exit" << endl;
	do
	{
	cin >> choice;
	switch(choice)
	{
	case 1:
	cout << "Enter a interval,ex-> a b (a <= b): ";
	cin >> low >> high;
	ptree.insert(low,true);
	ptree.insert(high,false);
	break;
	case 2:
	cout << "Enter a interval,ex -> a b (a <= b): ";
	cin >> low >> high;
	ptree.erase(low);
	ptree.erase(high);
	break;
	case 3:
	ptree.findPom();
	break;
	case 4:
	cout << "inTraversal-----" << endl;
	ptree.inTraversal();
	cout << "preTraversal----" << endl;
	ptree.preTraversal();
	break;
	case 5:
	ptree.destroy();
	return 0;
	default:
	cout << "Bad choice" << endl;
	}
	//cout << "Enter your choice,1-insert,2-delete,3-POM,4-print,5-exit" << endl;
	}while(choice >= 1 && choice <= 5);
	ptree.destroy();
	return 0;
}
