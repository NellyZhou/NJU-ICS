#include "cpu/instr.h"

static void instr_execute_2op()
{
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	operand_read(&opr_src);
	operand_read(&opr_dest);
	
	opr_dest.val &= opr_src.val;
	cpu.eflags.ZF = (opr_dest.val == 0);
	cpu.eflags.SF = (opr_dest.val >> (data_size - 1)) & 1;
	cpu.eflags.PF = 1;
	for (int i = 0; i < data_size; i++){
		cpu.eflags.PF ^= opr_dest.val;
		opr_dest.val >>= 1;
	}
	cpu.eflags.PF &= 1;

	
}

make_instr_impl_2op(test,r,rm,b)
make_instr_impl_2op(test,r,rm,v)
make_instr_impl_2op(test,i,rm,b)
make_instr_impl_2op(test,i,rm,v)
make_instr_impl_2op(test,i,a,b)
make_instr_impl_2op(test,i,a,v)
