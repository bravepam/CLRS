
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//算法导论，16章——贪心算法

/*
#include<iostream>
#include<vector>

using namespace std;

const int MAX = 0x7fffffff;

//活动选择问题------------------------------------------------------------------------------
struct activity
{//活动
	int start;//开始时间
	int finish;//结束时间
	activity(int s, int f) :start(s), finish(f){}
	activity() :start(0), finish(0){}
};

int partition_finish(vector<activity> &vec, int beg, int end)
{
	activity pivot = vec[beg];
	while (beg < end)
	{
		while (beg < end && vec[end].finish >= pivot.finish) --end;
		vec[beg] = vec[end];
		while (beg < end && vec[beg].finish <= pivot.finish) ++beg;
		vec[end] = vec[beg];
	}
	vec[beg] = pivot;
	return beg;
}

int partition_start(vector<activity> &vec, int beg, int end)
{
	activity pivot = vec[beg];
	while (beg < end)
	{
		while (beg < end && vec[end].start >= pivot.start) --end;
		vec[beg] = vec[end];
		while (beg < end && vec[beg].start <= pivot.start) ++beg;
		vec[end] = vec[beg];
	}
	vec[beg] = pivot;
	return beg;
}

void quickSort(vector<activity> &vec, int beg, int end)
{
	if (beg < end)
	{
		//int mid = partition_finish(vec, beg, end);
		int mid = partition_start(vec, beg, end);
		if (mid > beg)
			quickSort(vec, beg, mid - 1);
		quickSort(vec, mid + 1, end);
	}
}
//活动选择，动态规划版本，自底向上，时间复杂度O(n^3)，不算排序
//void activitySelector(vector<activity> &act, vector<vector<int> > &num, vector<vector<int> > &select)
//{
//	for (size_t step = 2; step != act.size(); ++step)
//	{//步长
//		for (size_t i = 0; i != act.size() - 2; ++i)
//		{
//			size_t j = i + step;
//			if (j < act.size())
//			{
//				for (size_t k = i + 1; k != j; ++k)
//				{
//					if (act[i].finish <= act[k].start && act[k].finish <= act[j].start)
//					{//是否与活动i,j兼容
//						int tmp = num[i][k] + num[k][j] + 1;
//						if (tmp > num[i][j])
//						{//使兼容活动数最大
//							num[i][j] = tmp;
//							select[i][j] = k;
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void printActivity(vector<vector<int> > &select, vector<vector<int> > &num,
//	vector<activity> &act, int beg, int end)
//{//打印所选活动
//	int k = select[beg][end];
//	if ((end - beg) < 2 || k <= beg || k >= end) return;
//	if ( (num[beg][end] == num[beg][k] + num[k][end] + 1))//满足条件，输出，然后继续递归
//		cout << act[k].start << ' ' << act[k].finish << endl;
//	printActivity(select, num, act, beg, k);
//	printActivity(select, num, act, k, end);
//}
//
//
//int main()
//{
//	vector<activity> act;//存储活动
//	act.push_back(activity());//虚拟活动
//	int s, f;
//	while (cin >> s >> f)
//		act.push_back(activity(s, f));
//	act.push_back(activity(MAX, MAX));//虚拟活动
//
//	vector<vector<int> > select(act.size()), num(act.size());//所选活动，最大兼容活动数
//	for (size_t i = 0; i != act.size(); ++i)
//	{
//		select[i].resize(act.size());
//		num[i].resize(act.size());
//	}
//
//	quickSort(act, 0, act.size() - 1);
//	activitySelector(act, num, select);
//
//	cout << "The number of activity selected: " << num[0][act.size() - 1] << endl;
//	cout << "Selected activity" << endl;
//	printActivity(select, num, act, 0, act.size() - 1);
//	getchar();
//	return 0;
//}


//贪心算法——自顶向下的递归版本，时间复杂度大幅下降，O(n),不算排序
void recursiveActivitySelector(vector<activity> &act, vector<int> &select,int beg, int end)
{
	int k = beg + 1;
	while (k <= end && act[k].start < act[beg].finish)
		++k;
	if (k <= end)
	{
		select.push_back(k);
		recursiveActivitySelector(act, select, k, end);
	}
}

//贪心算法——自顶向下的迭代版本，时间复杂度大幅下降，O(n),不算排序
void greedyActivitySelector(vector<activity> &act, vector<int> &select)
{
	size_t k = 0;;
	for (size_t i = 1; i != act.size(); ++i)
	{
		if (act[i].start >= act[k].finish)
		{
			select.push_back(i);
			k = i;
		}
	}
}

//贪心算法——自顶向下的迭代版本(从后面开始选)，时间复杂度大幅下降，O(n),不算排序
void greedyActivitySelectorFromLast(vector<activity> &act, vector<int> &select)
{
	size_t k = act.size() - 1;
	select.push_back(k);
	for (size_t i = act.size() - 2; i != 0; --i)
	{
		if (act[i].finish <= act[k].start)
		{
			select.push_back(i);
			k = i;
		}
	}
}

int main()
{
	vector<activity> act;//存储活动
	act.push_back(activity());//虚拟活动
	int s, f;
	while (cin >> s >> f)
		act.push_back(activity(s, f));

	vector<int> select;//所选活动，最大兼容活动数
	quickSort(act, 0, act.size() - 1);
	//recursiveActivitySelector(act, select, 0, act.size() - 1);//递归
	//greedyActivitySelector(act, select);//迭代,从前面开始选
	greedyActivitySelectorFromLast(act, select);//迭代，从后面开始选
	cout << "The number of selected activity: " << select.size() << endl;
	cout << "Selected activity" << endl;
	for (size_t i = 0; i != select.size(); ++i)
		cout << act[select[i]].start << ' ' << act[select[i]].finish << endl;
	getchar();
	return 0;
}
*/

//背包问题-------------------------------------------------------------------------
/*//0-1背包
#include<iostream>
#include<vector>


using namespace std;
struct good
{
	int weight;
	int value;
	good(int w, int v) :weight(w), value(v){}
};

void computeMaxValue(vector<good> &G, vector<int> &V,vector<int> &select)
{//计算最大价值
	for (size_t i = 0; i != G.size(); ++i)
	{//当前第i个物品，即当前第0,,,i，且背包容量为j时可以选择的最大物品价值
		for (size_t j = V.size() - 1; j >= G[i].weight; --j)
		{
			int tmp = V[j - G[i].weight] + G[i].value;//状态方程
			if (tmp > V[j])
			{
				V[j] = tmp;
				select[j] = i;//此时选择i
			}
		}
	}
}

void printSelectedGood(vector<good> &G, vector<int> &V, vector<int> &select)
{
	int W = V.size() - 1,k;
	while (true)
	{
		k = select[W];//当背包容量为W时选择物品k
		if (G[k].weight <= W)
		{//k可以放进背包
			cout << k << endl;
			W -= G[k].weight;
		}
		else break;
	}
}

int main()
{
	int N, W,w, v;
	cout << "Enter the number of goods and the maximum volumn of bag " << endl;
	while (cin >> N >> W)
	{
		vector<good> G;//存储物品
		vector<int> V(W + 1);//存储对应容量已选物品最大价值
		vector<int> select(W + 1);//存储对应价值时所选物品
		cout << "Input the weight and value of these goods,respectively" << endl;
		while (N--)
		{
			cin >> w >> v;
			G.push_back(good(w, v));
		}
		computeMaxValue(G, V,select);
		cout << "The maximum value is " << V[W] << endl;
		cout << "The selected goods as follows" << endl;
		printSelectedGood(G, V, select);
	}
	return 0;
}
*/

//加油站选择问题.dis[i]表示站点a[i]与a[i + 1]之间的距离，i = 0,1,2,,,n。其中a0为起点，油满，an为终点
//a1~an- 1是加油站。
/*
#include<iostream>
#include<png_vector.h>

void gasStatiocSelector(png_vector<int> &dis, png_vector<int> &select,int N)
{
	int distance = 0;
	for (int i = 0; i != dis.size(); ++i)
	{
		distance += dis[i];
		if (distance > N)
		{
			select.push_back(i);
			--i;
			distance = 0;
		}
	}
}

int main()
{
	int n, N;
	while (cin >> n >> N)
	{
		png_vector<int> dis,select;
		int i = n + 1, dist;
		while (cin >> dist, i--)
			dis.push_back(dist);
		gasStatiocSelector(dis, select, N);
		for (size_t i = 0; i != select.size(); ++i)
			cout << select[i] << ' ';
		cout << endl;
		cin.clear();
	}
	return 0;
}
*/

//确定一组数x[1,2...,n]的最小闭区间

//#include<iostream>
//#include<vector>
//
//using namespace std;
//
//#define min(i,j) (i < j ? i : j)
//#define max(i,j) (i > j ? i : j)
//const int MIN = 0x80000000;
//const int MAX = 0x7fffffff;

/*//贪心算法
void minimumInterval(vector<int> &x, int &left, int &right)
{
	left = MAX, right = MIN;
	for (size_t i = 0; i != x.size(); ++i)
	{
		left = min(left, x[i]);
		right = max(right, x[i]);
	}
}

int main()
{
	int n;
	while (cin >> n)
	{
		vector<int> x;
		int val;
		while (cin >> val, n--)
			x.push_back(val);
		int left = MAX, right = MIN;
		minimumInterval(x, left, right);
		cout << "The minimum interval is [" << left << ',' << right << ']' << endl;
		cin.clear();
	}
	return 0;
}
*/

/*//分治法
void minimumInterval(vector<int> &x, int beg,int end,int &left, int &right)
{
	if (end - beg == 1)
	{
		left = min(left, x[beg]);
		right = max(right, x[beg]);
		return;
	}
	int mid = (beg + end) / 2;
	minimumInterval(x,beg,mid,left,right);
	minimumInterval(x, mid, end, left, right);
}

int main()
{
	int n;
	while (cin >> n)
	{
		vector<int> x;
		int val;
		while (cin >> val, n--)
			x.push_back(val);
		int left = MAX, right = MIN;
		minimumInterval(x,0,x.size(),left,right);
		cout << "The minimum interval is [" << left << ',' << right << ']' << endl;
		cin.clear();
	}
	return 0;
}
*/

//算法导论16.2-6在O(n)时间内解决部分背包问题
#include<iostream>
#include<vector>
#include<ctime>

using namespace std;

struct good
{//物品
	int weight;
	int value;
	good(int w, int v) :weight(w), value(v){}
};

int partition(vector<good> &G, int beg, int end)
{//分割物品序列，前面为单位重量价值大的一部分，后面为小的一部分
	good pivot = G[beg];
	while (beg < end)
	{
		while (beg < end && (G[end].value / G[end].weight) <= (pivot.value / pivot.weight))
			--end;
		G[beg] = G[end];
		while (beg < end && (G[beg].value / G[beg].weight) >= (pivot.value / pivot.weight))
			++beg;
		G[end] = G[beg];
	}
	G[beg] = pivot;
	return beg;
}

int randomizedPartition(vector<good> &G, int beg, int end)
{//选定任意一个下标进行分割
	srand(unsigned int(time(0)));
	int select = (rand() % (end - beg + 1)) + beg;
	good tmp = G[beg];
	G[beg] = G[select];
	G[select] = tmp;
	return partition(G, beg, end);
}

void partialKnapsack(vector<good> &G, int beg, int end, int &W, int &value)
{//部分背包问题
	if (beg == end)
	{//若只有一个物品
		if (W >= G[beg].weight) value += G[beg].value;
		else value += W * G[beg].value / G[beg].weight;
		return;
	}
	int mid = randomizedPartition(G, beg, end);//有多个物品，则分割
	int sum_w = 0, sum_v = 0;
	for (size_t i = beg; i != mid; ++i)
	{//累加前面单位价值最高的一部分物品
		sum_w += G[i].weight;//累加重量
		sum_v += G[i].value;//累加价值
	}
	if (W < sum_w)//若W小于累加重量，则继续在前面搜寻
		partialKnapsack(G, beg, mid - 1, W, value);
	else if (W >= sum_w && W <= (sum_w + G[mid].weight))
	{//若W位于累加重量和其与mid重量之和之间，则再累加一部分mid物品价值
		value += sum_v + (W - sum_w) * G[mid].value / G[mid].weight;
		return;
	}
	else
	{//若W大于上述两者重量之和，则装入上述物品，并在后面继续搜寻
		W = W - sum_w - G[mid].weight;
		value += sum_v + G[mid].value;
		if (mid < end) partialKnapsack(G, mid + 1, end, W, value);
	}
}

int main()
{
	int N, W, w, v;
	cout << "Enter the number of goods and the maximum volumn of bag " << endl;
	while (cin >> N >> W)
	{
		vector<good> G;//存储物品
		int value = 0;
		cout << "Input the weight and value of these goods,respectively" << endl;
		while (N--)
		{
			cin >> w >> v;
			G.push_back(good(w, v));
		}
		partialKnapsack(G, 0, G.size() - 1, W, value);
		cout << "The maximum value is " << value << endl;
		cin.clear();
	}
	return 0;
}