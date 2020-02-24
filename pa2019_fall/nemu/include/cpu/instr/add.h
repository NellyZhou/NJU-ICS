#ifndef __INSTR_ADD_H__
#define __INSTR_ADD_H__

#define make_define_arigth(name) \
	make_instr_func(concat(name,r2rm_b)); \
	make_instr_func(concat(name,r2rm_v)); \
	make_instr_func(concat(name,rm2r_b)); \
	make_instr_func(concat(name,rm2r_v)); \
	make_instr_func(concat(name,i2rm_b)); \
	make_instr_func(concat(name,i2rm_v)); \
	make_instr_func(concat(name,i2rm_bv)); \
	make_instr_func(concat(name,i2a_b)); \
	make_instr_func(concat(name,i2a_v)); 

make_define_arigth(add_)
make_define_arigth(adc_)
make_define_arigth(sub_)
make_define_arigth(sbb_)
make_define_arigth(and_)
make_define_arigth(xor_)
make_define_arigth(or_)


#endif
