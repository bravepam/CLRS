#ifndef _BLOOMFILTER_H
#define _BLOOMFILTER_H

#include<cassert>
#include<vector>
#include<string>
#include"murmurhash.h"

using namespace std;

class BloomFilter
{//��¡������
private:
	struct BloomArgs
	{
		const int num_elements; //Ԫ������n
		int bits_num; //����bit����m
		int hashfuncs; //����hash��������k
		const double error_p; //����p
		BloomArgs(int n_, double p_) :num_elements(n_), error_p(p_)
		{//����n��p�������������������ѧԭ������������
			assert(num_elements > 0 && error_p < 1.0 && error_p > 0.0);
			const double ln2 = log(2.), lnp = log(error_p);
			const int temp_m = static_cast<int>(-(num_elements * lnp / (ln2 * ln2)));
			bits_num = (temp_m / 8 + 1) * 8; //����Բ����8�ı���
			hashfuncs = static_cast<int>(ln2 * (bits_num * 1.0 / num_elements));
			if (hashfuncs < 1) hashfuncs = 1;
			if (hashfuncs > 30) hashfuncs = 30;
		}
	};

	const BloomArgs bloomargs;
	string bloomfilter;
	static murmurhash hash;
public:
	BloomFilter(int n_, double p_) :bloomargs(BloomArgs(n_, p_)){}
	void createFilter(const vector<string>& keys)
	{//���������
		assert(static_cast<int>(keys.size()) == bloomargs.num_elements);
		bloomfilter.resize(bloomargs.bits_num / 8, 0);//��������С
		for (int i = 0; i != bloomargs.num_elements; ++i)
		{
			//hash(key) = (h1 + k * h2) mod m
			uint32_t h1 = hash.MurmurHash32(keys[i].data(), keys[i].size(), 0);
			const uint32_t h2 = (h1 >> 17) | (h1 << 15);
			for (int k = 0; k != bloomargs.hashfuncs; ++k)
			{
				uint32_t bitpos = h1 % bloomargs.bits_num;
				bloomfilter[bitpos / 8] |= (1 << (bitpos % 8)); //�ø�λΪ1
				h1 += h2;
			}
		}
	}
	bool keyMayMatch(const string& key)const
	{//��ѯ���Ƿ�����ڸü���
		uint32_t h1 = hash.MurmurHash32(key.data(), key.size(), 0);
		const uint32_t h2 = (h1 >> 17) | (h1 << 15);
		for (int k = 0; k != bloomargs.hashfuncs; ++k)
		{//����hash�������в���
			uint32_t bitpos = h1 % bloomargs.bits_num;
			if ((bloomfilter[bitpos / 8] & (1 << (bitpos % 8))) == 0)
				return false;//������κ�һλΪ0����˵���ز�����
		}
		return true;
	}
};

murmurhash BloomFilter::hash;

#endif