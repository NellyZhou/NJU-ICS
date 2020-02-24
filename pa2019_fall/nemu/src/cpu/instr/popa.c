#include "cpu/instr.h"

make_instr_func(popa){
	opcode_entry[0x5f](cpu.eip,0x5f);
	opcode_entry[0x5e](cpu.eip,0x5e);
	opcode_entry[0x5d](cpu.eip,0x5d);

	cpu.esp += 4;

	opcode_entry[0x5b](cpu.eip,0x5b);
	opcode_entry[0x5a](cpu.eip,0x5a);
	opcode_entry[0x59](cpu.eip,0x59);
	opcode_entry[0x58](cpu.eip,0x58);
	
	return 1;
}
