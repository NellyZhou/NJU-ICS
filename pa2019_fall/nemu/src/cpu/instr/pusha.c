#include "cpu/instr.h"

make_instr_func(pusha){
	uint32_t temp = cpu.esp;
	opcode_entry[0x50](cpu.eip,0x50);
	opcode_entry[0x51](cpu.eip,0x51);
	opcode_entry[0x52](cpu.eip,0x52);
	opcode_entry[0x53](cpu.eip,0x53);

	OPERAND r;
	cpu.esp -= 4;
	r.val = temp;
	r.sreg = SREG_SS;
	r.type = OPR_MEM;
	r.addr = cpu.esp;
	r.data_size = data_size;
	operand_write(&r);
	

	opcode_entry[0x55](cpu.eip,0x55);
	opcode_entry[0x56](cpu.eip,0x56);
	opcode_entry[0x57](cpu.eip,0x57);
	return 1;

}
