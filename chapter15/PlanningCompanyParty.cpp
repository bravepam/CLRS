//�㷨����15-4���ƻ�һ����˾�ۻᣬ��̬�滮

#include<iostream>
#include<vector>

using namespace std;

#define MAX(i,j) i > j ? i : j

void computeLoveSum(vector<vector<bool> > &relation,vector<int> &love,vector<vector<int> > &love_sum)
{//����ϲ�ó̶��ܺͣ�relation�洢��ϵ����love�洢ϲ���̶ȣ�love_sum�洢ȥ���߲�ȥʱ�Ըýڵ�Ϊ�������������ϲ�ó̶�
	for(int i = love.size() - 1;i >= 0;--i)
	{
		love_sum[i][1] = love[i];//��ʼ��i��Ա��ȥ
		love_sum[i][0] = 0;//��ʼ��i��Ա����ȥ
		for(int j = 0;j != relation[i].size();++j)
			if(relation[i][j] == true)
			{//��i��Ա����j��ֱ����˾
				love_sum[i][1] += love_sum[j][0];
				love_sum[i][0] += MAX(love_sum[j][0],love_sum[j][1]);
			}
	}
}

void attendOrNot(vector<vector<int> > &love_sum)
{//��ӡԱ���Ƿ�μӾۻ�
	for(int i = 0;i != love_sum.size();++i)
		if(love_sum[i][1] > love_sum[i][0])
			cout << "staff " << i << " will go" << endl;
		else cout << "staff " << i << " will not go" << endl;
}

void init(vector<vector<bool> > &relation,vector<vector<int> > &love_sum,vector<int> &love,int N)
{//��ʼ��
	cout << "Enter love of " << N << " staffs" << endl;
	for(int i = 0;i != N;++i)
		cin >> love[i];//���ϲ�ó̶�
	for(int i = 0;i != N;++i)
	{
		relation[i].resize(N);//relation��N*N����
		love_sum[i].resize(2);//love_sum��N*2����iȥ���߲�ȥ
	}
	cout << "The number of staff managed by leader(0,1..." << N - 1 << ")directly , and their ID" << endl;
	for(int i = 0;i != N;++i)
	{//��ø�Ա����ϵ�����ڽӾ���洢��Ա����0��ʼ���
		int number,ID;
		cin >> number;
		if(number == 0) continue;
		while(number--)
		{
			cin >> ID;
			relation[i][ID] = true;
		}
	}	
}

int main()
{
	int N;
	cout << "Enter number of staff: ";
	while(cin >> N)
	{
		vector<vector<bool> > relation(N);
		vector<vector<int> > love_sum(N);
		vector<int> love(N);
		init(relation,love_sum,love,N);
		computeLoveSum(relation,love,love_sum);
		int max = MAX(love_sum[0][1],love_sum[0][0]);
		cout << "The maximum love is " << max << endl << endl;
		attendOrNot(love_sum);
	}
	return 0;
}
/*
3 2 1 4 3 2

2 1 2
2 3 4
1 5
0
0
0
*/