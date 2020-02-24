#ifndef __INSTR_INC_H__
#define __INSTR_INC_H__

#define make_define_agrith_1op(name) \
	make_instr_func(concat(name,r_b)); \
	make_instr_func(concat(name,r_v)); \
	make_instr_func(concat(name,rm_b)); \
	make_instr_func(concat(name,rm_v)); 

make_define_agrith_1op(inc_)
make_define_agrith_1op(dec_)
make_define_agrith_1op(not_)

#endif
