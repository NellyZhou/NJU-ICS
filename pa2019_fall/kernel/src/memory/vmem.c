#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PAGE_SIZE - 1) / PAGE_SIZE) // number of page tables to cover the vmem

PTE kptables[NR_PT] align_to_page;
PDE *get_updir();

void create_video_mapping()
{

	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */

	PDE *pdir = (PDE *)va_to_pa(get_updir());
	PTE *ptable = (PTE *)va_to_pa(kptables);
	uint32_t ptable_idx, pframe_idx;

	pframe_idx = 0xa0;
	pdir[0].val = make_pde(ptable);
	ptable += 0xa0;
//	Log("nrpt %x	scr %x	pt %x",NR_PT, SCR_SIZE, PAGE_SIZE);
	for (ptable_idx = 0; ptable_idx < NR_PT; ptable_idx++){
		ptable->val = make_pte(pframe_idx << 12);
		pframe_idx++;
		ptable++;
	}

//	panic("please implement me");
}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
