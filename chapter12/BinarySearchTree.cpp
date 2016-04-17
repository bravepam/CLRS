
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<stack>

using namespace std;
template <typename T> class BSTree;

template <typename T>
class node
{
private:
	friend class BSTree<T>;
	T data;
	node *left;//����
	node *right;//�Һ���
	node *parent;//���ڵ�
public:
	node(const T &d):data(d),left(NULL),right(NULL),parent(NULL){}
	T getData()const {return data;}
	void setData(const T &d) {data = d;}
	/*ʡ��ָ����setter��getter*/
};

template <typename T>
class BSTree
{
private:
	node<T> *root;
	BSTree& operator=(const BSTree&);
	BSTree(const BSTree&);
	template<typename input_iter>
	void rebuildBST(input_iter,input_iter,size_t,node<T>*&);
public:
	BSTree(node<T> *r):root(r){}//���ƹ��캯��������һ���ڵ�ָ���β�
	BSTree():root(NULL){}
	bool empty()const {return root == NULL;}
	void insert(const T &);
	void create();
	node<T>* locate(const T&)const;
	void preTraversal()const;
	void preTraversalNotRecursive()const;
	void inTraversal()const;
	void inTraversalNotRecursive()const;
	void postTraversal()const;
	void postTraversalNotRecursive()const;
	void erase(const T &);
	node<T>* minMum()const;
	node<T>* maxMum()const;
	node<T>* successor(const T &)const;//�Һ��
	node<T>* predecessor(const T &)const;//��ǰ��
	void destroy();
	

	template<typename input_iter>
	void rebuildBST(input_iter preorder,input_iter inorder,size_t tree_size)//����ǰ��������������ؽ�������
	{
		rebuildBST(preorder,inorder,tree_size,root);
	}
	bool traversalAtLevel(size_t)const;//��α���ĳһ��Ľڵ㣬��������
	void levelTraversal_h()const;//��α����ⷨ1
	void levelTraversal_b()const;//��α����ⷨ2
	void levelTraversal_q()const;//��α����ⷨ3
	size_t height()const;//���߶�
};

template <typename T>
void BSTree<T>::insert(const T &d)
{//���룬�ǵݹ�
	node<T> *p = NULL,*curr = root;
	while(curr != NULL)
	{//�ҵ�����λ��
		p = curr;
		if(d <= curr->data)
			curr = curr->left;
		else curr = curr->right;
	}
	curr = new node<T>(d);
	if(p == NULL)//����Ϊ��
		root = curr;
	else if(d <= p->data) p->left = curr;
		else p->right = curr;
	curr->parent = p;
	curr->left = curr->right = NULL;
}


/*template <typename T>
void BSTree<T>::insert(const T &d)
{//�������ݵݹ�汾����覴ã����˵�һ���ڵ㣬�޷����ӵ��Ժ󴴽��Ľڵ㣬���������޷����ӵ�root�ĺ�����
	node<T> *p = NULL;
	if(root == NULL)
	{
		root = new node<T>(d);
		root->left = root->right = NULL;
		root->parent = p;
	}
	else if(d <= root->data)
	{
		BSTree LEFT(root->left);
		LEFT.insert(d);
	}
	else
	{
		BSTree RIGHT(root->left);
		RIGHT.insert(d);
	}
}
*/

template <typename T>
void BSTree<T>::create()
{
	T data;
	cout << "Enter the value(s),CTRL+Z to end" << endl;
	while(cin >> data)
		insert(data);
	cin.clear();
}

template <typename T>
void BSTree<T>::preTraversal()const
{
	node<T> *curr = root;
	if(curr != NULL)
	{
		cout << curr->data << ' ';
		BSTree LEFT(curr->left);//������������һ��BSTree���󣬼����ݹ�
		LEFT.preTraversal();
		BSTree RIGHT(curr->right);
		RIGHT.preTraversal();
	}
}

template <typename T>
void BSTree<T>::preTraversalNotRecursive()const
{
	if (!root) return;
	stack<node<T>*> S;
	node<T> *curr = nullptr;
	S.push(root);
	while (!S.empty())
	{
		curr = S.top();
		S.pop();
		cout << curr->data << ' ';
		if (curr->right)
			S.push(curr->right);
		if (curr->left)
			S.push(curr->left);
	}
	cout << endl;
}

template <typename T>
void BSTree<T>::inTraversal()const
{
	node<T> *curr = root;
	if(curr != NULL)
	{
		BSTree LEFT(curr->left);
		LEFT.inTraversal();
		cout << curr->data << ' ';
		BSTree RIGHT(curr->right);//������������һ��BSTree���󣬼����ݹ�
		RIGHT.inTraversal();
	}
}

template <typename T>
void BSTree<T>::inTraversalNotRecursive()const
{
	if (!root) return;
	node<T> *curr = root;
	stack<node<T>*> S;
	while (curr)
	{
		S.push(curr);
		curr = curr->left;
	}
	while (!S.empty())
	{
		curr = S.top();
		cout << curr->data << ' ';
		S.pop();
		if (curr->right)
		{
			curr = curr->right;
			while (curr)
			{
				S.push(curr);
				curr = curr->left;
			}
		}
	}
}

template <typename T>
void BSTree<T>::postTraversal()const
{
	node<T> *curr = root;
	if (curr)
	{
		BSTree<T> LEFT(curr->left);
		LEFT.postTraversal();
		BSTree<T> RIGHT(curr->right);
		RIGHT.postTraversal();
		cout << curr->data << ' ';
	}
}

template <typename T>
void BSTree<T>::postTraversalNotRecursive()const
{
	if (!root) return;
	node<T> *curr = nullptr, *prev = nullptr;
	stack<node<T>*> S;
	S.push(root);
	while (!S.empty())
	{
		curr = S.top();
		if ((!curr->right && !curr->left) || (curr->left && curr->left == prev)
			|| (curr->right && curr->right == prev))
		{
			cout << curr->data << ' ';
			S.pop();
			prev = curr;
		}
		else
		{
			if (curr->right)
				S.push(curr->right);
			if (curr->left)
				S.push(curr->left);
		}
	}
}

/*template <typename T>
node<T>* BSTree<T>::locate(const T &d)const
{//���ң��ǵݹ�
	node<T> *curr = root;
	while(curr != NULL && curr->data != d)
	{
		if(curr->data > d)
			curr = curr->left;
		else curr = curr->right;
	}
	return curr;
}*/

template <typename T>
node<T>* BSTree<T>::locate(const T &d)const
{//���ҵݹ�汾
	node<T> *curr = root;
	if(curr == NULL || curr->data == d)
		return curr;
	else if(curr->data > d)
	{
		BSTree LEFT(curr->left);
		return LEFT.locate(d);
	}
	else
	{
		BSTree RIGHT(curr->right);
		return RIGHT.locate(d);
	}
}


/*template <typename T>
node<T>* BSTree<T>::minMum()const
{//����Сֵ���ǵݹ�
	if(root == NULL) return root;
	node<T> *curr = root;
	while(curr->left != NULL)
		curr = curr->left;
	return curr;
}
*/
template <typename T>
node<T>* BSTree<T>::minMum()const
{//����Сֵ�ݹ�汾
	if(root == NULL) return root;
	node<T> *curr = root;
	if(curr->left == NULL) return curr;
	BSTree LEFT(curr->left);
	return LEFT.minMum();
}


/*template <typename T>
node<T>* BSTree<T>::maxMum()const
{//�����ֵ���ǵݹ�
	if(root == NULL) return root;
	node<T> *curr = root;
	while(curr->right != NULL)
		curr = curr->right;
	return curr;
}
*/
template <typename T>
node<T>* BSTree<T>::maxMum()const
{//�����ֵ�ݹ�汾
	if(root == NULL) return root;
	node<T> *curr = root;
	if(curr->right == NULL) return curr;
	BSTree RIGHT(curr->right);
	return RIGHT.maxMum();
}


template <typename T>
node<T>* BSTree<T>::successor(const T &d)const
{//�Һ��
	node<T> *p = locate(d);
	if(p->right != NULL)
	{//����������Ϊ�գ�����Ϊ����������Сֵ
		BSTree RIGHT(p->right);
		return RIGHT.minMum();
	}
	node<T> *par = p->parent;
	while(par != NULL && par->right == p)
	{//��Ϊ�գ�����������ָ�뷴����ϵ�һ���յ㴦
		p = par;
		par = p->parent;
	}
	return par;
}

template <typename T>
node<T>* BSTree<T>::predecessor(const T &d)const
{
	node<T> *p = locate(d);
	if(p->left != NULL)
	{//����������λ�գ���ǰ��Ϊ���������ֵ
		BSTree LEFT(p->left);
		return LEFT.maxMum();
	}
	node<T> *par = p->parent;
	while(par != NULL && par->left == p)
	{//��Ϊ�գ���ǰ��������ָ�뷴����ϵ�һ���յ㴦
		p = par;
		par = p->parent;
	}
	return par;
}

template <typename T>
void BSTree<T>::erase(const T &d)
{//ɾ��һ���ڵ�
	node<T> *p = locate(d),*next,*child;
	if(p->left == NULL || p->right == NULL)
		next = p;//ȷ��ɾ���ڵ㣬���������һ����Ů
	else next = successor(d);//����Ҳ���Ը�Ϊɾ��ǰ����predecessor���������ñ䡣
	if(next->left != NULL)
		child = next->left;//ȡ��̽ڵ����Ů�����Դ����̵�λ�ã���̽ڵ������һ����Ů
	else child = next->right;
	if(child != NULL)//��̽ڵ�����Ů
		child->parent = next->parent;
	if(next->parent == NULL)//��Ҫɾ���Ľڵ�Ϊ���ڵ�
		root = child;
	else if(next == next->parent->left)//�����Ϊ�丸�ڵ������
		next->parent->left = child;
	else next->parent->right = child;//��Ϊ�Һ���
	if(next != p)
		p->data = next->data;
	delete next;//�ͷź�̽ڵ�
}

template <typename T>
void BSTree<T>::destroy()
{//���ٶ�����
	if(root == NULL) return;
	if(root->left != NULL)
	{
		BSTree<T> LEFT(root->left);
		LEFT.destroy();
	}
	if(root->right != NULL)
	{
		BSTree<T> RIGHT(root->right);
		RIGHT.destroy();
	}
	delete root;
}

//-----------------------------------�������---------------------------------------------------
template <typename T> template<typename input_iter>
void BSTree<T>::rebuildBST(input_iter preorder,input_iter inorder,size_t tree_size,node<T> *&root)
{//�ؽ�������
	node<T> *p = new node<T>(*preorder);//������
	if(root == NULL) root = p;//����Ϊ�գ���pΪ����
	if(tree_size == 1) return ;//��������ģΪ1ʱ������
	input_iter right_in_start = inorder,left_pre_start = preorder + 1;
	size_t left_tree_size = 0;
	while(*right_in_start++ != *preorder)//����������������ʼλ��
		++left_tree_size;//������������ģ
	input_iter right_pre_start = left_pre_start + left_tree_size;//������ǰ����ʼλ��
	size_t right_tree_size = tree_size - left_tree_size - 1;//��������ģ
	if(left_tree_size > 0) rebuildBST(left_pre_start,inorder,left_tree_size,p->left);//�ݹ��ؽ�������
	if(right_tree_size > 0) rebuildBST(right_pre_start,right_in_start,right_tree_size,p->right);//�ݹ��ؽ�������
	if(p->left != NULL) p->left->parent = p;//���ø��Ը�ָ��
	if(p->right != NULL) p->right->parent = p;
}

template <typename T>
bool BSTree<T>::traversalAtLevel(size_t level)const
{//���������level�������
	if(root == NULL) return false;
	if(level == 0)
	{//����Ѿ��ݹ鵽��ǰ�㣬���
		cout << root->data << ' ';
		return true;
	}
	BSTree<T> LEFT(root->left),RIGHT(root->right);//����������µݹ�
	bool left_status = LEFT.traversalAtLevel(level - 1),
		right_status = RIGHT.traversalAtLevel(level - 1);
	return left_status || right_status;//���صݹ�ִ�е�״̬
}

template <typename T>
size_t BSTree<T>::height()const
{//������
	if (root == NULL) return 0;
	size_t left_height = 0, right_height = 0;
	if (root->left != NULL)
	{
		BSTree<T> LEFT(root->left);
		left_height = LEFT.height();
	}
	if (root->right != NULL)
	{
		BSTree<T> RIGHT(root->right);
		right_height = RIGHT.height();
	}
	return left_height > right_height ? left_height + 1 : right_height + 1;
}

template <typename T>
void BSTree<T>::levelTraversal_h()const
{//�������Ĳ�α������ⷨ1
	size_t h = height();//�������ߣ�������
	for (size_t i = 0; i <= h; ++i)
	{
		traversalAtLevel(i);//Ȼ�������ÿһ�������
		cout << endl;
	}
}

template <typename T>
void BSTree<T>::levelTraversal_b()const
{//��α����ⷨ2����Ҫ������ȷ����
	for (size_t level = 0;; ++level)
	{
		if (!traversalAtLevel(level)) break;//���ݵ�ǰ��ķ��ؽ���жϣ��Ƿ��Ѿ�������
		cout << endl;
	}
	return;
}

#include<vector>

template<typename T>
void BSTree<T>::levelTraversal_q()const
{//��α����ⷨ3
	if (root == NULL) return;
	cout << root->data << endl;
	vector<node<T>*> pvec;//����һ�����飨���߶��У��洢�ѷ��ʹ��Ľڵ�
	pvec.push_back(root);
	size_t curr = 0, last = 1;
	while (curr != pvec.size())
	{//ֻҪ�����л�����Ԫ��
		size_t num = 0;//��¼����ӵ�Ԫ����
		for (; curr != last;++curr)
		{//������ǰ���Ԫ��
			if (pvec[curr]->left != NULL)
			{//����
				cout << pvec[curr]->left->data << ' ';
				pvec.push_back(pvec[curr]->left);
				++num;
			}
			if (pvec[curr]->right != NULL)
			{//�Һ���
				cout << pvec[curr]->right->data << ' ';
				pvec.push_back(pvec[curr]->right);
				++num;
			}
		}
		cout << endl;
		last += num;
	}
}

int main()
{//12 5 18 2 9 15 19 17
	BSTree<int> btree;
	cout << "btree.create()" << endl;
	btree.create();
	cout << "btree.inTraversal()" << endl;
	btree.inTraversal();
	/*cout <<  endl << btree.locate(19)->getData();*/
	cout << endl << "btree.preTraversal()" << endl;
	btree.postTraversal();
	cout << endl;
	btree.postTraversalNotRecursive();
	cout << endl;
	btree.levelTraversal_q();
	btree.destroy();
	getchar();
	return 0;
}
//#include<string>
//
//int main()
//{
//	string pre = "abcde",in = "edcba";
//	BSTree<char> bt;
//	bt.rebuildBST(pre.begin(),in.begin(),pre.size());
//	bt.preTraversal();
//	cout << endl;
//	bt.inTraversal();
//	bt.destroy();
//	getchar();
//	return 0;
//}