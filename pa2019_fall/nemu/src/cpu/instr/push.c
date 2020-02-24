#include "cpu/instr.h"

static void instr_execute_1op(){
	operand_read(&opr_src);
	cpu.esp -= data_size / 8;
	OPERAND r;
	r.addr = cpu.esp;
	r.type = OPR_MEM;
	r.sreg = SREG_SS;
	r.data_size = data_size;
	r.val = opr_src.val;
	operand_write(&r);
}

make_instr_impl_1op(push,r,v)
make_instr_impl_1op(push,i,v)
make_instr_impl_1op(push,i,b)
make_instr_impl_1op(push,rm,v)
