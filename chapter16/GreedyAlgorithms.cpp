
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//�㷨���ۣ�16�¡���̰���㷨

/*
#include<iostream>
#include<vector>

using namespace std;

const int MAX = 0x7fffffff;

//�ѡ������------------------------------------------------------------------------------
struct activity
{//�
	int start;//��ʼʱ��
	int finish;//����ʱ��
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
//�ѡ�񣬶�̬�滮�汾���Ե����ϣ�ʱ�临�Ӷ�O(n^3)����������
//void activitySelector(vector<activity> &act, vector<vector<int> > &num, vector<vector<int> > &select)
//{
//	for (size_t step = 2; step != act.size(); ++step)
//	{//����
//		for (size_t i = 0; i != act.size() - 2; ++i)
//		{
//			size_t j = i + step;
//			if (j < act.size())
//			{
//				for (size_t k = i + 1; k != j; ++k)
//				{
//					if (act[i].finish <= act[k].start && act[k].finish <= act[j].start)
//					{//�Ƿ���i,j����
//						int tmp = num[i][k] + num[k][j] + 1;
//						if (tmp > num[i][j])
//						{//ʹ���ݻ�����
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
//{//��ӡ��ѡ�
//	int k = select[beg][end];
//	if ((end - beg) < 2 || k <= beg || k >= end) return;
//	if ( (num[beg][end] == num[beg][k] + num[k][end] + 1))//���������������Ȼ������ݹ�
//		cout << act[k].start << ' ' << act[k].finish << endl;
//	printActivity(select, num, act, beg, k);
//	printActivity(select, num, act, k, end);
//}
//
//
//int main()
//{
//	vector<activity> act;//�洢�
//	act.push_back(activity());//����
//	int s, f;
//	while (cin >> s >> f)
//		act.push_back(activity(s, f));
//	act.push_back(activity(MAX, MAX));//����
//
//	vector<vector<int> > select(act.size()), num(act.size());//��ѡ��������ݻ��
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


//̰���㷨�����Զ����µĵݹ�汾��ʱ�临�Ӷȴ���½���O(n),��������
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

//̰���㷨�����Զ����µĵ����汾��ʱ�临�Ӷȴ���½���O(n),��������
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

//̰���㷨�����Զ����µĵ����汾(�Ӻ��濪ʼѡ)��ʱ�临�Ӷȴ���½���O(n),��������
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
	vector<activity> act;//�洢�
	act.push_back(activity());//����
	int s, f;
	while (cin >> s >> f)
		act.push_back(activity(s, f));

	vector<int> select;//��ѡ��������ݻ��
	quickSort(act, 0, act.size() - 1);
	//recursiveActivitySelector(act, select, 0, act.size() - 1);//�ݹ�
	//greedyActivitySelector(act, select);//����,��ǰ�濪ʼѡ
	greedyActivitySelectorFromLast(act, select);//�������Ӻ��濪ʼѡ
	cout << "The number of selected activity: " << select.size() << endl;
	cout << "Selected activity" << endl;
	for (size_t i = 0; i != select.size(); ++i)
		cout << act[select[i]].start << ' ' << act[select[i]].finish << endl;
	getchar();
	return 0;
}
*/

//��������-------------------------------------------------------------------------
/*//0-1����
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
{//��������ֵ
	for (size_t i = 0; i != G.size(); ++i)
	{//��ǰ��i����Ʒ������ǰ��0,,,i���ұ�������Ϊjʱ����ѡ��������Ʒ��ֵ
		for (size_t j = V.size() - 1; j >= G[i].weight; --j)
		{
			int tmp = V[j - G[i].weight] + G[i].value;//״̬����
			if (tmp > V[j])
			{
				V[j] = tmp;
				select[j] = i;//��ʱѡ��i
			}
		}
	}
}

void printSelectedGood(vector<good> &G, vector<int> &V, vector<int> &select)
{
	int W = V.size() - 1,k;
	while (true)
	{
		k = select[W];//����������ΪWʱѡ����Ʒk
		if (G[k].weight <= W)
		{//k���ԷŽ�����
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
		vector<good> G;//�洢��Ʒ
		vector<int> V(W + 1);//�洢��Ӧ������ѡ��Ʒ����ֵ
		vector<int> select(W + 1);//�洢��Ӧ��ֵʱ��ѡ��Ʒ
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

//����վѡ������.dis[i]��ʾվ��a[i]��a[i + 1]֮��ľ��룬i = 0,1,2,,,n������a0Ϊ��㣬������anΪ�յ�
//a1~an- 1�Ǽ���վ��
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

//ȷ��һ����x[1,2...,n]����С������

//#include<iostream>
//#include<vector>
//
//using namespace std;
//
//#define min(i,j) (i < j ? i : j)
//#define max(i,j) (i > j ? i : j)
//const int MIN = 0x80000000;
//const int MAX = 0x7fffffff;

/*//̰���㷨
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

/*//���η�
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

//�㷨����16.2-6��O(n)ʱ���ڽ�����ֱ�������
#include<iostream>
#include<vector>
#include<ctime>

using namespace std;

struct good
{//��Ʒ
	int weight;
	int value;
	good(int w, int v) :weight(w), value(v){}
};

int partition(vector<good> &G, int beg, int end)
{//�ָ���Ʒ���У�ǰ��Ϊ��λ������ֵ���һ���֣�����ΪС��һ����
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
{//ѡ������һ���±���зָ�
	srand(unsigned int(time(0)));
	int select = (rand() % (end - beg + 1)) + beg;
	good tmp = G[beg];
	G[beg] = G[select];
	G[select] = tmp;
	return partition(G, beg, end);
}

void partialKnapsack(vector<good> &G, int beg, int end, int &W, int &value)
{//���ֱ�������
	if (beg == end)
	{//��ֻ��һ����Ʒ
		if (W >= G[beg].weight) value += G[beg].value;
		else value += W * G[beg].value / G[beg].weight;
		return;
	}
	int mid = randomizedPartition(G, beg, end);//�ж����Ʒ����ָ�
	int sum_w = 0, sum_v = 0;
	for (size_t i = beg; i != mid; ++i)
	{//�ۼ�ǰ�浥λ��ֵ��ߵ�һ������Ʒ
		sum_w += G[i].weight;//�ۼ�����
		sum_v += G[i].value;//�ۼӼ�ֵ
	}
	if (W < sum_w)//��WС���ۼ��������������ǰ����Ѱ
		partialKnapsack(G, beg, mid - 1, W, value);
	else if (W >= sum_w && W <= (sum_w + G[mid].weight))
	{//��Wλ���ۼ�����������mid����֮��֮�䣬�����ۼ�һ����mid��Ʒ��ֵ
		value += sum_v + (W - sum_w) * G[mid].value / G[mid].weight;
		return;
	}
	else
	{//��W����������������֮�ͣ���װ��������Ʒ�����ں��������Ѱ
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
		vector<good> G;//�洢��Ʒ
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