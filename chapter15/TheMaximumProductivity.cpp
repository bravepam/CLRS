#include<iostream>
#include<algorithm>

#include<ctime>

using namespace std;
const int N = 10;
#define min(i,k) i < k ? i : k

struct job
{
	int time;
	int productivity;
	int deadline;
	bool call;
};

int partition(job j[], int beg, int end)
{
	job pivot = j[beg];
	while (beg < end)
	{
		while (beg < end && j[end].deadline >= pivot.deadline) --end;
		j[beg] = j[end];
		while (beg < end && j[beg].deadline <= pivot.deadline) ++beg;
		j[end] = j[beg];
	}
	j[beg] = pivot;
	return beg;
}

void quickSort(job j[], int beg, int end)
{
	
	if (beg < end)
	{
		int mid = partition(j, beg, end);
		quickSort(j, beg, mid - 1);
		quickSort(j, mid + 1, end);
	}
}

void init(job j[],int (*pro)[N + 1])
{
	srand(unsigned int(time(0)));
	for (int i = 0; i < N; ++i)
	{
		j[i].time = rand() % 10 + 1;
		j[i].productivity = rand() % 10 + 1;
		j[i].deadline = rand() % 10 + 1;
		j[i].call = false;
	}
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= N; ++j)
			pro[i][j] = 0;
}

void maximumProductivity(job jj[],int (*pro)[N + 1])
{
	for (int i = 0; i <= N; ++i)
		pro[i][0] = 0;
	for (int j = 0; j <= N; ++j)
		pro[0][j] = 0;
	for (int i = 1; i <= N; ++i)
	{
		for (int j = 1; j <= jj[i - 1].deadline; ++j)
		{
			pro[i][j] = pro[i - 1][j];
			if (j > jj[i - 1].time)
			{
				if (pro[i][j] < pro[i - 1][j - jj[i - 1].time] + jj[i - 1].productivity)
				{
					pro[i][j] = pro[i - 1][ j - jj[i - 1].time ] + jj[i - 1].productivity;
					jj[i - 1].call = true;
				}
			}
		}

	}
}

void print(job jj[])
{
	for (int i = 0; i < N; ++i)
		if (jj[i].call == true)
			printf("%-5d %-5d %-5d call!\n", jj[i].time, jj[i].productivity, jj[i].deadline);
}

int main()
{
	job j[N];
	int pro[N + 1][N + 1];
	init(j,pro);
	quickSort(j, 0, N - 1);
	maximumProductivity(j, pro);
	cout << "The maximum productivity is " << pro[N][j[N - 1].deadline] << endl;
	cout << endl << "The call sequence as follow " << endl;
	print(j);
	getchar();
	return 0;
}