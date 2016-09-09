#ifndef TRIE_H
#define TRIE_H

#include<string>
#include<unordered_map>
#include<vector>
#include<utility>

using namespace std;

class Trie
{//ǰ׺��/�ֵ���
private:
	struct node
	{
		int count; //�Ӹ����ýڵ��·�������ɵĵ��ʳ��ֵĴ������������ýڵ�
		unordered_map<char, node*> children; //�����б�����ӵ��ͬһǰ׺
		node() :count(0), children(){}
	};
private:
	node *root;

	void destroy(node*);
	void countingFrequency(node*, string&, vector<pair<string, int>>&)const;
public:
	Trie() :root(new node()){}
	void create(const vector<string>&);
	void insert(const string&);
	int search(const string&)const;
	//ͳ�����г��ֵĵ��ʵ�Ƶ��
	vector<pair<string, int>> countingFrequency(int, int = 10)const;
	void clear()
	{
		destroy(root);
		root->children.clear();
	}
	~Trie()
	{
		clear();
		delete root;
	}
};

void Trie::insert(const string &s)
{
	node *p = root;
	for (int i = 0; i != s.size(); ++i)
	{
		if (p->children.find(s[i]) == p->children.end())
		{//����ýڵ�ĺ����б����ڸ��ַ����򴴽�һ���ڵ㣬���ַ�Ϊ��ֵ
			p->children.emplace(s[i], new node());
		}
		p = p->children[s[i]];
	}
	p->count++; //�õ��ʳ��ִ�����1
}

void Trie::create(const vector<string> &ss)
{
	for (int i = 0; i != ss.size(); ++i)
		insert(ss[i]);
}

int Trie::search(const string &s)const
{
	node *p = root;
	for (int i = 0; i != s.size(); ++i)
	{
		//�����б�û�и��ַ�����˵���õ��ʲ�����
		if (p->children.find(s[i]) == p->children.end())
			return 0;
		p = p->children[s[i]];
	}
	return p->count;
}

void Trie::destroy(node *r)
{
	if (r->children.empty())
	{
		delete r;
		return;
	}
	for (auto iter = r->children.begin(); iter != r->children.end();
		++iter)
		destroy(iter->second); //��ɾ������
	if (r != root)
		delete r; //��ɾ���Լ�
}

vector<pair<string, int>> Trie::countingFrequency(int wordcnt, int word_avg_len)const
{
	vector<pair<string, int>> countings;
	if (root->children.empty())
		return move(countings);
	countings.reserve(wordcnt);
	string prefix;
	prefix.reserve(word_avg_len);
	countingFrequency(root, prefix, countings);
	return move(countings);
}

void Trie::countingFrequency(node *r, string &prefix,
	vector<pair<string, int>> &countings)const
{
	if (r->children.empty() || r->count > 0)
	{//1��ΪҶ�ӽڵ㣻2����ĿǰΪֹ��prefix�����ɵĵ��ʳ��ֹ�
		countings.emplace_back(make_pair(prefix, r->count));
		if (r->children.empty())
			return;
	}
	for (auto iter = r->children.begin(); iter != r->children.end(); ++iter)
	{
		prefix.push_back(iter->first);
		countingFrequency(iter->second, prefix, countings);
		prefix.pop_back();
	}
}

#endif