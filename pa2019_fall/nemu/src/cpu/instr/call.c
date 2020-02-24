#include "cpu/instr.h"
make_instr_func(call_near_indirect)
{
	OPERAND rm;
	int len = 1;
	rm.data_size = data_size;
	len += modrm_rm(eip + 1, &rm);
	operand_read(&rm);
        int offset = sign_ext(rm.val, rm.data_size); 
        print_asm_1("jmp", "", 1 + data_size / 8, &rm); 
	
	cpu.esp -= 4;
	OPERAND r;
	r.addr = cpu.esp;
	r.type = OPR_MEM;
	r.sreg = SREG_DS;
	r.val = cpu.eip + len;
	r.data_size = data_size;
	operand_write(&r);
	cpu.eip = offset;
	return 0;
}

make_instr_func(call_near)
{
	uint16_t len = 1;
	len += data_size / 8;
	OPERAND rm;
	rm.type = OPR_MEM;
	rm.sreg = SREG_CS;
	rm.addr = cpu.eip + 1;
	rm.data_size = data_size;	
	operand_read(&rm);	
	int offset = sign_ext(rm.val,data_size);
	
	cpu.esp -= 4;
	OPERAND r;
	r.addr = cpu.esp;
	r.type = OPR_MEM;
	r.sreg = SREG_DS;
	r.val = cpu.eip + len;
	r.data_size = data_size;
	operand_write(&r);
	return  len + offset;	
}
