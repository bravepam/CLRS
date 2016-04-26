
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//三进制编码，即用0.1.2编码，哈弗曼。

#include<iostream>
#include<vector>
#include<queue>
#include<string>

using namespace std;

struct node
{//节点，三个指针域
	size_t frequency;
	char ch;
	node *left;
	node *middle;
	node *right;
	node(char c = '\0',size_t f = 0) :frequency(f), ch(c), 
		left(nullptr), middle(nullptr), right(nullptr){}
};

class huffmanTree
{//哈弗曼树
public:
	bool typedef (*comp)(node*, node*);//定义比较器类型，用于构造最小优先级队列
	huffmanTree(const comp &com,node *r = nullptr) :Q(com), root(r){}
	void initQueue();//初始化队列
	void createHuff();//构造哈夫曼树
	void printCode();//打印字符编码
	~huffmanTree()
	{
		destroy(root);
	}
private:
	node *root;
	priority_queue<node*, vector<node*>, comp> Q;
	void printCode(node*, string&);//递归打印编码
	void destroy(node*);

	huffmanTree& operator=(const huffmanTree&);
	huffmanTree(const huffmanTree&);
};

bool compare(node *lhs, node *rhs)
{//比较函数
	return lhs->frequency > rhs->frequency;
}

void huffmanTree::initQueue()
{
	size_t n;
	cout << "Enter the number of entry, > 1: ";
	cin >> n;
	cout << "Input the entry and its frequency,repectively" << endl;
	for (size_t i = 0; i != n; ++i)
	{//不断构造节点，插入队列
		char ch;
		size_t fre;
		cin >> ch >> fre;
		node *p = new node(ch, fre);
		Q.push(p);
	}
}

void huffmanTree::createHuff()
{
	size_t q_size = Q.size();
	for (size_t i = 0; i != q_size / 2; ++i)//三进制编码，只需q_size / 2次即可构造完毕
	{
		node *curr = new node();
		node *lchild = Q.top();
		Q.pop();
		curr->left = lchild; curr->frequency += lchild->frequency;//成为当前节点curr的左孩子，并更新频度
		node *mchild = Q.top();
		Q.pop();
		curr->middle = mchild; curr->frequency += mchild->frequency;//成为当前节点curr的中孩子，并更新频度
		if (!Q.empty())
		{//若队列不空，继续添加当前节点curr的右孩子，并更新频度。根据经验可知，不管有几个节点，只有右孩子可能为空
			node *rchild = Q.top();
			Q.pop();
			curr->right = rchild; curr->frequency += rchild->frequency;
		}
		else curr->right = nullptr;//若空
		Q.push(curr);//推入队列
	}
	root = Q.top();//哈夫曼树构造完毕，赋给树根
}

void huffmanTree::printCode()
{
	string code;
	cout << "The code of every entry as follows" << endl;
	printCode(root, code);//调用此重载函数，有助于递归
}

void huffmanTree::printCode(node *root, string &code = string(""))
{
	node *curr = root;
	if (curr == nullptr) return;
	if (curr->left != nullptr)
	{//若左子树不为空，则继续向左编码
		code += "0";
		printCode(curr->left, code);
		code.pop_back();//左子树编码完毕时，要删除左编码数字
	}
	else cout << curr->ch << " : " << code << endl;//若为空，说明是叶子，打印编码
	if (curr->middle != nullptr)
	{//同上
		code += "1";
		printCode(curr->middle, code);
		code.pop_back();
	}
	if (curr->right != nullptr)
	{//同上
		code += "2";
		printCode(curr->right, code);
		code.pop_back();
	}
}

void huffmanTree::destroy(node* root)
{
	if (!root) return;
	destroy(root->left);
	destroy(root->middle);
	destroy(root->right);
	delete root;
}

int main()
{
	bool con = true;
	while (con)
	{
		huffmanTree huff(compare);
		huff.initQueue();
		huff.createHuff();
		huff.printCode();
		cout << "continue ? 1/0 :";
		cin >> con;
	}
	return 0;
}