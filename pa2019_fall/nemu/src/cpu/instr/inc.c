#include "cpu/instr.h"
#define c_inc(a) ((a) + 1)
#define c_dec(a) ((a) - 1)
#define c_not(a) (~(a))


#define name_execute_1op(inst_name) \
	operand_read(&opr_src); \
	opr_src.val = concat(c_,inst_name) (opr_src.val); \
	operand_write(&opr_src); 

#define make_agrith_logic_1op(name) \
	make_instr_name_1op(name,r,b) \
	make_instr_name_1op(name,r,v) \
	make_instr_name_1op(name,rm,b) \
	make_instr_name_1op(name,rm,v)

make_agrith_logic_1op(inc)
make_agrith_logic_1op(dec)
make_agrith_logic_1op(not)
