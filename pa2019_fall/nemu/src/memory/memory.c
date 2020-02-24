#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "memory/cache.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data)
{
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	if (is_mmio(paddr) == -1){
#ifdef CACHE_ENABLED
		ret = cache_read(paddr, len, L1_dcache);
#else
		ret = hw_mem_read(paddr, len);
#endif
	} else {
		ret = mmio_read(paddr, len, is_mmio(paddr));
	}
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data)
{
	if (is_mmio(paddr) == -1){
#ifdef CACHE_ENABLED
		cache_write(paddr, len, data, L1_dcache);
#else
		hw_mem_write(paddr, len, data);
#endif
	} else{
		mmio_write(paddr, len, data, is_mmio(paddr));
	}
}

uint32_t laddr_read(laddr_t laddr, size_t len)
{	
	assert(len == 1 || len == 2 || len == 4);
	uint32_t paddr = laddr;
//	printf("laddr_read--> pe: %d pg: %d\n",cpu.cr0.pe, cpu.cr0.pg);
	if (cpu.cr0.pe && cpu.cr0.pg){
//		printf("laddr_read 0x%x\n",laddr);
		if ((laddr & 0xFFF) + len > 0x1000){
			size_t len1 = 0x1000 - (laddr & 0xFFF);
			size_t len2 = len - len1;
			uint32_t data1 = 0;
			uint32_t data2 = 0;
			if (len1 == 0x3){
				uint32_t data1_1 = laddr_read(laddr, 1);
				uint32_t data1_2 = laddr_read(laddr + 1, 2);
				data1 = data1_1 + (data1_2 << 8);
			} else {
				data1 = laddr_read(laddr, len1);
			}
			laddr_t laddr1 = laddr + len1;
			if (len2 == 0x3){
				uint32_t data2_1 = laddr_read(laddr1, 1);
				uint32_t data2_2 = laddr_read(laddr1 + 1, 2);
				data2 = data2_1 + (data2_2 << 8);
			} else {
				data2 = laddr_read(laddr1, len2);
			}
			return (data1 + (data2 << (8 * len1)));
		}
		else {
			paddr = page_translate(laddr);
//			printf("Read-- laddr: 0x%x paddr 0x%x \n",laddr,paddr);
		}
	}
	return paddr_read(paddr, len);
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data)
{
	uint32_t paddr = laddr;
//	printf("laddr_write--> pe: %d pg: %d\n",cpu.cr0.pe, cpu.cr0.pg);
	if (cpu.cr0.pe && cpu.cr0.pg){
//	printf("laddr_write\n");
		if ((laddr & 0xFFF) + len > 0x1000){
			size_t len1 = 0x1000 - (laddr & 0xFFF);
			size_t len2 = len - len1;
			if (len1 == 0x3){
				laddr_write(laddr, 1, data);
				laddr_write(laddr + 1, 2, data >> 8);
			} else {
				laddr_write(laddr, len1, data);
			}
			laddr_t laddr1 = laddr + len1;
			uint32_t data1 = data >> (8 * len1);
			if (len2 == 0x3){
				laddr_write(laddr1, 1, data1);
				laddr_write(laddr1 + 1, 2, data1 >> 8);
			} else {
				laddr_write(laddr1, len2, data1);
			}
			return;
		}
		else {
			paddr = page_translate(laddr);
//			printf("Write-- laddr: 0x%x paddr 0x%x \n",laddr,paddr);
		}
	}
	
	paddr_write(paddr, len, data);
}

uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	return laddr_read(vaddr, len);
#else
	uint32_t laddr = vaddr;
//	printf("vaddr_read: 0x%x\n",vaddr);
	if (cpu.cr0.pe){
		laddr = segment_translate(vaddr, sreg);
	}
	return laddr_read(laddr, len);
#endif
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	laddr_write(vaddr, len, data);
#else
	uint32_t laddr = vaddr;
	if (cpu.cr0.pe){
		laddr = segment_translate(vaddr, sreg);
	}
	laddr_write(laddr, len, data);
#endif

}

void init_mem()
{
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);

#ifdef CACHE_ENABLED		
	init_cache();
#endif

#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t *get_mem_addr()
{
	return hw_mem;
}
