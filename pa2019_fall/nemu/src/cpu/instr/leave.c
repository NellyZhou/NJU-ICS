#include "cpu/instr.h"

make_instr_func(leave){
	cpu.esp = cpu.ebp;
	OPERAND rm;
	rm.addr = cpu.esp;
	rm.data_size = data_size;
	rm.type = OPR_MEM;
	rm.sreg = SREG_SS;
	operand_read(&rm);
	cpu.ebp = rm.val;
	cpu.esp += data_size / 8;
	return 1;
}
