#include<iostream>
#include<vector>
#include<ctime>

using namespace std;

#define MAX3(i,j,k) (i > j ? (i > k ? i : k) : ( j > k ? j : k))//求三个数最大值

void computeSumMoney(vector<vector<int> > &money,vector<vector<int> > &sum_money)
{//sum_money[i][j]表示到money[i][j]位置时得到的金钱最大值
	for(vector<vector<int> >::size_type i = 0;i != money.size();++i)
	{
		for(vector<int>::size_type j = 0;j != money[i].size();++j)
		{
			if(i == 0)//初始情况
				sum_money[i][j] = money[i][j];
			else
			{//递推
				int left_down = 0,right_down = 0;
				if(j > 0)
					left_down = sum_money[i -  1][j - 1];
				if(j < money[i].size() - 1)
					right_down = sum_money[i - 1][j + 1];
				//这是递推式
				sum_money[i][j] = MAX3(left_down,sum_money[i - 1][j],right_down) + money[i][j];
			}
		}
	}
}

void printTrace(vector<vector<int> > &sum_money)
{//打印移动路径
	size_t tmp = sum_money.size();
	vector<int> trace(tmp);
	int max = 0;
	for(vector<int>::size_type j = 0;j != sum_money[tmp - 1].size();++j)
		if(sum_money[tmp - 1][j] > max)//首先取得最后一行最大金钱数，即整个问题中可以获得的金钱最大值
		{
			max = sum_money[tmp - 1][j];
			trace[tmp - 1] = j;//几下所在列标
		}
	for(int i = tmp - 2;i >= 0;--i)
	{//继续从倒数第二行开始继续倒着获得最大金钱路径
		int tmp_max = 0;
		for(int j = trace[i + 1] - 1;j <= trace[i + 1] + 1;++j)
		{//此时，最大值只可能是trace[i]以及左右两个方格
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
{//获得随机数，初始化money数组
	srand(unsigned int(time(0)));
	for(vector<vector<int> >::size_type i = 0;i != money.size();++i)
		for(vector<int>::size_type j = 0;j != money[i].size();++j)
			money[i][j] = rand() % 100;//0~100范围
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
