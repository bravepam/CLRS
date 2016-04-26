
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
{//哈弗曼树节点
	char ch;//记录
	size_t frequency;//频度
	node *left;
	node *right;
	node(char c, size_t f) :ch(c), frequency(f), left(NULL), right(NULL){}
	node() :ch('\0'), frequency(0), left(NULL), right(NULL){}
};

class huffmanTree
{//哈夫曼树
public:
	bool typedef (*comp)(node*, node*);//函数指针，用于定义比较函数
	huffmanTree(comp compare, node *r = NULL) :q(compare), root(r){}//构造函数，必须为比较器提供初值
	void initQueue();//初始化节点队列
	void createHuff();//构造哈夫曼树
	void printCode();//打印哈弗曼编码
	~huffmanTree()
	{
		destroy(root);
	}
private:
	node *root;
	priority_queue<node*, vector<node*>, comp> q;//队列，基于vector<node*>，比较器为comp类型
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
	{//若左子树不为空
		code += "0";//往左走编码0
		printCode(curr->left, code);//继续编码左子树
		code.pop_back();//编码完左子树后删除编码串最后一位，因为要往右编码了
	}
	else cout << curr->ch << ":\t" << code << endl;//否则为空，则说明到达叶子，输出一个记录和其编码
	if (curr->right != NULL)
	{//与上述左子树对称
		code += "1";
		printCode(curr->right, code);
		code.pop_back();
	}
}//基本上，二叉树的递归算法都是左右子树对称的

void huffmanTree::printCode()
{
	string code;
	cout << "The code of every entry as follows" << endl;
	printCode(root, code);//调用此重载函数，有助于递归
}

void huffmanTree::initQueue()
{
	size_t n;
	cout << "Enter the number of entry, > 0: ";
	cin >> n;
	cout << "Input the entry and its frequency,repectively" << endl;
	for (size_t i = 0; i != n; ++i)
	{//不断构造节点，插入队列
		char ch;
		size_t fre;
		cin >> ch >> fre;
		node *p = new node(ch, fre);
		q.push(p);
	}
}

void huffmanTree::createHuff()
{
	size_t q_size = q.size();//记下队列长度
	for (size_t i = 0; i != q_size - 1; ++i)
	{//n个节点，得重复n - 1次才可构造完毕
		node *p = new node;
		node *lchild = q.top();//获得当前优先最高(此处以频度最低为标准)节点
		q.pop();//删除
		node *rchild = q.top();//同理
		q.pop();
		p->left = lchild; p->right = rchild;//使该两节点成为新节点孩子
		p->frequency = lchild->frequency + rchild->frequency;//频度为它们之和
		q.push(p);//新节点插入队列
	}
	root = q.top();//队列只剩下一个节点了，即为哈弗曼树根
}

void huffmanTree::destroy(node* root)
{
	if (!root) return;
	destroy(root->left);
	destroy(root->right);
	delete root;
}

bool compare(node *pl, node *pr)
{//定义node*的比较规则，按频度比较，用于构造最小优先级队列
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