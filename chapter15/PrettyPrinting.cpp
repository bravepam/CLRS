//�����ӡ�����㷨����15-2

#include<iostream>

using namespace std;
const int M = 10;
const int N = 10;
const int MAX = 0x7fff;

void remainCharacter(int len[],int (*remain)[N])
{//���������i,,,j��һ�п�����ַ���
	for(int i = 0;i != N;++i)
	{
		remain[i][i] = M - len[i];
		for(int j = i + 1;j != N;j++)
			remain[i][j] = remain[i][j - 1] - len[j] - 1;
	}
}

void cubeSum(int (*remain)[N],int (*cube)[N])
{//����ÿһ��i,,,j�����ַ�������������
	for(int i = 0;i != N;i++)
	{
		for(int j = i;j != N;j++)
		{
			if(remain[i][j] < 0)
				cube[i][j] = MAX;
			else if(remain[i][j] >= 0 && j == N - 1)
				cube[i][j] = 0;
			else
				cube[i][j] = pow(static_cast<double>(remain[i][j]),3);
		}
	}
}

void minSum(int (*cube)[N],int sum[],int line[])
{//����cube�����sum[i]����ʾ����0,,,i��С�����ͣ�
	sum[0] = 0;
	for(int i = 1;i != N;i++)
	{
		sum[i] = MAX;
		for(int j = 0;j != i;j++)
		{
			int tmp = sum[j] + cube[j + 1][i];
			if(tmp < sum[i])
			{
				sum[i] = tmp;
				line[i] = j;//��ס���д�
			}
		}
	}
}

void printLineEnd(int line[],int n)
{//��ӡ���д�
	if(line[n] != n)
		printLineEnd(line,line[n]);
	if(n != 0)
		cout << "line end: " << n << endl;
}

int main()
{
	int len[] = {1,2,3,1,2,3,1,2,3,1};
	int cube[N][N],remain[N][N],line[N],sum[N];
	for(int i = 0;i != N;i++)
		line[i] = i;
	remainCharacter(len,remain);
	cubeSum(remain,cube);
	minSum(cube,sum,line);
	for(int i = 0;i != N;++i)
		cout << sum[i] << ' ';
	cout << endl;
	for(int i = 0;i != N;++i)
		cout << line[i] << ' ';
	cout << endl;
	cout << "The minmum sum of cube is " << sum[N - 1] << endl;
	printLineEnd(line,N - 1);
	getchar();
	return 0;
}