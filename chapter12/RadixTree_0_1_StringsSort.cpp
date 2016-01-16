
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<string>

using namespace std;

typedef struct Node
{
	struct Node *left;
	struct Node *right;
	string str;
	Node() :left(NULL), right(NULL){}
}node, RTree;

void insertRadixTree(RTree *rt, const string &str)
{
	node *curr = rt;
	if (curr == NULL) return;
	for (string::size_type i = 0; i != str.size(); ++i)
	{//����ÿһ���ַ���
		if (str[i] == '0')
		{//����ǰ�ַ�Ϊ0����������
			if (curr->left == NULL)
				curr->left = new node();
			curr = curr->left;
		}
		else
		{//Ϊ1��������
			if (curr->right == NULL)
				curr->right = new node();
			curr = curr->right;
		}
		if (i == str.size() - 1)
			curr->str = str;
	}
}

void createRadixTree(RTree *rt)
{
	string str;
	cout << "Enter some 0_1 strings,CTRL+Z to end" << endl;
	while (cin >> str)
		insertRadixTree(rt, str);
	cin.clear();
}

void preTraversal(RTree *rt)
{//�������
	if (rt != NULL)
	{
		if (rt->str != "")
			cout << rt->str << endl;
		preTraversal(rt->left);//�ݹ����������
		preTraversal(rt->right);//�ݹ����������
	}
}
void destroy(node *root)
{
	if (root == NULL) return;
	destroy(root->left);
	destroy(root->right);
	delete root;
}

int main()
{
	RTree *rt = new node();
	createRadixTree(rt);
	preTraversal(rt);
	destroy(rt);
	getchar();
	return 0;
}