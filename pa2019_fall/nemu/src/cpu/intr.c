#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
//	printf("Please implement raise_intr()");
//	assert(0);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, SREG_SS, 4, cpu.eflags.val);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, SREG_SS, 4, cpu.cs.val);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, SREG_SS, 4, cpu.eip);

	GateDesc gatedesc;
	gatedesc.val[0] = laddr_read(cpu.idtr.base + (intr_no << 3), 4);
	gatedesc.val[1] = laddr_read(cpu.idtr.base + (intr_no << 3) + 4, 4);
	if (gatedesc.type == 0xe) cpu.eflags.IF = 0;
	
	uint32_t r_offset = gatedesc.offset_15_0 + (gatedesc.offset_31_16 << 16);
	cpu.eip = r_offset;
		
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
