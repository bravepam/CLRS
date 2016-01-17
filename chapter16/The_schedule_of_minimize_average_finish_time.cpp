
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//��С��ƽ������ʱ��ĵ��ȣ��㷨����16-2(a)
//ÿ��ѡ��ִ��ʱ����̵�������ô��ð�����ִ��ʱ���С�������У�ʱ�临�Ӷ�O(nlgn)
//������

//(b) ���ڿ�����ռ����ôÿ��ѡ��Ķ���ʣ��ִ��ʱ����̵�����Ϊ�˱���ѡ������������
//��ȡ��С���ȼ����С�

#include<iostream>
#include<vector>
#include<queue>

using namespace std;

struct task
{//����ڵ�
	int id;
	int exe;//ִ��ʱ��
	int preemptable;//�ɳ�ʱ�䣬�ڴ�ʱ����ڿ�����ռ
	task(int i, int e, int p) :id(i), exe(e), preemptable(p){}
};

bool typedef (*comp)(const task&, const task&);

bool compare(const task &lhs, const task &rhs)
{//����ȽϺ��������ڹ�����С���ȼ�����
	return lhs.exe > rhs.exe;
}

void scheduling(priority_queue<task, vector<task>, comp> &Q)
{//���Ⱥ���
	//task_no���ڸ��½�������id��sum�洢�ܽ���ʱ�䣬count�洢��ǰ����ʱ�䣬ÿִ��һ��λʱ�䣬����1
	int task_no = Q.size(),sum = 0,count = 0;
	while (!Q.empty())
	{
		task t = Q.top();
		cout << "NO." << t.id << " starts at " << count << endl;
		int i;
		for (i = 0; i != t.exe; ++i)
		{//������t������ʱ����
			char new_task;
			cout << "New task is conming? [y/n] -> ";//ÿ��ʱ�̶��ж��Ƿ����������
			cin >> new_task;
			if (new_task == 'y')
			{//����
				int exe_time, pre;
				cout << "Enter its execution and preemptable time" << endl;
				cin >> exe_time >> pre;//�򴴽�������
				task tmp_task = task(++task_no, exe_time, pre);
				if (i < t.preemptable && tmp_task.exe < (t.exe - i))
				{//�������ʱt�������ɳ�ʱ�䣬�������ִ��ʱ���ֱ���ʣ��ִ��ʱ��̣�����ռ
					cout << "NO." << t.id << " is preempted by NO." << task_no << endl;
					Q.top().exe -= i;//ͬʱ����t��ִ��ʱ�䣬��Ҫֱ�Ӹ���t����Ϊt����ʱ���������ڶ�������
					Q.push(tmp_task);//Ȼ���ٽ��������������
					break;//�˵�λʱ����tû�б�ִ��
				}
				else Q.push(tmp_task);//����ֱ�ӽ��������������
			}
			++count;//�˵�λʱ�����������1
		}
		if (i == t.exe)
		{//�������t������
			cout << "NO." << t.id << " is finished at " << count << endl;
			sum += count;//���ۼӽ���ʱ��
			Q.pop();//���ڶ�����ɾ��
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