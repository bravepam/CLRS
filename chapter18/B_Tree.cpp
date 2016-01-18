
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//B��

#include<iostream>
#include<vector>

using namespace std;

template < typename T, int degree = 3, typename Compare = less<T> >
struct node
{//B���ڵ�����,degreeΪ�ȣ�Ĭ��Ϊ3
	static const int min_num = degree - 1;//ÿ���ڵ����С�ؼ�����
	static const int max_num = 2 * degree - 1;//Ҷ�ӽڵ����ؼ��������ڽڵ�Ҫ��1
	static Compare compare;
	int num = 0;//�ýڵ�ؼ�����Ŀ
	size_t height = 0;//�Ըýڵ�Ϊ��������
	bool leaf = true;//�Ƿ�ΪҶ�ӽڵ�
	T key[max_num];//�ؼ�������
	node *child[max_num + 1];//���ӽڵ�����

	static void setCompare(const Compare &c){ compare = c; }
	node()
	{//Ĭ�Ϲ��캯��
		for (int i = 0; i != max_num; ++i)
		{
			key[i] = T();
			child[i] = nullptr;
		}
		child[max_num] = nullptr;
	}
	int search(const T&)const;
	void insert(const T&);
	bool erase(int);
};

//���徲̬����
template < typename T, int degree, typename Compare> const int node<T, degree, Compare>::max_num;
template < typename T, int degree, typename Compare> const int node<T, degree, Compare>::min_num;
template < typename T, int degree, typename Compare> Compare node<T, degree, Compare>::compare;

template < typename T, int degree = 3, typename Compare = less<T> >
int node<T, degree,Compare>::search(const T &k)const
{//�ڵ��ڹؼ��ֲ���
	int low = 0, high = num - 1;
	while (low < high)
	{//���ֲ���
		int mid = (low + high) / 2;
		if (!compare(k,key[mid]) && !compare(key[mid],k)) return mid;
		else if (compare(k,key[mid])) high = mid - 1;
		else low = mid + 1;
	}
	if (compare(key[low], k) && low < num - 1) ++low;
	return low;//���ص�һ����С��k�Ĺؼ��ֵ����������ȫ������kС����low�����һ���ؼ�������
}

template < typename T, int degree = 3, typename Compare = less<T> >
void node<T, degree, Compare>::insert(const T &k)
{//�ڵ��ڲ���
	int i = num - 1;
	while (i >= 0 && compare(k,key[i]))
	{//��Ѱ����λ��
		key[i + 1] = key[i];
		--i;
	}
	key[++i] = k;//����
	++num;
}

template <typename T,int degree = 3,class Compare>
bool node<T, degree, Compare>::erase(int index)
{//�ڵ���ɾ��
	for (int i = index + 1; i != num; ++i)
		key[i - 1] = key[i];
	--num;
	return true;
}

template < typename T, int degree = 3, typename Compare = less<T> >
class Btree
{//B��
public:
	typedef node<T, degree, Compare>					node;
	typedef Compare										comp;
private:
	node *root;
	Compare compare;
	void destroy(node*);//������
	node* split(node*, int);//�ָ�ڵ�
	node* underfillSplit(node*, int);//δ�����ѣ�������ר��
	void insert_aux(node*, const T&);//���븨��
	bool erase_aux(node*, const T&);//ɾ������
	void merge(node*, int);//�ϲ��ڵ�
	T erasePredecessor(node*);//ɾ��ǰ��
	T eraseSuccessor(node*);//ɾ�����
	void borrowFromRightSibling(node*, int);//�����ֵܽ�ؼ���
	void borrowFromLeftSibling(node*, int);//�����ֵܽ�ؼ���
	void print(node*)const;//���ؼ��ַǵݼ�˳���ӡ
	
	node* nodeAtRightOfHeight(size_t);
	node* nodeAtLeftOfHeight(size_t);
	void linkAtRight(const T&, Btree&);
	void linkAtLeft(const T&, Btree&);
	void linkTwoTrees(Btree &lhs, const T &link_key, Btree &rhs)
	{	
		if (lhs.empty())
		{
			lhs.root = rhs.root;
			rhs.root = nullptr;
		}
		else lhs.link(link_key, rhs);
	}
	void rootFull()
	{
		node *p = root;
		root = new node;//�����᳤��
		root->child[0] = p;
		root->height = p->height + 1;
		root->leaf = false;
		split(root, 0);//��������
	}
	explicit Btree(node *r) :root(r), compare(r->compare){}//ֻ�ڷ��Ѻ����б����ã�˽��
public:
	Btree() :root(nullptr), compare(Compare()){ node::setCompare(compare); }
	Btree(const Compare &c) :root(nullptr), compare(c){ node::setCompare(c); }
	//size_t getHeight()const { return root->height; }
	bool empty()const { return root == nullptr; }
	void insert(const T&);
	bool search(const T&)const;
	void link(const T&, Btree&);//������
	void splitTree(const T&,Btree&,Btree&);//������
	void sequentialPrint()const { print(root); }
	void clear()
	{
		destroy(root); 
		root = nullptr;
	}
	bool erase(const T &k) { return erase_aux(root, k); }
	~Btree(){ destroy(root); }
};

template <typename T,int degree,class Compare>
bool Btree<T,degree, Compare>::search(const T &k)const
{//��B���в���k
	node *curr = root;
	while (curr != nullptr)
	{
		int index = curr->search(k);//�ڵ�ǰ�ڵ����
		if (!compare(k, curr->key[index]) && !compare(curr->key[index], k)) return true;//������
		else if (compare(k,curr->key[index]))//��kС��index���Ĺؼ�����������߲���
			curr = curr->child[index];
		else curr = curr->child[index + 1];//�������ұ߲���
	}
	return false;
}

template <typename T, int degree, class Compare>
node<T,degree,Compare>* Btree<T, degree, Compare>::split(node *curr, int index)
{//��curr��ָ�ڵ��index�����ӽڵ�ָ�������ڵ㣬���߸�degree - 1���ؼ��֣�index���ؼ���������curr��
	node *new_child = new node,*old_child = curr->child[index];
	T k = old_child->key[degree - 1];
	for (int first = 0; first != degree - 1; ++first)//��ԭ�ڵ�ĺ�һ��ؼ��ָ��Ƶ��½ڵ�
		new_child->key[first] = old_child->key[first + degree];
	if (!old_child->leaf)//�������Ҷ��
		for (int first = 0; first != degree; ++first)//��Ҫ����һ��ĺ��ӽڵ�ָ��
			new_child->child[first] = old_child->child[first + degree];
	new_child->leaf = old_child->leaf;
	new_child->height = old_child->height;
	new_child->num = degree - 1;//�½ڵ�ؼ�����
	old_child->num -= degree;//ԭ�ڵ�ؼ���������
	for (int last = curr->num - 1; last >= index; --last)//��curr��index��������֮��Ĺؼ���ȫ������һ��λ��
		curr->key[last + 1] = curr->key[last];
	for (int last = curr->num; last > index; --last)//��curr��index����������֮��ĺ���ָ��ȫ������һ��λ��
		curr->child[last + 1] = curr->child[last];
	curr->key[index] = k;//��curr�е�index�������������Ĺؼ���
	curr->child[index + 1] = new_child;//index��ĺ���ָ��ָ���½ڵ�
	++curr->num;
	return new_child;
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::insert_aux(node *curr, const T &k)
{//���븨������
	if (curr->leaf) curr->insert(k);//����ǰ�ڵ���Ҷ�ӣ���ֱ�Ӳ���
	else
	{//����
		int index = curr->search(k);//�ҵ���һ����С��k�Ĺؼ�������
		if (compare(curr->key[index], k)) ++index;//����������ýڵ����йؼ��־�С��k
		if ((curr->child[index])->num == node::max_num)
		{//�����������ĺ��ӽڵ�ؼ���������
			split(curr, index);//����Ӹô��ָ�
			if (compare(curr->key[index], k))//�ָ�����������Ĺؼ�����С��k
				++index;//��Ҫ�������ɵĽڵ��м�������
		}
		insert_aux(curr->child[index], k);//�ݹ����
	}
}

template <typename T, int degree, class Compare>
void Btree<T, degree, Compare>::insert(const T &k)
{//����ؼ���
	if (root == nullptr)
	{//����ڿ����в����һ���ؼ���
		root = new node;
		root->insert(k);
		return;
	}
	else if (root->num == node::max_num)//����������ڵ���
		rootFull();
	insert_aux(root, k);
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::merge(node *curr, int index)
{//�ϲ�curr��index���ĺ��ӽڵ�������ֵܣ���ʱ���߾�ǡ��ֻ��degree - 1���ؼ���
	node *left = curr->child[index], *right = curr->child[index + 1];
	left->key[degree - 1] = curr->key[index];//curr��index���ؼ������½�
	for (int i = 0; i != right->num; ++i)//����right���йؼ��ֹ���
		left->key[degree + i] = right->key[i];
	for (int i = index + 1; i != curr->num; ++i)//��curr��index֮��Ĺؼ���ǰ��1
		curr->key[i - 1] = curr->key[i];
	if (!left->leaf)//�������Ҷ��
		for (int i = 0; i <= right->num; ++i)//��Ҫ�ƶ�right�ĺ���ָ��
			left->child[degree + i] = right->child[i];
	for (int i = index + 2; i <= curr->num; ++i)//�ƶ�curr��index+2��������ָ��ǰ��1��index�����ر�����
		curr->child[i - 1] = curr->child[i];
	--curr->num; ++left->num;
	left->num += right->num;
	delete right;
}

template <typename T,int degree,class Compare>
T Btree<T, degree, Compare>::erasePredecessor(node *curr)
{//ɾ��ǰ���������2.a�б�����
	if (curr->leaf)
	{//����Ҷ�ӣ���˵���Ѿ�����ɾ�����һ��Ԫ�أ���ǰ����
		T tmp = curr->key[curr->num - 1];
		--curr->num;
		return tmp;//����ǰ��
	}
	//�������ڽڵ㣬�����ݹ����£�
	else if (curr->child[curr->num]->num == node::min_num)
	{//�����һ�����ӹؼ�����Ŀ�Ѵ���Сֵ
		if (curr->child[curr->num - 1]->num > node::min_num)//�����ֵ��ж���ؼ���
			borrowFromLeftSibling(curr, curr->num);//��ô��һ��
		else merge(curr, curr->num - 1);//����ֻ�кϲ���
	}
	return erasePredecessor(curr->child[curr->num]);//�������µݹ�
}

template <typename T,int degree,class Compare>
T Btree<T, degree, Compare>::eraseSuccessor(node *curr)
{//ɾ����̣������2.b�б�����
	if (curr->leaf)
	{//����Ҷ�ӽڵ㣬��ֱ��ɾ����ǰ��Ԫ�أ������
		T tmp = curr->key[0];
		curr->erase(0);
		return tmp;
	}
	//�������ڽڵ㣬�������£�
	else if (curr->child[0]->num == node::min_num)
	{//����һ�����ӹؼ�����Ŀ�Ѵ���Сֵ
		if (curr->child[1]->num > node::min_num)//�����ֵ����㹻�ؼ���
			borrowFromRightSibling(curr, 0);//���һ��
		else merge(curr, 0);//����ֻ�кϲ���
	}
	return eraseSuccessor(curr->child[0]);//�������µݹ�
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::borrowFromRightSibling(node *curr, int index)
{//curr��index�����������ֵܽ�һ���ؼ���
	node *left = curr->child[index], *right = curr->child[index + 1];
	left->key[left->num] = curr->key[index];//�Ƚ�curr��index���Ĺؼ����������������left��ָ
	curr->key[index] = right->key[0];//�������ֵܣ�right��ָ����һ���ؼ��ָ���curr��index��λ
	for (int i = 1; i != right->num; ++i)//�����ֵܴ�1��ʼ�����йؼ���ǰ��1
		right->key[i - 1] = right->key[i];
	if (!left->leaf)
	{//������Ҷ�ӣ���Ҫ������غ���ָ����
		left->child[left->num + 1] = right->child[0];//���ֵܵ�һ�����ӳ�Ϊleft���һ������
		for (int i = 1; i <= right->num; ++i)//ǰ�����ֵܵĺ���ָ������
			right->child[i - 1] = right->child[i];
	}
	++left->num; --right->num;
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::borrowFromLeftSibling(node *curr, int index)
{
	--index;//�Ƶ����ֵܲ�λ
	node *left = curr->child[index], *right = curr->child[index + 1];
	right->insert(curr->key[index]);
	curr->key[index] = left->key[left->num - 1];
	if (!right->leaf)
	{//��Ҷ�ӣ��ƶ�ָ��
		for (int i = right->num; i >= 0; --i)
			right->child[i + 1] = right->child[i];
		right->child[0] = left->child[left->num];
	}
	--left->num;
}

template <typename T,int degree,class Compare>
bool Btree<T, degree, Compare>::erase_aux(node *curr, const T &k)
{//ɾ����������
	int index = curr->search(k);//�ҵ���һ����С��k�Ĺؼ���
	if (curr->leaf && (!compare(curr->key[index], k) && !compare(k, curr->key[index])))
		return curr->erase(index);//���1���ؼ�����Ҷ��
	else if (curr->leaf) return false;//����Ҷ�ӽڵ㣬��ɾ��ʧ�ܣ������ڸùؼ���
	if (!curr->leaf && (!compare(curr->key[index], k) && !compare(k, curr->key[index])))
	{//���2���ؼ����ڸ��ڽڵ㡣��ʹ�øùؼ��ֵ�ǰ�����ߺ�̴���
		if (curr->child[index]->num > node::min_num)
		{//���2.a�����������㹻�Ĺؼ��֣�������degree - 1������ʹ��ǰ�����档
			//ɾ����ǰ����������ǰ���ؼ��֣��Ը��Ǹùؼ���
			curr->key[index] = erasePredecessor(curr->child[index]);
			return true;
		}
		else if (curr->child[index + 1]->num > node::min_num)
		{//���2.b���������Һ������㹻�ؼ��֣���ʹ�ú�̴���
			curr->key[index] = eraseSuccessor(curr->child[index + 1]);//ͬ��
			return true;
		}
		else
		{//�������ڸùؼ������Һ��ӵĹؼ��������������٣����ܲ���ǰ�����ߺ�̴��棬��ô�ϲ����Һ����Լ��ùؼ���
			merge(curr, index);//��curr�ڵ��index����عؼ��ֺͺ��Ӻϲ�
			return erase_aux(curr->child[index], k);
		}
	}
	else
	{//���3���ؼ��ֲ��ڸýڵ㣬���ڸùؼ��ֵ���������
		if (compare(curr->key[index], k)) ++index;//�����������curr�����йؼ��־���kСʱ����
		if (curr->child[index]->num == node::min_num)
		{//���������ؼ������ѵ�����Сֵ
			if (index < curr->num && curr->child[index + 1]->num > node::min_num)
				//���3.a���������ֵܣ������㹻�ڵ㣬������degree - 1�����������һ��
				borrowFromRightSibling(curr, index);
			else if (index > 0 && curr->child[index - 1]->num > node::min_num)
				//���3.b,�������ֵ��ҹؼ������㹻��������3.a
				borrowFromLeftSibling(curr, index);
			else
			{//3.c����/���ֵܾ�ֻ��degree - 1���ؼ��֣���ô�ϲ��ڵ�
				if (index == curr->num) --index;//��Ӧ������������µĽڵ�ϲ�
				merge(curr, index);
				if (curr == root && curr->num == 0)
				{//����ǰcurr�Ǹ����ҽ���һ��Ԫ��
					root = curr->child[index];
					delete curr;//��ô���߽�1
					return erase_aux(root, k);
				}
			}
		}
		return erase_aux(curr->child[index], k);
	}
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::print(node *curr)const
{//��ӡ������
	if (curr == nullptr) return;
	if (curr->leaf)
	{//����Ҷ�ӽڵ㣬���ӡȫ���ؼ���
		cout << "[ ";
		for (int i = 0; i != curr->num; ++i)
			cout << curr->key[i] << ' ';
		cout << "]\theight: " << curr->height << endl;
	}
	else
	{//����
		cout << '{' << endl;
		for (int i = 0; i <= curr->num; ++i)
		{//���δ�ӡ���Ӻ͹ؼ���
			print(curr->child[i]);
			if (i < curr->num)
				cout << curr->key[i] << endl;
		}
		cout << "}\theight: " << curr->height << endl;
	}
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::destroy(node *curr)
{//����B��
	if (curr == nullptr) return;
	if (curr->leaf) delete curr;//����Ҷ�ӣ�ֱ������
	else
	{//����
		for (int i = 0; i <= curr->num; ++i)//�����������к��Ӻ�
			destroy(curr->child[i]);
		delete curr;//�����ٸýڵ�
	}
}

template <typename T,int degree,class Compare = less<T>>
node<T, degree, Compare>* Btree<T, degree, Compare>::nodeAtRightOfHeight(size_t h)
{//�ҵ������߶ȵ����Ҳ�ڵ�ĸ��׻��ߵ�����Ϊhʱ���������������Һϲ���׼�����½�Ѱ��ʱ�������Ľڵ�Ҫ���Է���
	if (root->num == node::max_num)//������ڵ���
		rootFull();
	node *curr = root;
	if (curr->height == h) return curr;//�������Ǹýڵ�
	while (curr->child[curr->num]->height != h)
	{//һֱ����������
		if (curr->child[curr->num]->num == node::max_num)//�����Һ�����
			curr = split(curr, curr->num);//����ѣ��޸���һ��split������ʹ�䷵���º��ӵ�ַ
		else
			curr = curr->child[curr->num];
	}
	return curr;
}

template <typename T, int degree, class Compare = less<T>>
node<T, degree, Compare>* Btree<T, degree, Compare>::nodeAtLeftOfHeight(size_t h)
{//�ҵ������߶ȵ������ڵ�ĸ��ף�������ϲ���׼�����½�Ѱ��ʱ�������Ľڵ�Ҫ���Է���
	if (root->num == node::max_num)//������ڵ���
		rootFull();
	node *curr = root;
	while (curr->child[0]->height != h)
	{//һֱ����������
		if (curr->child[0]->num == node::max_num)//�����Һ�����
			split(curr, 0);//�����
		curr = curr->child[0];
	}
	return curr;
}

template <typename T,int degree,class Compare = less<T>>
void Btree<T, degree, Compare>::linkAtRight(const T &k, Btree &rhs)
{
	node *curr = nodeAtRightOfHeight(rhs.root->height);
	if (curr == root && curr->height == rhs.root->height)
	{//������������һ����
		root = new node;
		root->insert(k);
		root->child[0] = curr;
		root->child[1] = rhs.root;
		root->height = curr->height + 1;
		root->leaf = false;
	}
	else
	{//����ֱ�Ӱѹؼ�k����curr��Ȼ�����Һ���ָ��ָ�򱻺ϲ�������Ϊһ·����������curr��������
		curr->insert(k);
		curr->child[curr->num] = rhs.root;
	}
}

template <typename T, int degree, class Compare = less<T>>
void Btree<T, degree, Compare>::linkAtLeft(const T &k, Btree &lhs)
{
	node *curr = nodeAtLeftOfHeight(lhs.root->height);
	curr->insert(k);
	for (int i = curr->num - 1; i >= 0; --i)
		curr->child[i + 1] = curr->child[i];
	curr->child[0] = lhs.root;
}

template <typename T,int degree,class Compare = less<T>>
void Btree<T, degree, Compare>::link(const T &k, Btree &linkedTree)
{//����ת���������������������ת����ǰ��������������
	if (compare(this->root->key[0], k) && compare(k,linkedTree.root->key[0]))
	{//1������key[this] < k < key[linkedTree]��������ø���0�Źؼ���ֻ������һ���Ǻ������ӣ�
		//���Ǽ�������Ĺؼ��ֺ�������������ϵ����ͬ
		if (this->root->height >= linkedTree.root->height)//1.1 �����ϸ߻��ߺͱ�������һ����
			linkAtRight(k, linkedTree);//���ڱ����Ҳ�����
		else
		{//1.2 �������ϰ�
			swap(root, linkedTree.root);//��������
			linkAtLeft(k, linkedTree);//���µı����������
		}
	}
	else if (compare(linkedTree.root->key[0], k) && compare(k, this->root->key[0]))
	{//2��key[this] > k > key[linkedTree]
		if (linkedTree.root->height < this->root->height)//2.1 ��������
			linkAtLeft(k, linkedTree);//���ڱ����������
		else
		{//2.2 �������ϰ����ߺͱ�������һ����
			swap(root, linkedTree.root);//�򽻻�����
			linkAtRight(k, linkedTree);//���µı����Ҳ�����
		}
	}
	else
	{
		cout << "Error: bad input!" << endl;
		return;
	}
	linkedTree.root = nullptr;
}

template <typename T,int degree,class Compare = less<T>>
node<T, degree, Compare>* Btree<T, degree, Compare>::underfillSplit(node *curr, int index)
{//δ�����ѣ���curr�ڵ��index��һ��Ϊ��
	node *new_child = new node;
	for (int i = index + 1; i < curr->num; ++i)//�ƶ�index֮��Ĺؼ����½ڵ�
		new_child->key[i - index - 1] = curr->key[i];
	if (!curr->leaf)
	{//������Ҷ��
		for (int i = index + 1; i <= curr->num; ++i)//��Ҫ�ƶ�����ָ��
			new_child->child[i - index - 1] = curr->child[i];
	}
	new_child->num = curr->num - index - 1;
	new_child->leaf = curr->leaf;
	new_child->height = curr->height;
	curr->num = curr->num - new_child->num - 1;
	return new_child;
}

template <typename T, int degree, class Compare = less<T> >
void Btree<T,degree,Compare>::splitTree(const T &k, Btree &smallTree,Btree &bigTree)
{//����Ѱ�ؼ���k��·��p�ָ�������С��k�ļ��Ϻϲ�ΪsmallTree����ĺϲ�ΪbigTree�����Ǽ���k����
	node *curr = root; 	root = nullptr;
	T small_link_key = T(), big_link_key = T();
	while (true)
	{//index��curr�е�һ����С��k�Ĺؼ�������������curr���йؼ��ֱ�k��Сʱ���һ���ؼ�������
		//���Ⲣ��Ӱ�����
		int index = curr->search(k);
		T temp = curr->key[index];
		node *new_node = underfillSplit(curr, index);//���Ѹýڵ㣬���������ɵĽڵ��ַ
		if (new_node->num == 0)
		{//���½ڵ�û�йؼ���
			node *r = new_node;
			new_node = new_node->child[0];
			delete r;
		}
		if (curr->num == 0)
		{//�����Ѻ�ǰ�ڵ㲻���йؼ���
			node *r = curr;
			curr = curr->child[0];
			delete r;
		}
		if (compare(k, temp))
		{//��kС��index���ؼ��֣���Ҫ�����ߣ��������ǲ��ٸ���curr�������ڸõ����ѭ��
			linkTwoTrees(bigTree, big_link_key, Btree(new_node));
			big_link_key = temp;//������һ�εķָ�ؼ��֣��Ա��´�����
		}
		else if (compare(temp, k))
		{//������k����index���ؼ��֣���Ҫ�����ߣ�����������������ýڵ�ؼ���ȫ����kСʱ����
			linkTwoTrees(smallTree, small_link_key, Btree(curr));
			small_link_key = temp;
			curr = new_node;//����curr
		}
		else
		{//����ȣ����Ѿ��ָ���ϣ���ô�ϲ���������������
			if (curr != nullptr)//���curr��Ҷ�ӣ����������������if���(����numΪ0)�Ĵ���
				//��ʱ��currӦ��Ϊ�գ�����Ҫ�ϲ�
				linkTwoTrees(smallTree, small_link_key, Btree(curr));
			if (new_node != nullptr)//ͬ����
				linkTwoTrees(bigTree, big_link_key, Btree(new_node));
			break;
		}
	}
}

int main()
{
	Btree<char,2> bt,small,big;
	vector<char> cvec = { 'P', 'C', 'M', 'T', 'X', 'A', 'B', 'D', 'E',
		'F', 'J', 'K', 'L', 'N'};
	for (size_t i = 0; i != cvec.size(); ++i)
		bt.insert(cvec[i]);
	cout << "original B Tree------------" << endl;
	bt.sequentialPrint();
	bt.splitTree('F', small, big);
	/*bt.splitTree('B', small,big);
	bt.splitTree('D', small, big);
	bt.splitTree('A', small, big);*/
	cout << "small-----------------------" << endl;
	small.sequentialPrint();
	cout << "big-------------------------" << endl;
	big.sequentialPrint();
	getchar();
	return 0;
}