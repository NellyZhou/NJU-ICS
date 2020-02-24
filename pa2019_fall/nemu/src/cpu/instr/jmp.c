#include "cpu/instr.h"

make_instr_func(jmp_near_indirect){
	OPERAND rm;
	rm.data_size = data_size;
	int len = 1;
	len += modrm_rm(eip + 1, &rm);
	operand_read(&rm);
	cpu.eip = rm.val & (0xFFFFFFFF >> (32 - data_size));
//	printf("0x%x\n",cpu.eip);
	return 0;


}

make_instr_func(jmp_near){ 
	OPERAND rel;
	rel.type = OPR_IMM; 
	rel.sreg = SREG_CS; 
        rel.data_size = data_size;
	rel.addr = eip + 1; 
        operand_read(&rel); 
        int offset = sign_ext(rel.val, data_size); 
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);
        cpu.eip += offset; 
        return 1 + data_size / 8; 
}

make_instr_func(jmp_short_){
	OPERAND rel;
	rel.type = OPR_IMM; 
	rel.sreg = SREG_CS; 
        rel.data_size = 8;
	rel.addr = eip + 1; 
        operand_read(&rel); 
        int offset = sign_ext(rel.val, 8); 
        print_asm_1("jmp", "", 1 + data_size / 8, &rel); 
  	cpu.eip += offset;
        return 2; 
}

make_instr_func(jmp_far_imm){
	OPERAND rm;
	rm.type = OPR_IMM;
	rm.sreg = SREG_CS;
	rm.data_size = 32;
	rm.addr = eip + 1;
	operand_read(&rm);
	if (data_size == 16)
		cpu.eip = rm.val & 0xFFFF;
	else
		cpu.eip = rm.val;
	cpu.cs.val = instr_fetch(eip + 5, 2);
	load_sreg(1);
	return 0;

}
