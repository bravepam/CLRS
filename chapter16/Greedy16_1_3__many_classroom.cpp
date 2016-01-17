
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//有多个教室，有多个活动，给这些活动分配教室，要求所用教室数目最少。前提：按活动完成时间排序
//思路1：常规方法多次扫描这些活动，每次分配一个教室，直至所有活动分配完毕，时间复杂度Q(n^2)
//思路2：扫描一次这些活动就分配好。运用最小优先级队列，队中存储教室，
//其包含最晚活动结束时间last和已经安排在此教室的活动
//(1) 扫描到活动i，获得队首元素top，比较start[i],和last[top]
//(2) 若大于或等于，则说明活动i可以安排在此教室，同时更改last[top] = finish[i]
//(3) 否则要分派新教室分派活动i，同时更新last = finish[i]。时间复杂度O(nlgn)

#include<iostream>
#include<vector>
#include<queue>

using namespace std;

struct activity
{//活动
	int start;
	int finish;
	activity(int s = 0, int f = 0) :start(s), finish(f){}
};

struct classroom
{//教室
	int last;//最晚活动结束时间
	vector<int> select;//已安排在此教室的活动
	classroom() :last(0), select(){}
};

int partition(vector<activity> &act, int beg, int end)
{//分割
	activity pivot = act[beg];
	while (beg < end)
	{
		while (beg < end && act[end].finish >= pivot.finish) --end;
		act[beg] = act[end];
		while (beg < end && act[beg].finish <= pivot.finish) ++beg;
		act[end] = act[beg];
	}
	act[beg] = pivot;
	return beg;
}

void quickSort(vector<activity> &act, int beg, int end)
{//快排
	if (beg < end)
	{
		int mid = partition(act, beg, end);
		if (mid > beg)
			quickSort(act, beg, mid - 1);
		quickSort(act, mid + 1, end);
	}
}

bool typedef (*comp)(const classroom &, const classroom &);//定义比较器类型

void allocateClassroom(vector<activity> act, priority_queue<classroom, vector<classroom>, comp> &Q)
{//分配教室
	for (size_t i = 0; i != act.size(); ++i)
	{
		classroom cl;
		if (i == 0)
		{//若是第一个活动，这时队列为空
			cl.select.push_back(i);
			cl.last = act[i].finish;
			Q.push(cl);
			continue;
		}
		classroom cl_top = Q.top();
		if (cl_top.last <= act[i].start)
		{//若该教室最晚活动结束时间不超过活动i开始时间
			cl_top.select.push_back(i);//则活动i可以安排在此教室
			cl_top.last = act[i].finish;//同时更新last
			Q.pop();//删除当前队首
			Q.push(cl_top);//插入处于新状态的教室
		}
		else
		{//否则分配新教室
			cl.select.push_back(i);
			cl.last = act[i].finish;
			Q.push(cl);
		}
	}
}

bool compare(const classroom &lhs, const classroom &rhs)
{//比较器，实现最小优先级队列
	return lhs.last > rhs.last;
}

void printAllocation(vector<activity> &act, priority_queue<classroom, vector<classroom>, comp> &Q)
{//打印活动分配情况
	size_t num = 0;//教室标号
	while (!Q.empty())
	{
		classroom t = Q.top();
		cout << "classroom " << num << endl;
		for (size_t i = 0; i != t.select.size(); ++i)
			cout << act[t.select[i]].start << ' ' << act[t.select[i]].finish << endl;
		Q.pop(); ++num;
	}
}

int main()
{
	int start, finish;
	bool con = true;
	while (con)
	{
		vector<activity> act;
		while (cin >> start >> finish)
			act.push_back(activity(start, finish));
		priority_queue<classroom, vector<classroom>, comp> Q(compare);
		quickSort(act, 0, act.size() - 1);
		allocateClassroom(act, Q);
		printAllocation(act, Q);
		cout << "continue? 1/0 :";
		cin.clear();
		cin >> con;
	}
	return 0;
}