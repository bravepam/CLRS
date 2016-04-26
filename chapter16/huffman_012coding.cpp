
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//�����Ʊ��룬����0.1.2���룬��������

#include<iostream>
#include<vector>
#include<queue>
#include<string>

using namespace std;

struct node
{//�ڵ㣬����ָ����
	size_t frequency;
	char ch;
	node *left;
	node *middle;
	node *right;
	node(char c = '\0',size_t f = 0) :frequency(f), ch(c), 
		left(nullptr), middle(nullptr), right(nullptr){}
};

class huffmanTree
{//��������
public:
	bool typedef (*comp)(node*, node*);//����Ƚ������ͣ����ڹ�����С���ȼ�����
	huffmanTree(const comp &com,node *r = nullptr) :Q(com), root(r){}
	void initQueue();//��ʼ������
	void createHuff();//�����������
	void printCode();//��ӡ�ַ�����
	~huffmanTree()
	{
		destroy(root);
	}
private:
	node *root;
	priority_queue<node*, vector<node*>, comp> Q;
	void printCode(node*, string&);//�ݹ��ӡ����
	void destroy(node*);

	huffmanTree& operator=(const huffmanTree&);
	huffmanTree(const huffmanTree&);
};

bool compare(node *lhs, node *rhs)
{//�ȽϺ���
	return lhs->frequency > rhs->frequency;
}

void huffmanTree::initQueue()
{
	size_t n;
	cout << "Enter the number of entry, > 1: ";
	cin >> n;
	cout << "Input the entry and its frequency,repectively" << endl;
	for (size_t i = 0; i != n; ++i)
	{//���Ϲ���ڵ㣬�������
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
	for (size_t i = 0; i != q_size / 2; ++i)//�����Ʊ��룬ֻ��q_size / 2�μ��ɹ������
	{
		node *curr = new node();
		node *lchild = Q.top();
		Q.pop();
		curr->left = lchild; curr->frequency += lchild->frequency;//��Ϊ��ǰ�ڵ�curr�����ӣ�������Ƶ��
		node *mchild = Q.top();
		Q.pop();
		curr->middle = mchild; curr->frequency += mchild->frequency;//��Ϊ��ǰ�ڵ�curr���к��ӣ�������Ƶ��
		if (!Q.empty())
		{//�����в��գ�������ӵ�ǰ�ڵ�curr���Һ��ӣ�������Ƶ�ȡ����ݾ����֪�������м����ڵ㣬ֻ���Һ��ӿ���Ϊ��
			node *rchild = Q.top();
			Q.pop();
			curr->right = rchild; curr->frequency += rchild->frequency;
		}
		else curr->right = nullptr;//����
		Q.push(curr);//�������
	}
	root = Q.top();//��������������ϣ���������
}

void huffmanTree::printCode()
{
	string code;
	cout << "The code of every entry as follows" << endl;
	printCode(root, code);//���ô����غ����������ڵݹ�
}

void huffmanTree::printCode(node *root, string &code = string(""))
{
	node *curr = root;
	if (curr == nullptr) return;
	if (curr->left != nullptr)
	{//����������Ϊ�գ�������������
		code += "0";
		printCode(curr->left, code);
		code.pop_back();//�������������ʱ��Ҫɾ�����������
	}
	else cout << curr->ch << " : " << code << endl;//��Ϊ�գ�˵����Ҷ�ӣ���ӡ����
	if (curr->middle != nullptr)
	{//ͬ��
		code += "1";
		printCode(curr->middle, code);
		code.pop_back();
	}
	if (curr->right != nullptr)
	{//ͬ��
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