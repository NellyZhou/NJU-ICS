#include "cpu/instr.h"

make_instr_func(ret_near)
{
	OPERAND rm;
	rm.addr = cpu.esp;
	rm.data_size = data_size;
	rm.type = OPR_MEM;
	rm.sreg = SREG_SS;
	operand_read(&rm);
	cpu.eip = rm.val;
	cpu.esp += 4;
	return 0;
}

make_instr_func(ret_near_imm16)
{
	OPERAND rm;
	rm.addr = cpu.esp;
	rm.data_size = data_size;
	rm.type = OPR_MEM;
	rm.sreg = SREG_CS;
	operand_read(&rm);
	cpu.eip = rm.val;
	int offset = sign_ext(instr_fetch(eip + 1,2), 16);
	cpu.esp += data_size / 8 + offset;
	return 0;
}
