#include "memory/cache.h"
#include "memory/memory.h"
#include "cpu/cpu.h"
#include <memory.h>
#include <time.h>
#include <stdlib.h>

void init_cache(){
//	printf("initailized success!\n");
	for (int i = 0; i < 1024; i++){
		L1_dcache[i].validbit = false;
		L1_dcache[i].tag = 0;
		memset(L1_dcache[i].data, 0, 64);
	}
	return;
}

uint32_t cache_read(paddr_t paddr, size_t len, CacheLine* cache){
//	return hw_mem_read(paddr, len);
//	printf("read entry: %x  %x\n", paddr,len);
	uint32_t b_addr = paddr & 0x3f;
	uint32_t q_slot = (paddr >> 6) & 0x7f;
	uint32_t m_tag = paddr >> 13;

	for (int i = q_slot * 8; i < (q_slot + 1) * 8; i++){
		if (cache[i].tag != m_tag)
			continue;
		if (!cache[i].validbit)
			continue;
		uint32_t ans = 0;
		if (b_addr + len > 64){
//			printf("	read hw_mem\n");
			memcpy(&ans,hw_mem + paddr, len);
		}
		else {
//			printf("	read cache\n");
			memcpy(&ans, cache[i].data + b_addr, len);
		}
//		printf("read end\n");
		return ans;

	}

	
//	return hw_mem_read(paddr, len);

//	printf("	fail to find %x\n", paddr);	
	srand(time(0));
	int k = q_slot * 8 + (rand() % 8);
	for (int i = q_slot * 8; i < (q_slot + 1) * 8; i++)
		if (!cache[k].validbit){
			k = i;
			break;
		}
	cache[k].validbit = true;
	cache[k].tag = m_tag;
	paddr_t slot_addr = (paddr >> 6) << 6;
//	printf("	load hw_mem to cache:%d  %d\n",q_slot, k);
	
	
	memcpy(cache[k].data, hw_mem + slot_addr, 64);
//	printf("read update\n");
	return cache_read(paddr, len, cache);

}

void cache_write(paddr_t paddr,size_t len, uint32_t data, CacheLine* cache){
	uint32_t q_slot = (paddr >> 6) & 0x7f;
	uint32_t m_tag = paddr >> 13;
	for (int i = q_slot * 8; i < (q_slot + 1) * 8; i++){
		if (cache[i].tag != m_tag)
			continue;
		if (!cache[i].validbit)
			continue;
		memcpy(hw_mem + paddr, &data, len);
		uint32_t slot_addr = paddr & (~0x3f);
		memcpy(cache[i].data, hw_mem + slot_addr, 64);
		return;
	}

	memcpy(hw_mem + paddr, &data, len);
	return;
}
