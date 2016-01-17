
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

struct task
{//任务节点
	int id;
	int deadline;
	int weight;
	task(int i = 0,int d = 0,int w = 0):id(i), deadline(d), weight(w){}
};

bool compareW(const task &lhs, const task &rhs)
{//按权值从大到小排序
	return lhs.weight > rhs.weight;
}

bool compareD(const task &lhs, const task &rhs)
{//按截止时间从小到大排序
	return lhs.deadline < rhs.deadline;
}

void independentSet(vector<task> &t, vector<size_t> Nt,vector<task> &select)
{//选择独立任务子集，Nt[i]表示独立子集中在时间i之前可以结束的任务数，O(n^2)
	size_t last_time = t.size();//单位时间任务，有多少个任务就有多少时间
	for (size_t i = 0; i != t.size(); ++i)
	{
		bool compatible = true;
		for (size_t time = t[i].deadline; time <= last_time; ++time)
			if (Nt[time] + 1 > time)//从任务i截止时间开始判断任务i加入后是否仍然独立(相容)
			{//根据P240的16.12性质2判断，要逐一验证
				compatible = false;
				break;
			}
		if (compatible)
		{//若相容
			select.push_back(t[i]);//则加入独立子集
			for (size_t time = t[i].deadline; time <= last_time; ++time)//并更新Nt
				++Nt[time];
		}
	}
}

void taskScheduling(vector<task> select,vector<task> &t)
{//打印任务调度顺序
	int sum = 0, sum_sche = 0;
	sort(select.begin(), select.end(), compareD);//先按截止时间排序，即要早任务优先
	for (size_t i = 0; i != select.size(); ++i)
	{
		printf("id:%-3d deadline:%-3d weight:%-3d\n", select[i].id, select[i].deadline, select[i].weight);
		sum_sche += select[i].weight;//计算惩罚值
	}
	for (size_t i = 0; i != t.size(); ++i)
		sum += t[i].weight;
	cout << "The minimum punishment is " << (sum - sum_sche) << endl;//最小惩罚值
}

int main()
{
	int n;
	cout << "The number of task: ";
	while (cin >> n)
	{
		vector<task> t, select;
		vector<size_t> Nt(n + 1);//Nt[i]表示独立子集中在时间i之前可以结束的任务数
		int d, w,i = 1;
		cout << "Enter the deadline and weight,respectively" << endl;
		while (i <= n)
		{
			cin >> d >> w;
			t.push_back(task(i, d, w));
			++i;
		}
		cin.clear();
		sort(t.begin(), t.end(), compareW);
		independentSet(t, Nt, select);
		taskScheduling(select,t);
	}
	return 0;
}