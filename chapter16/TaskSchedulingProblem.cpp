
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
{//����ڵ�
	int id;
	int deadline;
	int weight;
	task(int i = 0,int d = 0,int w = 0):id(i), deadline(d), weight(w){}
};

bool compareW(const task &lhs, const task &rhs)
{//��Ȩֵ�Ӵ�С����
	return lhs.weight > rhs.weight;
}

bool compareD(const task &lhs, const task &rhs)
{//����ֹʱ���С��������
	return lhs.deadline < rhs.deadline;
}

void independentSet(vector<task> &t, vector<size_t> Nt,vector<task> &select)
{//ѡ����������Ӽ���Nt[i]��ʾ�����Ӽ�����ʱ��i֮ǰ���Խ�������������O(n^2)
	size_t last_time = t.size();//��λʱ�������ж��ٸ�������ж���ʱ��
	for (size_t i = 0; i != t.size(); ++i)
	{
		bool compatible = true;
		for (size_t time = t[i].deadline; time <= last_time; ++time)
			if (Nt[time] + 1 > time)//������i��ֹʱ�俪ʼ�ж�����i������Ƿ���Ȼ����(����)
			{//����P240��16.12����2�жϣ�Ҫ��һ��֤
				compatible = false;
				break;
			}
		if (compatible)
		{//������
			select.push_back(t[i]);//���������Ӽ�
			for (size_t time = t[i].deadline; time <= last_time; ++time)//������Nt
				++Nt[time];
		}
	}
}

void taskScheduling(vector<task> select,vector<task> &t)
{//��ӡ�������˳��
	int sum = 0, sum_sche = 0;
	sort(select.begin(), select.end(), compareD);//�Ȱ���ֹʱ�����򣬼�Ҫ����������
	for (size_t i = 0; i != select.size(); ++i)
	{
		printf("id:%-3d deadline:%-3d weight:%-3d\n", select[i].id, select[i].deadline, select[i].weight);
		sum_sche += select[i].weight;//����ͷ�ֵ
	}
	for (size_t i = 0; i != t.size(); ++i)
		sum += t[i].weight;
	cout << "The minimum punishment is " << (sum - sum_sche) << endl;//��С�ͷ�ֵ
}

int main()
{
	int n;
	cout << "The number of task: ";
	while (cin >> n)
	{
		vector<task> t, select;
		vector<size_t> Nt(n + 1);//Nt[i]��ʾ�����Ӽ�����ʱ��i֮ǰ���Խ�����������
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