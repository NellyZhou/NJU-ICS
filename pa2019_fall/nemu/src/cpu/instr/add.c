#include "cpu/instr.h"
#define c_add(a, b) (alu_add((a),(b),opr_dest.data_size))
#define c_sub(a, b) (alu_sub((b),(a),opr_dest.data_size))
#define c_sbb(a, b) (alu_sbb((b),(a),opr_dest.data_size))
#define c_and(a, b) (alu_and((a),(b),opr_dest.data_size))
#define c_adc(a, b) (alu_adc((a),(b),opr_dest.data_size))
#define c_xor(a, b) (alu_xor((a),(b),data_size))
#define c_or(a, b) (alu_or((a),(b),data_size))

#define name_execute_2op(inst_name) \
	operand_read(&opr_src);	\
	operand_read(&opr_dest);\
	uint32_t offset = sign_ext(opr_src.val,opr_src.data_size);\
	opr_dest.val = concat(c_,inst_name) (opr_dest.val, offset);\
	operand_write(&opr_dest);


#define make_arith_logic(name) \
make_instr_name_2op(name, r, rm, b) \
make_instr_name_2op(name, r, rm, v) \
make_instr_name_2op(name, rm, r, b) \
make_instr_name_2op(name, rm, r, v) \
make_instr_name_2op(name, i, rm, b) \
make_instr_name_2op(name, i, rm, v) \
make_instr_name_2op(name, i, rm, bv) \
make_instr_name_2op(name, i, a, b) \
make_instr_name_2op(name, i, a, v)

make_arith_logic(add)
make_arith_logic(sub)
make_arith_logic(sbb)
make_arith_logic(and)
make_arith_logic(adc)
make_arith_logic(xor)
make_arith_logic(or)
