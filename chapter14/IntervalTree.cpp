
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

/****������Interval Tree����������������
*������key_low��high��max���ݳ�Ա
*node������getInterval������ITree������searchϵ�к���
*�޸���������������֧�ֶ������������ά��
*/

#include<iostream>

using namespace std;
#define MAX(i,j,k) (i > j ? (i > k ? i : k):(j > k ? j : k));//�����������ֵ
enum COLOR { red, black };//ö�٣�������ɫ

class node
{
private:
	friend class ITree;
	node *parent;
	node *left;
	node *right;
	int key_low;//����������˵㣬Ҳ�ǽڵ�Ĺؼ���
	int high;//�����Ҷ˵�
	int max;//�Ե�ǰ�ڵ�Ϊ�����������Ҷ˵����ֵ
	COLOR color;
	node(){}//Ĭ�Ϲ��캯����ֻ������nilʱ����
public:
	node(int kl, int hi, COLOR c = red) :key_low(kl), high(hi), color(c),
		max(hi), parent(NULL), left(NULL), right(NULL){}
	void getInterval()//��ӡ������Ϣ
	{
		cout << '[' << key_low << ',' << high << "]\t" << max;
		if (color == red) cout << "\tred" << endl;
		else cout << "\tblack" << endl;
	}
	//ʡ��ָ�����getter��setter
};

class ITree
{
private:
	static node *nil;//�ڱ�����̬��Ա��������ITree��������
	node *root;
	ITree(const ITree&);//ֻ���������壬�Խ�ֹ���ƹ���
	ITree operator=(const ITree&);//��ֹ��ֵ
	void leftRotate(node*);//����
	void rightRotate(node*);//����
	void insertFixup(node*);//����ڵ���������ʵ���
	void eraseFixup(node*);//ɾ���ڵ���������ʵ���
public:
	ITree() :root(nil)
	{//����nil�ĸ�����
		root->parent = nil;
		root->left = nil;
		root->right = nil;
		root->color = black;
		nil->key_low = nil->high = nil->max = -1;
	}
	ITree(node *rbt) :root(rbt){}//���ƹ��캯�������ڴ���������������
	void insert(int, int);//����
	void create();//����������
	void erase(int);//ɾ��
	node* locate(int)const;//����
	node* search(int low, int high)const;//���������в�����[low,high]�ص�������
	node* searchMinLeftPoint(int low, int high)const;//���������в�����[low,high]�ص�����˵���С������
	void searchAll(int low, int high)const;//�����������ҳ�������[low,high]�ص�������
	node* searchExactly(int low, int high)const;//�����������ҳ���[low,high]��ȫƥ�������
	node* minMumInterval()const;//�ҵ�������˵���Сֵ����
	node* maxMumInterval()const;//������˵����ֵ����
	//node* longestInterval()const;//���ҳ����������
	node* successor(int)const;//�Һ������
	node* predecessor(int)const;//��ǰ������
	void inTraversal()const;//�и�����
	void destroy();//����������
	bool empty()const{ return root == nil; }//�п�
};

node *ITree::nil = new node;//���徲̬��Աnil

void ITree::leftRotate(node *curr)
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
		rchild->max = curr->max;
		curr->max = MAX(curr->max, curr->left->max, curr->right->max);
	}
}

void ITree::rightRotate(node *curr)
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
		lchild->max = curr->max;
		curr->max = MAX(curr->max, curr->left->max, curr->right->max);
	}
}

void ITree::insert(int low, int high)
{
	node *pkey = new node(low, high),
		*p = nil, *curr = root;
	while (curr != nil)
	{//�Ҳ���λ��
		p = curr;//��ס��ǰ�ڵ㸸��
		if (low < curr->key_low)//������
			curr = curr->left;
		else curr = curr->right;//������
	}
	pkey->parent = p;
	if (p == nil)//������ǵ�һ���ڵ�
		root = pkey;
	else if (low < p->key_low)
		p->left = pkey;
	else p->right = pkey;
	pkey->left = pkey->right = nil;
	while (p != nil)
	{
		p->max = MAX(p->max, p->left->max, p->right->max);
		p = p->parent;
	}
	insertFixup(pkey);//����
}

void ITree::insertFixup(node *curr)
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

void ITree::create()
{
	int low, high;
	cout << "Enter interval(s),CTRL+Z to end" << endl;//���к�CTRL+Z��������
	while (cin >> low >> high)
		insert(low, high);
	cin.clear();
}

void ITree::inTraversal()const
{
	node *curr = root;
	if (curr != nil)
	{
		ITree LEFT(curr->left);
		LEFT.inTraversal();
		curr->getInterval();//��ӡ������Ϣ
		ITree RIGHT(curr->right);//�����������и�����
		RIGHT.inTraversal();
	}
}

node* ITree::successor(int kl)const
{
	node *curr = locate(kl);
	if (curr->right != nil)
	{//����������Ϊ�գ�����Ϊ��������Сֵ
		ITree RIGHT(curr->right);
		return RIGHT.minMumInterval();
	}
	node *p = curr->parent;
	while (p != nil && curr == p->right)
	{//����Ϊ����ָ��һֱ����ֱ����һ�����䴦�ڵ�
		curr = p;
		p = p->parent;
	}
	return p;
}

node* ITree::minMumInterval()const
{
	node *curr = root;
	while (curr->left != nil)
		curr = curr->left;
	return curr;
}

node* ITree::maxMumInterval()const
{
	node *curr = root;
	while (curr->right != nil)
		curr = curr->right;
	return curr;
}

node* ITree::predecessor(int kl)const
{
	node *curr = locate(kl);
	if (curr->left != nil)
	{//����������Ϊ�գ���ǰ��Ϊ���������ֵ
		ITree LEFT(curr->left);
		return LEFT.maxMumInterval();
	}
	node *p = curr->parent;
	while (p != nil && curr == p->left)
	{//����Ϊ����ָ��һֱ���ϵĵ�һ�����䴦�ڵ�
		curr = p;
		p = p->parent;
	}
	return p;
}

void ITree::erase(int kl)
{
	node *curr = locate(kl), *pdel, *child;
	if (curr == nil) return;
	if (curr->left == nil || curr->right == nil)//����ɾ���ڵ�
		pdel = curr;//����ǰ�ڵ�������һ�����ӣ���ɾ����
	else pdel = successor(kl);//�������������ӣ���ɾ������
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
	{
		curr->key_low = pdel->key_low;//����ɾ���Ǻ�̣��򽫺��ֵ������ǰ�ڵ�
		curr->high = pdel->high;
	}
	curr = child->parent;
	while (curr != nil)
	{
		curr->max = MAX(curr->max, curr->left->max, curr->right->max);
		curr = curr->parent;
	}
	if (pdel->color == black)//��ɾ�ڵ�Ϊ��ɫʱ�ŵ���
		eraseFixup(child);
	delete pdel;//�ͷ���ռ�ڴ�
}

void ITree::eraseFixup(node *curr)
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

node* ITree::locate(int kl)const
{
	node *curr = root;
	while (curr != nil && curr->key_low != kl)
	{
		if (kl < curr->key_low)curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}

node* ITree::search(int low, int high)const
{
	node *curr = root;
	while (curr != nil && (low > curr->high || curr->key_low > high))
		if (curr->left->max >= low) curr = curr->left;
		else curr = curr->right;
		return curr;
}

node* ITree::searchMinLeftPoint(int low, int high)const
{
	node *curr = root, *pMin = nil;
	int minPoint = 0x7fffffff;
	while (curr != nil)
	{
		if ((curr->key_low <= high && low <= curr->high) && curr->key_low < minPoint)
		{
			pMin = curr;
			minPoint = curr->key_low;
		}
		if (curr->left != nil && curr->left->max >= low)
			curr = curr->left;
		else curr = curr->right;
	}
	return pMin;
}

void ITree::searchAll(int low, int high)const
{//�ݹ飬���������������ܴ�������������
	node *curr = root;
	if (curr->key_low <= high && low <= curr->high)
		curr->getInterval();
	if (curr->left != nil && curr->left->max >= low)
	{
		ITree LEFT(curr->left);
		LEFT.searchAll(low, high);
	}
	if (curr->right != nil)
	{
		ITree RIGHT(curr->right);
		RIGHT.searchAll(low, high);
	}
}

node* ITree::searchExactly(int low, int high)const
{
	node *curr = root;
	while (curr != nil)
	{
		if (curr->key_low == low && curr->high == high)
			return curr;
		if (curr->key_low >= low)//ע�����������search��һ������
			curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}

void ITree::destroy()
{
	while (root != nil)
	{
		//cout << "erase: " << root->key_low << endl;
		erase(root->key_low);
	}
	delete nil;
}

int main()
{//16 21   8 9   25 30   5 8   15 23   17 19   26 26   0 3   6 10   19 20
	ITree itree;
	cout << "creare-------------" << endl;
	itree.create();
	cout << "inTraversal--------" << endl;
	itree.inTraversal();
	cout << endl;
	itree.searchExactly(26, 26)->getInterval();
	cout << "search-------------" << endl;
	itree.search(22,25)->getInterval();
	cout << endl;
	itree.minMumInterval()->getInterval();
	cout << endl;
	itree.maxMumInterval()->getInterval();
	itree.erase(16);
	itree.erase(26);
	itree.erase(16);
	cout << endl;
	itree.inTraversal();
	itree.insert(13,19);
	cout << endl;
	itree.searchAll(17, 19);
	itree.inTraversal();
	itree.destroy();
	getchar();
	return 0;
}