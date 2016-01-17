
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//最小化平均结束时间的调度，算法导论16-2(a)
//每次选择执行时间最短的任务，那么最好按任务执行时间从小到大排列，时间复杂度O(nlgn)
//代码略

//(b) 由于可以抢占，那么每次选择的都是剩余执行时间最短的任务，为了便于选择这样的任务
//采取最小优先级队列。

#include<iostream>
#include<vector>
#include<queue>

using namespace std;

struct task
{//任务节点
	int id;
	int exe;//执行时间
	int preemptable;//松弛时间，在此时间段内可以抢占
	task(int i, int e, int p) :id(i), exe(e), preemptable(p){}
};

bool typedef (*comp)(const task&, const task&);

bool compare(const task &lhs, const task &rhs)
{//定义比较函数，用于构建最小优先级队列
	return lhs.exe > rhs.exe;
}

void scheduling(priority_queue<task, vector<task>, comp> &Q)
{//调度函数
	//task_no用于给新建任务赋以id，sum存储总结束时间，count存储当前已用时间，每执行一单位时间，自增1
	int task_no = Q.size(),sum = 0,count = 0;
	while (!Q.empty())
	{
		task t = Q.top();
		cout << "NO." << t.id << " starts at " << count << endl;
		int i;
		for (i = 0; i != t.exe; ++i)
		{//在任务t的运行时间内
			char new_task;
			cout << "New task is conming? [y/n] -> ";//每个时刻都判断是否新任务加入
			cin >> new_task;
			if (new_task == 'y')
			{//若有
				int exe_time, pre;
				cout << "Enter its execution and preemptable time" << endl;
				cin >> exe_time >> pre;//则创建新任务
				task tmp_task = task(++task_no, exe_time, pre);
				if (i < t.preemptable && tmp_task.exe < (t.exe - i))
				{//而如果此时t正处于松弛时间，新任务的执行时间又比它剩余执行时间短，则抢占
					cout << "NO." << t.id << " is preempted by NO." << task_no << endl;
					Q.top().exe -= i;//同时更新t的执行时间，不要直接更新t，因为t是临时变量，不在队列里面
					Q.push(tmp_task);//然后再将新任务推入队列
					break;//此单位时间内t没有被执行
				}
				else Q.push(tmp_task);//否则直接将新任务推入队列
			}
			++count;//此单位时间结束，自增1
		}
		if (i == t.exe)
		{//如果任务t结束了
			cout << "NO." << t.id << " is finished at " << count << endl;
			sum += count;//则累加结束时间
			Q.pop();//并在队列中删除
		}
	}
	cout << "The minimum average time is " << (sum / task_no) << endl;
}

/*3 2
  4 3
  2 0
  5 1
  1 1
  6 4
  */
int main()
{
	int e, p;
	bool con = true;
	while (con)
	{
		priority_queue<task, vector<task>, comp> Q(compare);
		int no = 1;
		cout << "Enter tasks' execution and preemptable time" << endl;
		while (cin >> e >> p)
			Q.push(task(no++, e, p));
		cin.clear();
		scheduling(Q);
		cin >> con;
	}
}