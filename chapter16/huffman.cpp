
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<queue>
#include<vector>
#include<string>

using namespace std;

struct node
{//���������ڵ�
	char ch;//��¼
	size_t frequency;//Ƶ��
	node *left;
	node *right;
	node(char c, size_t f) :ch(c), frequency(f), left(NULL), right(NULL){}
	node() :ch('\0'), frequency(0), left(NULL), right(NULL){}
};

class huffmanTree
{//��������
public:
	bool typedef (*comp)(node*, node*);//����ָ�룬���ڶ���ȽϺ���
	huffmanTree(comp compare, node *r = NULL) :q(compare), root(r){}//���캯��������Ϊ�Ƚ����ṩ��ֵ
	void initQueue();//��ʼ���ڵ����
	void createHuff();//�����������
	void printCode();//��ӡ����������
	~huffmanTree()
	{
		destroy(root);
	}
private:
	node *root;
	priority_queue<node*, vector<node*>, comp> q;//���У�����vector<node*>���Ƚ���Ϊcomp����
	void printCode(node*,string&);
	void destroy(node*);

	huffmanTree& operator=(const huffmanTree&);
	huffmanTree(const huffmanTree&);
};

void huffmanTree::printCode(node *root,string &code = string(""))
{
	node *curr = root;
	if (curr == NULL) return;
	if (curr->left != NULL)
	{//����������Ϊ��
		code += "0";//�����߱���0
		printCode(curr->left, code);//��������������
		code.pop_back();//��������������ɾ�����봮���һλ����ΪҪ���ұ�����
	}
	else cout << curr->ch << ":\t" << code << endl;//����Ϊ�գ���˵������Ҷ�ӣ����һ����¼�������
	if (curr->right != NULL)
	{//�������������Գ�
		code += "1";
		printCode(curr->right, code);
		code.pop_back();
	}
}//�����ϣ��������ĵݹ��㷨�������������ԳƵ�

void huffmanTree::printCode()
{
	string code;
	cout << "The code of every entry as follows" << endl;
	printCode(root, code);//���ô����غ����������ڵݹ�
}

void huffmanTree::initQueue()
{
	size_t n;
	cout << "Enter the number of entry, > 0: ";
	cin >> n;
	cout << "Input the entry and its frequency,repectively" << endl;
	for (size_t i = 0; i != n; ++i)
	{//���Ϲ���ڵ㣬�������
		char ch;
		size_t fre;
		cin >> ch >> fre;
		node *p = new node(ch, fre);
		q.push(p);
	}
}

void huffmanTree::createHuff()
{
	size_t q_size = q.size();//���¶��г���
	for (size_t i = 0; i != q_size - 1; ++i)
	{//n���ڵ㣬���ظ�n - 1�βſɹ������
		node *p = new node;
		node *lchild = q.top();//��õ�ǰ�������(�˴���Ƶ�����Ϊ��׼)�ڵ�
		q.pop();//ɾ��
		node *rchild = q.top();//ͬ��
		q.pop();
		p->left = lchild; p->right = rchild;//ʹ�����ڵ��Ϊ�½ڵ㺢��
		p->frequency = lchild->frequency + rchild->frequency;//Ƶ��Ϊ����֮��
		q.push(p);//�½ڵ�������
	}
	root = q.top();//����ֻʣ��һ���ڵ��ˣ���Ϊ����������
}

void huffmanTree::destroy(node* root)
{
	if (!root) return;
	destroy(root->left);
	destroy(root->right);
	delete root;
}

bool compare(node *pl, node *pr)
{//����node*�ıȽϹ��򣬰�Ƶ�ȱȽϣ����ڹ�����С���ȼ�����
	return pl->frequency > pr->frequency;
}

int main()
{
	bool continue__ = true;
	while (continue__)
	{
		huffmanTree huff(compare);
		huff.initQueue();
		huff.createHuff();
		huff.printCode();
		cin >> continue__;
	}
	return 0;
}