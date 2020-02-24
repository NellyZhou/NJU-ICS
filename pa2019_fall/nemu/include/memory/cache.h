#ifndef __CACHE_H__
#define __CACHE_H__

#include "memory/memory.h"

typedef struct{
	bool validbit;
	uint32_t tag;
	uint8_t data[64];
}CacheLine;

CacheLine L1_dcache[1024];

void init_cache();
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine* cache);
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine* cache);

#endif

