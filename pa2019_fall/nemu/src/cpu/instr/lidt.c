#include "cpu/instr.h"

make_instr_func(lidt){
	int len = 1;
	OPERAND r;
	len += modrm_rm(eip + 1, &r);
	r.data_size = data_size;
	operand_read(&r);
	cpu.idtr.limit = r.val & 0xFFFF;
	r.addr += 2;
	operand_read(&r);
	cpu.idtr.base = r.val;

	return len;
}
