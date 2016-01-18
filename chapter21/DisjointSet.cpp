#include<iostream>
#include<vector>

using namespace std;

class UFS
{//默认从1开始
private:
	vector<size_t> parent;
	vector<size_t> rank;
public:
	explicit UFS(size_t size) :parent(size + 1), rank(size + 1){}
	void makeSet(size_t i)
	{
		parent[i] = i;
		rank[i] = 0;
	}
	void Union(size_t x, size_t y)
	{
		size_t left = findSet(x), right = findSet(y);
		if (rank[left] < rank[right])
			parent[left] = right;
		else
		{
			parent[right] = left;
			if (rank[left] == rank[right])
				++rank[left];
		}
	}
	/*size_t findSet(size_t x)
	{
		if (x == parent[x]) return x;
		parent[x] = findSet(parent[x]);
		return parent[x];
	}*/
	size_t findSet(size_t x)
	{
		size_t p = x;
		while (p != parent[p])
			p = parent[p];
		while (x != p)
		{
			size_t y = parent[x];
			parent[x] = p;
			x = y;
		}
		return parent[x];
	}
};

int main()
{
	UFS ufs(16);
	for (size_t i = 1; i <= 16; ++i)
		ufs.makeSet(i);
	for (size_t i = 1; i <= 15;i += 2)
		ufs.Union(i, i + 1);
	for (size_t i = 1; i <= 13; i += 4)
		ufs.Union(i, i + 2);
	ufs.Union(1, 5);
	ufs.Union(11, 13);
	ufs.Union(1, 10);
	cout << ufs.findSet(2) << endl;
	cout << ufs.findSet(9) << endl;
	getchar();
	return 0;
}