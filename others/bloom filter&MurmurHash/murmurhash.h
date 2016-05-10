#ifndef _MURMURHASH_H
#define _MURMURHASH_H

//-----------------------------------------------------------------------------
// MurmurHash2, 64-bit versions, by Austin Appleby
// The same caveats as 32-bit MurmurHash2 apply here - beware of alignment 
// and endian-ness issues if used across multiple platforms.

#include<cstdint>

class murmurhash
{
public:
	//Ӧ����64λƽ̨��64λhash
	uint64_t MurmurHash64A(const void* key, int len, uint32_t seed)
	{
		const uint64_t m = 0xc6a4a7935bd1e995;
		const int r = 47;

		uint64_t h = seed ^ (len * m);//��ʼhashֵ
		const uint64_t * data = (const uint64_t *)key;
		const uint64_t * end = data + (len / 8);

		while (data != end)
		{//���ϼ���hashֵ
			uint64_t k = *data++;
			k *= m;
			k ^= k >> r;
			k *= m;

			h ^= k;
			h *= m;
		}
		const unsigned char* data2 = (const unsigned char*)data;//key��ʣ�µ��ֽ�����
		switch (len & 7)
		{//ʣ���ֽ���
		case 7: h ^= static_cast<uint64_t>(data2[6]) << 48;
		case 6: h ^= static_cast<uint64_t>(data2[5]) << 40;
		case 5: h ^= static_cast<uint64_t>(data2[4]) << 32;
		case 4: h ^= static_cast<uint64_t>(data2[3]) << 24;
		case 3: h ^= static_cast<uint64_t>(data2[2]) << 16;
		case 2: h ^= static_cast<uint64_t>(data2[1]) << 8;
		case 1: h ^= static_cast<uint64_t>(data2[0]);
				h *= m;
		};
		h ^= h >> r;
		h *= m;
		h ^= h >> r;
		return h;
	}
	// Ӧ����32λƽ̨��64λhash
	uint64_t MurmurHash64B(const void* key, int len, uint32_t seed)
	{
		const uint32_t m = 0x5bd1e995;
		const int r = 24;
		//��Ҫ��Ϊ�����ּ���
		uint32_t h1 = seed ^ len; 
		uint32_t h2 = 0;

		const uint32_t* data = (const uint32_t*)key;
		while (len >= 8)
		{
			//�����32λhash
			uint32_t k1 = *data++;
			k1 *= m; k1 ^= k1 >> r; k1 *= m;
			h1 *= m; h1 ^= k1;
			len -= 4;
			//�����32λhash
			uint32_t k2 = *data++;
			k2 *= m; k2 ^= k2 >> r; k2 *= m;
			h2 *= m; h2 ^= k2;
			len -= 4;
		}

		if (len >= 4)
		{//���ʣ���ֽ���Ŀ����4������������32λhash
			uint32_t k1 = *data++;
			k1 *= m; k1 ^= k1 >> r; k1 *= m;
			h1 *= m; h1 ^= k1;
			len -= 4;
		}
		const unsigned char* data2 = (const unsigned char*)data;
		switch (len)
		{//����ʣ���ֽ��������32hash
		case 3: h2 ^= data2[2] << 16;
		case 2: h2 ^= data2[1] << 8;
		case 1: h2 ^= data2[0];
				h2 *= m;
		};

		h1 ^= h2 >> 18; h1 *= m;
		h2 ^= h1 >> 22; h2 *= m;
		h1 ^= h2 >> 17; h1 *= m;
		h2 ^= h1 >> 19; h2 *= m;

		uint64_t h = h1;
		h = (h << 32) | h2;//�ϲ��ߵ�32λhash
		return h;
	}
	//32λhash
	uint32_t MurmurHash32(const void* key, size_t len, uint32_t seed) 
	{
		const uint32_t m = 0xc6a4a793;
		const uint32_t r = 24;
		const uint32_t* data = (const uint32_t*)key;
		uint32_t h = seed ^ (len * m);
		while (len >= 4)
		{
			uint32_t w = *data++;
			h += w;
			h *= m;
			h ^= (h >> 16);
			len -= 4;
		}
		const unsigned char* data2 = (const unsigned char*)data;
		switch (len)
		{
		case 3: h += data2[2] << 16;
		case 2:	h += data2[1] << 8;
		case 1: h += data2[0];
				h *= m;
				h ^= (h >> r);
		}
		return h;
	}
};

#endif