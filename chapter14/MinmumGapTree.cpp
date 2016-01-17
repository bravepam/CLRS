
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

/****��С�������ά��һ����������������������Ĳ�ֵ(>=0)�����������
 *������minGap,min,max���ݳ�Ա������min��maxֻ��Ϊ��ά��minGap�����
 *������update��keep�Լ�minGap����
 *update�ڸ���ĳ�ڵ���Ϣ��keep�ɵ����ϸ������ڵ���Ϣ
 ****/
#include<iostream>

using namespace std;
enum COLOR {red,black};//ö�٣�������ɫ
const int MAX = 0x7fffffff;
//#define Max(i,j,k) (i > j ?(i > k ? i : k) : (j > k ? j : k));
//#define Min3(i,j,k) (i < j ?(i < k ? i : k) : (j < k ? j : k));

int Min4(int a,int b,int c,int d) 
{//���ĸ�������Сֵ
	a = a < b ? a :b; 
	c = c < d ? c : d; 
	return a < c ? a : c;
}

class node
{
private:
	friend class MGTree;
	node *parent;
	node *left;
	node *right;
	int key;//�ؼ���
	int minGap;//�Դ˽ڵ�Ϊ����������С���
	int min;//�Դ˽ڵ�Ϊ����������Сֵ
	int max;//�Դ˽ڵ�Ϊ�����������ֵ
	COLOR color;
	node(){}//Ĭ�Ϲ��캯����ֻ������nilʱ����
public:
	node(int k,COLOR c = red):key(k),minGap(MAX),min(k),max(k),
		color(c),parent(NULL),left(NULL),right(NULL){}
	void print()const
	{
		printf("%-10dMinGap: %-15dmin: %-6dmax: %-6d",key,minGap,min,max);
		if(color == red) printf("red\n");
		else printf("black\n");
		/*cout << "key: " << key << "\tMinGap: " << minGap
			 << "\t\tmin: " << min << "\tmax:    " << max;
		if(color == red) cout << "\tred" << endl;
		else cout << "\tblack" << endl;*/
	}
	//ʡ��ָ�����getter��setter
};


class MGTree
{
private:
	static node *nil;//�ڱ�����̬��Ա��������MGTree��������
	node *root;
	MGTree(const MGTree&);//��ֹ���ƹ���
	MGTree operator=(const MGTree&);//��ֹ��ֵ
	void leftRotate(node*);//����
	void rightRotate(node*);//����
	void insertFixup(node*);//����ڵ�������ʵ���
	void eraseFixup(node*);//ɾ���ڵ�������ʵ���
	void update(node*);//���½ڵ���Ϣ
	void keep(node*);//����MGTree�������ʣ�����insert��erase����
public:
	MGTree():root(nil)
	{
		root->parent = nil;
		root->left = nil;
		root->right = nil;
		root->color = black;
		root->key = MAX;root->max = MAX;
		root->min = MAX;root->minGap = MAX;
	}
	MGTree(node *rbt):root(rbt){}//���ƹ��캯�������ڴ����Ӻ��������
	void insert(int);//����
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
	void minGap()const{ root->print();}//����������С���
	bool empty()const{return root == nil;}//�п�
};

 node *MGTree::nil = new node;//���徲̬��Աnil

void MGTree::update(node *curr)
{//���½ڵ���Ϣ
	curr->min = (curr->left == nil) ? curr->key : curr->left->min;
	curr->max = (curr->right == nil) ? curr->key : curr->right->max;
	int leftMinGap = (curr->left == nil) ? MAX : curr->left->minGap;
	int rightMinGap = (curr->right == nil) ? MAX : curr->right->minGap;
	int currLeftGap = (curr->left == nil) ? MAX : (curr->key - curr->left->max);
	int currRightGap = (curr->right == nil) ? MAX : (curr->right->min - curr->key);
	curr->minGap = Min4(leftMinGap,rightMinGap,currLeftGap,currRightGap);
}

void MGTree::keep(node *curr)
{
	while(curr != nil)
	{
		update(curr);
		curr = curr->parent;
	}
}

void MGTree::leftRotate(node *curr)
{
	if(curr->right != nil)
	{//�����Һ���ʱ��������
		node *rchild = curr->right;
		curr->right = rchild->left;
		if(rchild->left != nil)
			rchild->left->parent = curr;
		rchild->parent = curr->parent;
		if(curr->parent == nil)
			root = rchild;
		else if(curr == curr->parent->left)
			curr->parent->left = rchild;
		else curr->parent->right = rchild;
		curr->parent = rchild;
		rchild->left = curr;
		rchild->min = curr->min;
		rchild->max = curr->max;
		rchild->minGap = curr->minGap;
		update(curr);
	}
}


void MGTree::rightRotate(node *curr)
{
	if(curr->left != nil)
	{//��������ʱ��������
		node *lchild = curr->left;
		curr->left = lchild->right;
		if(lchild->right != nil)
			lchild->right->parent = curr;
		lchild->parent = curr->parent;
		if(curr->parent == nil)
			root = lchild;
		else if(curr == curr->parent->left)
			curr->parent->left = lchild;
		else curr->parent->right = lchild;
		lchild->right = curr;
		curr->parent = lchild;
		lchild->min = curr->min;
		lchild->max = curr->max;
		lchild->minGap = curr->minGap;
		update(curr);
	}
}


void MGTree::insert(int k)
{
	node *pkey = new node(k),
		*p = nil,*curr = root;
	while(curr != nil)
	{//�Ҳ���λ��
		p = curr;//��ס��ǰ�ڵ㸸��
		if(k < curr->key)//������
			curr = curr->left;
		else curr = curr->right;//������
	}
	pkey->parent = p;
	if(p == nil)//������ǵ�һ���ڵ�
		root = pkey;
	else if(k < p->key)
		p->left = pkey;
	else p->right = pkey;
	pkey->left = pkey->right = nil;
	keep(pkey->parent);
	insertFixup(pkey);//����
}


void MGTree::insertFixup(node *curr)
{
	while(curr->parent->color == red)
	{//����Ϊ��ڵ�ʱ����Ҫ����ѭ������
		if(curr->parent == curr->parent->parent->left)
		{//�������游����
			node *uncle = curr->parent->parent->right;
			if(uncle != nil && uncle->color == red)
			{//���1������ڵ������Ϊ��ɫ
				curr->parent->color = black;
				uncle->color = black;
				curr->parent->parent->color = red;
				curr = curr->parent->parent;
			}
			else if(curr == curr->parent->right)
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
			if(uncle != nil && uncle->color == red)
			{//���1
				curr->parent->color = black;
				uncle->color = black;
				curr->parent->parent->color = red;
				curr = curr->parent->parent;
			}
			else if(curr == curr->parent->left)
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


void MGTree::create()
{
	int k;
	cout << "Enter element(s),CTRL+Z to end" << endl;//���к�CTRL+Z��������
	while(cin >> k)
		insert(k);
	cin.clear();
}


void MGTree::preTraversal()const
{
	node *curr = root;
	if(curr != nil)
	{
		curr->print();
		MGTree LEFT(curr->left);//�����������ȸ�����
		LEFT.preTraversal();
		MGTree RIGHT(curr->right);
		RIGHT.preTraversal();
	}
}


void MGTree::inTraversal()const
{
	node *curr = root;
	if(curr != nil)
	{
		MGTree LEFT(curr->left);
		LEFT.inTraversal();
		curr->print();
		MGTree RIGHT(curr->right);//�����������и�����
		RIGHT.inTraversal();
	}
}


node* MGTree::successor(int k)const
{
	node *curr = locate(k);
	if(curr->right != nil)
	{//����������Ϊ�գ�����Ϊ��������Сֵ
		MGTree RIGHT(curr->right);
		return RIGHT.minMum();
	}
	node *p = curr->parent;
	while(p != nil && curr == p->right)
	{//����Ϊ����ָ��һֱ����ֱ����һ�����䴦�ڵ�
		curr = p;
		p = p->parent;
	}
	return p;
}


node* MGTree::minMum()const
{
	node *curr = root;
	while(curr->left != nil)
		curr = curr->left;
	return curr;
}


node* MGTree::maxMum()const
{
	node *curr = root;
	while(curr->right != nil)
		curr = curr->right;
	return curr;
}


node* MGTree::predecessor(int k)const
{
	node *curr = locate(k);
	if(curr->left != nil)
	{//����������Ϊ�գ���ǰ��Ϊ���������ֵ
		MGTree LEFT(curr->left);
		return LEFT.maxMum();
	}
	node *p = curr->parent;
	while(p != nil && curr == p->left)
	{//����Ϊ����ָ��һֱ���ϵĵ�һ�����䴦�ڵ�
		curr = p;
		p = p->parent;
	}
	return p;
}


void MGTree::erase(int k)
{
	node *curr = locate(k),*pdel,*child;
	if(curr == nil)
	{
		cout << "Error:no data!" << endl;
		return ;
	}
	if(curr->left == nil || curr->right == nil)//����ɾ���ڵ�
		pdel = curr;//����ǰ�ڵ�������һ�����ӣ���ɾ����
	else pdel = successor(k);//�������������ӣ���ɾ������
	node *pdel_parent = pdel->parent;//���±�ɾ�ڵ㸸ĸ
	if(pdel->left != nil)//���²�Ϊ�յĺ���
		child = pdel->left;
	else child = pdel->right;
	child->parent = pdel_parent;
	if(pdel_parent == nil)//��ɾ�����Ǹ��ڵ�
		root = child;
	else if(pdel == pdel_parent->left)//��������ɾ�ڵ����丸������
		pdel_parent->left = child;
	else pdel_parent->right = child;
	if(curr != pdel)
		curr->key = pdel->key;//����ɾ���Ǻ�̣��򽫺��ֵ������ǰ�ڵ�
	keep(pdel_parent);//�Ե�����ά���ڵ���Ϣ
	if(pdel->color == black)//��ɾ�ڵ�Ϊ��ɫʱ�ŵ���
		eraseFixup(child);
	delete pdel;//�ͷ���ռ�ڴ�
}


void MGTree::eraseFixup(node *curr)
{
	while(curr != root && curr->color == black)
	{//��ǰ��Ϊ������Ϊ��ɫ
		if(curr == curr->parent->left)
		{//�����Ǹ�������
			node *brother = curr->parent->right;//�ֵܽڵ�϶�����
			if(brother->color == red)
			{//���1���ֵ��Ǻ�ɫ��ת��Ϊ���2,3,4
				brother->color = black;
				curr->parent->color = red;
				leftRotate(curr->parent);
				brother = curr->parent->right;
			}
			if(brother->left->color == black && brother->right->color == black)
			{//���2���ֵ��Ǻ�ɫ����������Ҳ�Ǻ�ɫ������ǰ�ڵ���ֵ�ȥһ�غ�ɫ
				brother->color = red;
				curr = curr->parent;
			}
			else if(brother->right->color == black)
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
			if(brother->color == red)
			{//���1
				brother->color = black;
				curr->parent->color = red;
				rightRotate(curr->parent);
				brother = curr->parent->left;
			}
			if(brother->right->color == black && brother->left->color == black)
			{//���2
				brother->color = red;
				curr = curr->parent;
			}
			else if(brother->left->color == black)
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


node* MGTree::locate(int k)const
{
	node *curr = root;
	while(curr != nil && curr->key != k)
	{
		if(k < curr->key)curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}

void MGTree::destroy()
{
	while(root != nil)
	{
		//cout << "erase: " << root->key << endl;
		erase(root->key);
	}
	delete nil;
}

int main()
{//1 5 9 15 18 22
	MGTree mgt;
	cout << "create----------" << endl;
	mgt.create();
	int choice,k;
	cout << "Enter your choice,1->insert,2->delete,3->minGap,4->print tree,5->exit" << endl;
	do
	{
		cin >> choice;
		switch(choice)
		{
		case 1:
			cout << "Enter a integer: ";
			cin >> k;
			mgt.insert(k);
			break;
		case 2:
			cout << "Enter a integer: ";
			cin >> k;
			mgt.erase(k);
			break;
		case 3:
			if(!mgt.empty())mgt.minGap();
			else cout << "Error:empty!" << endl;
			break;
		case 4:
			cout << "inTraversal-----" << endl;
			mgt.inTraversal();
			cout << "preTraversal----" << endl;
			mgt.preTraversal();
			break;
		case 5:
			cout << "destroy---------" << endl;
			mgt.destroy();
			return 0;
		default:
			cout << "Bad input!" << endl;
		}
		cout << "Enter your choice,1->insert,2->delete,3->minGap,4->print tree,5->exit" << endl;
	}while(choice >= 1 && choice <= 5);
	return 0;
}