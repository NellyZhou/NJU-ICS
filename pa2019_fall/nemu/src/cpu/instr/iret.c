#include "cpu/instr.h"

make_instr_func(iret){
	OPERAND r;
	r.data_size = 32;
	r.type = OPR_MEM;
	r.sreg = SREG_SS;

	r.addr = cpu.esp;
	operand_read(&r);
	cpu.esp += 4;
	cpu.eip = r.val;

	r.addr = cpu.esp;
	operand_read(&r);
	cpu.esp += 4;
	cpu.cs.val = r.val;
	
	r.addr = cpu.esp;
	operand_read(&r);
	cpu.esp += 4;
	cpu.eflags.val = r.val;

	return 0;
}
