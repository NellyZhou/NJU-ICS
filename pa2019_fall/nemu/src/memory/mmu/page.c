#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	uint32_t dir = (laddr >> 22) & 0x3ff;
	uint32_t page = (laddr >> 12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;
//	printf("cr3.base = 0x%x\n",cpu.cr3.pdtr);
	PDE* pdir = (PDE*) (hw_mem + (cpu.cr3.pdtr << 12) + (dir << 2));
	assert(pdir->present == 1);
	PTE* ptable = (PTE*)(hw_mem + (pdir->page_frame << 12) + (page << 2));	
	if (ptable->present != 1){
		printf("eip %x: edi:%x\n",cpu.eip,cpu.edi);

		printf("laddr 0x%x\n",laddr >> 12);
	printf("\tfind pdir: 0x%x\n",pdir->page_frame);
	printf("addr: 0x%x \n", (pdir->page_frame << 12) + (page << 2));
		printf("\tfind ptable: ");
		printf("0x%x\n",ptable->page_frame);
		printf("%x %x\n ",((ptable->page_frame << 12) + offset + 0xc0000000), laddr);
	}
	assert(ptable->present == 1);
//	assert((ptable->page_frame << 12) + offset + 0xc0000000 == laddr || (ptable->page_frame << 12) + offset == laddr); 
	return (ptable->page_frame << 12) + offset;

#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
