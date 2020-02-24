#include "cpu/instr.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	uint32_t src = sign_ext(opr_src.val, opr_src.data_size);
	alu_sub(src, opr_dest.val, opr_dest.data_size);
	if ((cpu.eip == 0x832190)&&(src != 0)){
//		printf("edi: %x\n",cpu.edi);
		if(src != 0){
//			printf("(edi): %x	al: %x\n",opr_dest.val, src);
	
		}
	}
//	printf("%d %d\n",src,opr_dest.val);
//	printf("eflags zf:%x sf:%x\n",cpu.eflags.ZF,cpu.eflags.SF);
}

make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, i, rm, bv)
make_instr_impl_2op(cmp, i, a, b)
make_instr_impl_2op(cmp, i, a, v)
make_instr_impl_2op(cmp, r, rm, b)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, rm, r, v)
