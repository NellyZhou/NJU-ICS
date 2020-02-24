#include "cpu/instr.h"
#include "cpu/intr.h"

make_instr_func(_int){
	OPERAND r;
	r.data_size = 8;
	r.type =OPR_IMM;
	r.sreg = SREG_CS;
	r.addr = eip + 1;
	operand_read(&r);
	raise_sw_intr(r.val);
	return 0;
}
