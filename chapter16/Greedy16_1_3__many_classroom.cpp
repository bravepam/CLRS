
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//�ж�����ң��ж���������Щ�������ң�Ҫ�����ý�����Ŀ���١�ǰ�᣺������ʱ������
//˼·1�����淽�����ɨ����Щ���ÿ�η���һ�����ң�ֱ�����л������ϣ�ʱ�临�Ӷ�Q(n^2)
//˼·2��ɨ��һ����Щ��ͷ���á�������С���ȼ����У����д洢���ң�
//�������������ʱ��last���Ѿ������ڴ˽��ҵĻ
//(1) ɨ�赽�i����ö���Ԫ��top���Ƚ�start[i],��last[top]
//(2) �����ڻ���ڣ���˵���i���԰����ڴ˽��ң�ͬʱ����last[top] = finish[i]
//(3) ����Ҫ�����½��ҷ��ɻi��ͬʱ����last = finish[i]��ʱ�临�Ӷ�O(nlgn)

#include<iostream>
#include<vector>
#include<queue>

using namespace std;

struct activity
{//�
	int start;
	int finish;
	activity(int s = 0, int f = 0) :start(s), finish(f){}
};

struct classroom
{//����
	int last;//��������ʱ��
	vector<int> select;//�Ѱ����ڴ˽��ҵĻ
	classroom() :last(0), select(){}
};

int partition(vector<activity> &act, int beg, int end)
{//�ָ�
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
{//����
	if (beg < end)
	{
		int mid = partition(act, beg, end);
		if (mid > beg)
			quickSort(act, beg, mid - 1);
		quickSort(act, mid + 1, end);
	}
}

bool typedef (*comp)(const classroom &, const classroom &);//����Ƚ�������

void allocateClassroom(vector<activity> act, priority_queue<classroom, vector<classroom>, comp> &Q)
{//�������
	for (size_t i = 0; i != act.size(); ++i)
	{
		classroom cl;
		if (i == 0)
		{//���ǵ�һ�������ʱ����Ϊ��
			cl.select.push_back(i);
			cl.last = act[i].finish;
			Q.push(cl);
			continue;
		}
		classroom cl_top = Q.top();
		if (cl_top.last <= act[i].start)
		{//���ý�����������ʱ�䲻�����i��ʼʱ��
			cl_top.select.push_back(i);//��i���԰����ڴ˽���
			cl_top.last = act[i].finish;//ͬʱ����last
			Q.pop();//ɾ����ǰ����
			Q.push(cl_top);//���봦����״̬�Ľ���
		}
		else
		{//��������½���
			cl.select.push_back(i);
			cl.last = act[i].finish;
			Q.push(cl);
		}
	}
}

bool compare(const classroom &lhs, const classroom &rhs)
{//�Ƚ�����ʵ����С���ȼ�����
	return lhs.last > rhs.last;
}

void printAllocation(vector<activity> &act, priority_queue<classroom, vector<classroom>, comp> &Q)
{//��ӡ��������
	size_t num = 0;//���ұ��
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