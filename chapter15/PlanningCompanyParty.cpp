//算法导论15-4，计划一个公司聚会，动态规划

#include<iostream>
#include<vector>

using namespace std;

#define MAX(i,j) i > j ? i : j

void computeLoveSum(vector<vector<bool> > &relation,vector<int> &love,vector<vector<int> > &love_sum)
{//计算喜好程度总和，relation存储关系网，love存储喜爱程度，love_sum存储去或者不去时以该节点为根的子树的最大喜好程度
	for(int i = love.size() - 1;i >= 0;--i)
	{
		love_sum[i][1] = love[i];//初始，i号员工去
		love_sum[i][0] = 0;//初始，i号员工不去
		for(int j = 0;j != relation[i].size();++j)
			if(relation[i][j] == true)
			{//若i号员工是j的直接上司
				love_sum[i][1] += love_sum[j][0];
				love_sum[i][0] += MAX(love_sum[j][0],love_sum[j][1]);
			}
	}
}

void attendOrNot(vector<vector<int> > &love_sum)
{//打印员工是否参加聚会
	for(int i = 0;i != love_sum.size();++i)
		if(love_sum[i][1] > love_sum[i][0])
			cout << "staff " << i << " will go" << endl;
		else cout << "staff " << i << " will not go" << endl;
}

void init(vector<vector<bool> > &relation,vector<vector<int> > &love_sum,vector<int> &love,int N)
{//初始化
	cout << "Enter love of " << N << " staffs" << endl;
	for(int i = 0;i != N;++i)
		cin >> love[i];//获得喜好程度
	for(int i = 0;i != N;++i)
	{
		relation[i].resize(N);//relation是N*N矩阵
		love_sum[i].resize(2);//love_sum是N*2矩阵，i去或者不去
	}
	cout << "The number of staff managed by leader(0,1..." << N - 1 << ")directly , and their ID" << endl;
	for(int i = 0;i != N;++i)
	{//获得各员工关系网，邻接矩阵存储，员工从0开始编号
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