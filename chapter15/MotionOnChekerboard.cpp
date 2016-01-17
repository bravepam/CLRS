#include<iostream>
#include<vector>
#include<ctime>

using namespace std;

#define MAX3(i,j,k) (i > j ? (i > k ? i : k) : ( j > k ? j : k))//�����������ֵ

void computeSumMoney(vector<vector<int> > &money,vector<vector<int> > &sum_money)
{//sum_money[i][j]��ʾ��money[i][j]λ��ʱ�õ��Ľ�Ǯ���ֵ
	for(vector<vector<int> >::size_type i = 0;i != money.size();++i)
	{
		for(vector<int>::size_type j = 0;j != money[i].size();++j)
		{
			if(i == 0)//��ʼ���
				sum_money[i][j] = money[i][j];
			else
			{//����
				int left_down = 0,right_down = 0;
				if(j > 0)
					left_down = sum_money[i -  1][j - 1];
				if(j < money[i].size() - 1)
					right_down = sum_money[i - 1][j + 1];
				//���ǵ���ʽ
				sum_money[i][j] = MAX3(left_down,sum_money[i - 1][j],right_down) + money[i][j];
			}
		}
	}
}

void printTrace(vector<vector<int> > &sum_money)
{//��ӡ�ƶ�·��
	size_t tmp = sum_money.size();
	vector<int> trace(tmp);
	int max = 0;
	for(vector<int>::size_type j = 0;j != sum_money[tmp - 1].size();++j)
		if(sum_money[tmp - 1][j] > max)//����ȡ�����һ������Ǯ���������������п��Ի�õĽ�Ǯ���ֵ
		{
			max = sum_money[tmp - 1][j];
			trace[tmp - 1] = j;//���������б�
		}
	for(int i = tmp - 2;i >= 0;--i)
	{//�����ӵ����ڶ��п�ʼ�������Ż������Ǯ·��
		int tmp_max = 0;
		for(int j = trace[i + 1] - 1;j <= trace[i + 1] + 1;++j)
		{//��ʱ�����ֵֻ������trace[i]�Լ�������������
			if(j < 0 || j >= tmp) continue;
			if(sum_money[i][j] > tmp_max)
			{
				tmp_max = sum_money[i][j];
				trace[i] = j;
			}
		}
	}
	cout << "The maximum money is " << max << endl;
	cout << "print motion trace-----------" << endl;
	for(vector<int>::size_type i = 0;i != trace.size();++i)
		cout << "[ " << i << ',' << trace[i] << " ]" << endl;
}

void init(vector<vector<int> > &money)
{//������������ʼ��money����
	srand(unsigned int(time(0)));
	for(vector<vector<int> >::size_type i = 0;i != money.size();++i)
		for(vector<int>::size_type j = 0;j != money[i].size();++j)
			money[i][j] = rand() % 100;//0~100��Χ
}

int main()
{
	int N;
	cout << "Enter the dimension of chekerboard: ";
	while(cin >> N)
	{
		vector<vector<int> > money(N),sum_money(N);
		for(vector<int>::size_type i = 0;i != money.size();++i)
		{
			money[i].resize(N);
			sum_money[i].resize(N);
		}
		init(money);
		cout << "money matrix--------" << endl;
		for(vector<vector<int> >::size_type i = 0;i != money.size();++i)
		{
			for(vector<int>::size_type j = 0;j != money[i].size();++j)
				printf("%6d ",money[i][j]);
			printf("\n");
		}
		computeSumMoney(money,sum_money);
		cout << "sum money matrix----" << endl;
		for(vector<vector<int> >::size_type i = 0;i != sum_money.size();++i)
		{
			for(vector<int>::size_type j = 0;j != sum_money[i].size();++j)
				printf("%6d ",sum_money[i][j]);
			printf("\n");
		}
		printTrace(sum_money);
	}
	return 0;
}
