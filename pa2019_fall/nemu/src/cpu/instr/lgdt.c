#include "cpu/instr.h"

make_instr_func(lgdt)
{
	int len = 1;
	OPERAND rm;
	rm.data_size = 16;
	len += modrm_rm(eip + 1, &rm);
	operand_read(&rm);
	cpu.gdtr.limit = rm.val;
	rm.addr += 2;
	rm.data_size = 32;
	operand_read(&rm);
	cpu.gdtr.base = rm.val;
	return len;
}
