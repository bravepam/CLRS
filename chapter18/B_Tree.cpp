
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//B树

#include<iostream>
#include<vector>

using namespace std;

template < typename T, int degree = 3, typename Compare = less<T> >
struct node
{//B树节点类型,degree为度，默认为3
	static const int min_num = degree - 1;//每个节点的最小关键字数
	static const int max_num = 2 * degree - 1;//叶子节点最大关键字数，内节点要减1
	static Compare compare;
	int num = 0;//该节点关键字数目
	size_t height = 0;//以该节点为根的树高
	bool leaf = true;//是否为叶子节点
	T key[max_num];//关键字数组
	node *child[max_num + 1];//孩子节点数组

	static void setCompare(const Compare &c){ compare = c; }
	node()
	{//默认构造函数
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

//定义静态变量
template < typename T, int degree, typename Compare> const int node<T, degree, Compare>::max_num;
template < typename T, int degree, typename Compare> const int node<T, degree, Compare>::min_num;
template < typename T, int degree, typename Compare> Compare node<T, degree, Compare>::compare;

template < typename T, int degree = 3, typename Compare = less<T> >
int node<T, degree,Compare>::search(const T &k)const
{//节点内关键字查找
	int low = 0, high = num - 1;
	while (low < high)
	{//二分查找
		int mid = (low + high) / 2;
		if (!compare(k,key[mid]) && !compare(key[mid],k)) return mid;
		else if (compare(k,key[mid])) high = mid - 1;
		else low = mid + 1;
	}
	if (compare(key[low], k) && low < num - 1) ++low;
	return low;//返回第一个不小于k的关键字的索引，如果全部都比k小，则low是最后一个关键字索引
}

template < typename T, int degree = 3, typename Compare = less<T> >
void node<T, degree, Compare>::insert(const T &k)
{//节点内插入
	int i = num - 1;
	while (i >= 0 && compare(k,key[i]))
	{//找寻插入位置
		key[i + 1] = key[i];
		--i;
	}
	key[++i] = k;//插入
	++num;
}

template <typename T,int degree = 3,class Compare>
bool node<T, degree, Compare>::erase(int index)
{//节点内删除
	for (int i = index + 1; i != num; ++i)
		key[i - 1] = key[i];
	--num;
	return true;
}

template < typename T, int degree = 3, typename Compare = less<T> >
class Btree
{//B树
public:
	typedef node<T, degree, Compare>					node;
	typedef Compare										comp;
private:
	node *root;
	Compare compare;
	void destroy(node*);//销毁树
	node* split(node*, int);//分割节点
	node* underfillSplit(node*, int);//未满分裂，树分裂专用
	void insert_aux(node*, const T&);//插入辅助
	bool erase_aux(node*, const T&);//删除辅助
	void merge(node*, int);//合并节点
	T erasePredecessor(node*);//删除前驱
	T eraseSuccessor(node*);//删除后继
	void borrowFromRightSibling(node*, int);//向右兄弟借关键字
	void borrowFromLeftSibling(node*, int);//向左兄弟借关键字
	void print(node*)const;//按关键字非递减顺序打印
	
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
		root = new node;//树将会长高
		root->child[0] = p;
		root->height = p->height + 1;
		root->leaf = false;
		split(root, 0);//树根分裂
	}
	explicit Btree(node *r) :root(r), compare(r->compare){}//只在分裂函数中被调用，私有
public:
	Btree() :root(nullptr), compare(Compare()){ node::setCompare(compare); }
	Btree(const Compare &c) :root(nullptr), compare(c){ node::setCompare(c); }
	//size_t getHeight()const { return root->height; }
	bool empty()const { return root == nullptr; }
	void insert(const T&);
	bool search(const T&)const;
	void link(const T&, Btree&);//树连接，链接相关都是思考题18-2的内容
	void splitTree(const T&,Btree&,Btree&);//树分裂
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
{//在B树中查找k
	node *curr = root;
	while (curr != nullptr)
	{
		int index = curr->search(k);//在当前节点查找
		if (!compare(k, curr->key[index]) && !compare(curr->key[index], k)) return true;//若存在
		else if (compare(k,curr->key[index]))//若k小于index处的关键，则在其左边查找
			curr = curr->child[index];
		else curr = curr->child[index + 1];//否则在右边查找
	}
	return false;
}

template <typename T, int degree, class Compare>
node<T,degree,Compare>* Btree<T, degree, Compare>::split(node *curr, int index)
{//将curr所指节点的index处孩子节点分割成两个节点，两边各degree - 1个关键字，index处关键字上升到curr中
	node *new_child = new node,*old_child = curr->child[index];
	T k = old_child->key[degree - 1];
	for (int first = 0; first != degree - 1; ++first)//将原节点的后一半关键字复制到新节点
		new_child->key[first] = old_child->key[first + degree];
	if (!old_child->leaf)//如果不是叶子
		for (int first = 0; first != degree; ++first)//则还要复制一半的孩子节点指针
			new_child->child[first] = old_child->child[first + degree];
	new_child->leaf = old_child->leaf;
	new_child->height = old_child->height;
	new_child->num = degree - 1;//新节点关键字数
	old_child->num -= degree;//原节点关键字数减半
	for (int last = curr->num - 1; last >= index; --last)//将curr中index（包括）之后的关键字全部后移一个位置
		curr->key[last + 1] = curr->key[last];
	for (int last = curr->num; last > index; --last)//将curr中index（不包括）之后的孩子指针全部后移一个位置
		curr->child[last + 1] = curr->child[last];
	curr->key[index] = k;//在curr中的index处填上升上来的关键字
	curr->child[index + 1] = new_child;//index后的孩子指针指向新节点
	++curr->num;
	return new_child;
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::insert_aux(node *curr, const T &k)
{//插入辅助函数
	if (curr->leaf) curr->insert(k);//若当前节点是叶子，则直接插入
	else
	{//否则
		int index = curr->search(k);//找到第一个不小于k的关键字索引
		if (compare(curr->key[index], k)) ++index;//极端情况，该节点所有关键字均小于k
		if ((curr->child[index])->num == node::max_num)
		{//若该所引处的孩子节点关键字数已满
			split(curr, index);//则将其从该处分割
			if (compare(curr->key[index], k))//分割后上升上来的关键字若小于k
				++index;//则将要到新生成的节点中继续插入
		}
		insert_aux(curr->child[index], k);//递归插入
	}
}

template <typename T, int degree, class Compare>
void Btree<T, degree, Compare>::insert(const T &k)
{//插入关键字
	if (root == nullptr)
	{//如果在空树中插入第一个关键字
		root = new node;
		root->insert(k);
		return;
	}
	else if (root->num == node::max_num)//否则如果根节点满
		rootFull();
	insert_aux(root, k);
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::merge(node *curr, int index)
{//合并curr中index处的孩子节点和其右兄弟，此时两者均恰好只有degree - 1个关键字
	node *left = curr->child[index], *right = curr->child[index + 1];
	left->key[degree - 1] = curr->key[index];//curr中index处关键字先下降
	for (int i = 0; i != right->num; ++i)//复制right所有关键字过来
		left->key[degree + i] = right->key[i];
	for (int i = index + 1; i != curr->num; ++i)//将curr中index之后的关键字前移1
		curr->key[i - 1] = curr->key[i];
	if (!left->leaf)//如果不是叶子
		for (int i = 0; i <= right->num; ++i)//则还要移动right的孩子指针
			left->child[degree + i] = right->child[i];
	for (int i = index + 2; i <= curr->num; ++i)//移动curr中index+2（包括）指针前移1，index处不必被覆盖
		curr->child[i - 1] = curr->child[i];
	--curr->num; ++left->num;
	left->num += right->num;
	delete right;
}

template <typename T,int degree,class Compare>
T Btree<T, degree, Compare>::erasePredecessor(node *curr)
{//删除前驱，在情况2.a中被调用
	if (curr->leaf)
	{//若是叶子，则说明已经可以删除最后一个元素，即前驱了
		T tmp = curr->key[curr->num - 1];
		--curr->num;
		return tmp;//返回前驱
	}
	//否则是内节点，继续递归向下？
	else if (curr->child[curr->num]->num == node::min_num)
	{//若最后一个孩子关键字数目已达最小值
		if (curr->child[curr->num - 1]->num > node::min_num)//若左兄弟有多余关键字
			borrowFromLeftSibling(curr, curr->num);//那么借一个
		else merge(curr, curr->num - 1);//否则只有合并了
	}
	return erasePredecessor(curr->child[curr->num]);//继续向下递归
}

template <typename T,int degree,class Compare>
T Btree<T, degree, Compare>::eraseSuccessor(node *curr)
{//删除后继，在情况2.b中被调用
	if (curr->leaf)
	{//若是叶子节点，则直接删除最前面元素，即后继
		T tmp = curr->key[0];
		curr->erase(0);
		return tmp;
	}
	//否则，是内节点，继续向下？
	else if (curr->child[0]->num == node::min_num)
	{//若第一个孩子关键字数目已达最小值
		if (curr->child[1]->num > node::min_num)//若右兄弟有足够关键字
			borrowFromRightSibling(curr, 0);//则借一个
		else merge(curr, 0);//否则只有合并了
	}
	return eraseSuccessor(curr->child[0]);//继续向下递归
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::borrowFromRightSibling(node *curr, int index)
{//curr中index孩子向其右兄弟借一个关键字
	node *left = curr->child[index], *right = curr->child[index + 1];
	left->key[left->num] = curr->key[index];//先将curr中index处的关键字添入该子树根，left所指
	curr->key[index] = right->key[0];//再用右兄弟（right所指）第一个关键字覆盖curr中index槽位
	for (int i = 1; i != right->num; ++i)//将右兄弟从1开始的所有关键字前移1
		right->key[i - 1] = right->key[i];
	if (!left->leaf)
	{//若并非叶子，则还要设置相关孩子指针域
		left->child[left->num + 1] = right->child[0];//右兄弟第一个孩子成为left最后一个孩子
		for (int i = 1; i <= right->num; ++i)//前移右兄弟的孩子指针数组
			right->child[i - 1] = right->child[i];
	}
	++left->num; --right->num;
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::borrowFromLeftSibling(node *curr, int index)
{
	--index;//移到左兄弟槽位
	node *left = curr->child[index], *right = curr->child[index + 1];
	right->insert(curr->key[index]);
	curr->key[index] = left->key[left->num - 1];
	if (!right->leaf)
	{//非叶子，移动指针
		for (int i = right->num; i >= 0; --i)
			right->child[i + 1] = right->child[i];
		right->child[0] = left->child[left->num];
	}
	--left->num;
}

template <typename T,int degree,class Compare>
bool Btree<T, degree, Compare>::erase_aux(node *curr, const T &k)
{//删除辅助函数
	int index = curr->search(k);//找到第一个不小于k的关键字
	if (curr->leaf && (!compare(curr->key[index], k) && !compare(k, curr->key[index])))
		return curr->erase(index);//情况1，关键字在叶子
	else if (curr->leaf) return false;//不在叶子节点，则删除失败，不存在该关键字
	if (!curr->leaf && (!compare(curr->key[index], k) && !compare(k, curr->key[index])))
	{//情况2，关键字在该内节点。则使用该关键字的前驱或者后继代替
		if (curr->child[index]->num > node::min_num)
		{//情况2.a，其左孩子有足够的关键字，即至少degree - 1个，则使用前驱代替。
			//删除其前驱，并返回前驱关键字，以覆盖该关键字
			curr->key[index] = erasePredecessor(curr->child[index]);
			return true;
		}
		else if (curr->child[index + 1]->num > node::min_num)
		{//情况2.b，否则其右孩子有足够关键字，则使用后继代替
			curr->key[index] = eraseSuccessor(curr->child[index + 1]);//同上
			return true;
		}
		else
		{//否则，由于该关键字左右孩子的关键字数均处于最少，则不能采用前驱或者后继代替，那么合并左右孩子以及该关键字
			merge(curr, index);//将curr节点的index处相关关键字和孩子合并
			return erase_aux(curr->child[index], k);
		}
	}
	else
	{//情况3，关键字不在该节点，处于该关键字的左子树中
		if (compare(curr->key[index], k)) ++index;//极端情况，当curr中所有关键字均比k小时出现
		if (curr->child[index]->num == node::min_num)
		{//若左子树关键字数已到达最小值
			if (index < curr->num && curr->child[index + 1]->num > node::min_num)
				//情况3.a，存在右兄弟，且有足够节点，即至少degree - 1个，则向其借一个
				borrowFromRightSibling(curr, index);
			else if (index > 0 && curr->child[index - 1]->num > node::min_num)
				//情况3.b,存在左兄弟且关键字数足够，类似于3.a
				borrowFromLeftSibling(curr, index);
			else
			{//3.c，左/右兄弟均只有degree - 1个关键字，那么合并节点
				if (index == curr->num) --index;//呼应上述极端情况下的节点合并
				merge(curr, index);
				if (curr == root && curr->num == 0)
				{//若当前curr是根，且仅存一个元素
					root = curr->child[index];
					delete curr;//那么树高降1
					return erase_aux(root, k);
				}
			}
		}
		return erase_aux(curr->child[index], k);
	}
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::print(node *curr)const
{//打印整棵树
	if (curr == nullptr) return;
	if (curr->leaf)
	{//若是叶子节点，则打印全部关键字
		cout << "[ ";
		for (int i = 0; i != curr->num; ++i)
			cout << curr->key[i] << ' ';
		cout << "]\theight: " << curr->height << endl;
	}
	else
	{//否则
		cout << '{' << endl;
		for (int i = 0; i <= curr->num; ++i)
		{//依次打印孩子和关键字
			print(curr->child[i]);
			if (i < curr->num)
				cout << curr->key[i] << endl;
		}
		cout << "}\theight: " << curr->height << endl;
	}
}

template <typename T,int degree,class Compare>
void Btree<T, degree, Compare>::destroy(node *curr)
{//销毁B树
	if (curr == nullptr) return;
	if (curr->leaf) delete curr;//若是叶子，直接销毁
	else
	{//否则
		for (int i = 0; i <= curr->num; ++i)//依次销毁所有孩子后
			destroy(curr->child[i]);
		delete curr;//再销毁该节点
	}
}

template <typename T,int degree,class Compare = less<T>>
node<T, degree, Compare>* Btree<T, degree, Compare>::nodeAtRightOfHeight(size_t h)
{//找到给定高度的最右侧节点的父亲或者当树高为h时返回树根，给树右合并做准备，下降寻找时对于满的节点要予以分裂
	if (root->num == node::max_num)//如果根节点满
		rootFull();
	node *curr = root;
	if (curr->height == h) return curr;//若根正是该节点
	while (curr->child[curr->num]->height != h)
	{//一直往最右下找
		if (curr->child[curr->num]->num == node::max_num)//若最右孩子满
			curr = split(curr, curr->num);//则分裂，修改了一下split函数，使其返回新孩子地址
		else
			curr = curr->child[curr->num];
	}
	return curr;
}

template <typename T, int degree, class Compare = less<T>>
node<T, degree, Compare>* Btree<T, degree, Compare>::nodeAtLeftOfHeight(size_t h)
{//找到给定高度的最左侧节点的父亲，给树左合并做准备，下降寻找时对于满的节点要予以分裂
	if (root->num == node::max_num)//如果根节点满
		rootFull();
	node *curr = root;
	while (curr->child[0]->height != h)
	{//一直往最右下找
		if (curr->child[0]->num == node::max_num)//若最右孩子满
			split(curr, 0);//则分裂
		curr = curr->child[0];
	}
	return curr;
}

template <typename T,int degree,class Compare = less<T>>
void Btree<T, degree, Compare>::linkAtRight(const T &k, Btree &rhs)
{
	node *curr = nodeAtRightOfHeight(rhs.root->height);
	if (curr == root && curr->height == rhs.root->height)
	{//若两棵树正好一样高
		root = new node;
		root->insert(k);
		root->child[0] = curr;
		root->child[1] = rhs.root;
		root->height = curr->height + 1;
		root->leaf = false;
	}
	else
	{//否则，直接把关键k插入curr，然后将最右孩子指针指向被合并树。因为一路分裂下来，curr不可能满
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
{//连接转发函数，按以下四种情况转发。前提是两树均不空
	if (compare(this->root->key[0], k) && compare(k,linkedTree.root->key[0]))
	{//1、任意key[this] < k < key[linkedTree]。这里采用根的0号关键字只是区分一下是何种连接，
		//我们假设给定的关键字和树满足上述关系，下同
		if (this->root->height >= linkedTree.root->height)//1.1 本树较高或者和被连接树一样高
			linkAtRight(k, linkedTree);//则在本树右侧连接
		else
		{//1.2 否则本树较矮
			swap(root, linkedTree.root);//交换两树
			linkAtLeft(k, linkedTree);//在新的本树左侧连接
		}
	}
	else if (compare(linkedTree.root->key[0], k) && compare(k, this->root->key[0]))
	{//2、key[this] > k > key[linkedTree]
		if (linkedTree.root->height < this->root->height)//2.1 若本树高
			linkAtLeft(k, linkedTree);//则在本树左侧连接
		else
		{//2.2 否则本树较矮或者和被连接树一样高
			swap(root, linkedTree.root);//则交换两树
			linkAtRight(k, linkedTree);//在新的本树右侧连接
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
{//未满分裂，将curr节点从index处一分为二
	node *new_child = new node;
	for (int i = index + 1; i < curr->num; ++i)//移动index之后的关键到新节点
		new_child->key[i - index - 1] = curr->key[i];
	if (!curr->leaf)
	{//若不是叶子
		for (int i = index + 1; i <= curr->num; ++i)//则还要移动孩子指针
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
{//以找寻关键字k的路径p分割树，将小于k的集合合并为smallTree，大的合并为bigTree。我们假设k存在
	node *curr = root; 	root = nullptr;
	T small_link_key = T(), big_link_key = T();
	while (true)
	{//index是curr中第一个不小于k的关键字索引，或者curr所有关键字比k都小时最后一个关键字索引
		//但这并不影响分裂
		int index = curr->search(k);
		T temp = curr->key[index];
		node *new_node = underfillSplit(curr, index);//分裂该节点，返回新生成的节点地址
		if (new_node->num == 0)
		{//若新节点没有关键字
			node *r = new_node;
			new_node = new_node->child[0];
			delete r;
		}
		if (curr->num == 0)
		{//若分裂后当前节点不再有关键字
			node *r = curr;
			curr = curr->child[0];
			delete r;
		}
		if (compare(k, temp))
		{//若k小于index处关键字，则要往左走，这里我们不再更新curr，让其在该点继续循环
			linkTwoTrees(bigTree, big_link_key, Btree(new_node));
			big_link_key = temp;//记下这一次的分割关键字，以备下次再用
		}
		else if (compare(temp, k))
		{//否则若k大于index处关键字，则要往右走，这是特殊情况，当该节点关键字全部比k小时发生
			linkTwoTrees(smallTree, small_link_key, Btree(curr));
			small_link_key = temp;
			curr = new_node;//更新curr
		}
		else
		{//若相等，即已经分割完毕，那么合并左右两树，结束
			if (curr != nullptr)//如果curr是叶子，且如果经过了上面if语句(满足num为0)的处理，
				//这时候curr应当为空，不需要合并
				linkTwoTrees(smallTree, small_link_key, Btree(curr));
			if (new_node != nullptr)//同上理
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
