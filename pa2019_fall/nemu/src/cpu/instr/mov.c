#include "cpu/instr.h"

static void instr_execute_2op() 
{
//	printf("eip = 0x%x\n",cpu.eip);
//	printf("mov src: %x dest %x\n",opr_src.addr, opr_dest.addr);
	operand_read(&opr_src);
//	printf("read src ok\n");
//	if(((cpu.eip == 0x832190) || (cpu.eip == 0x832190)) && ( == 0xbffffa))
//		printf("at %x: esp %x\n",cpu.eip,cpu.esp);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
//	printf("read dest ok\n");
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

make_instr_func(mov_c2r_l){
//	printf("at eip = 0x%x :\n ",cpu.eip);
//	printf("mov_c2r_l");
//	printf("\tpe: %x pg: %x   ",cpu.cr0.pe, cpu.cr0.pg);
//	printf("\t cr3 0x%x\n",cpu.cr3.pdtr);
	OPERAND greg,creg;
	modrm_r_rm(eip + 1, &creg, &greg);
	creg.type = OPR_CREG;
	creg.data_size = greg.data_size = 32;
	operand_read(&creg);
	greg.val = creg.val;
	operand_write(&greg);

//	printf("mov_c2r success pe: %x pg: %x  ",cpu.cr0.pe, cpu.cr0.pg);
//	printf("\t cr3 0x%x\n",cpu.cr3.pdtr);
	return 2;

}

make_instr_func(mov_r2c_l){
//	printf("at eip = 0x%x :\n ",cpu.eip);
//	printf("mov_r2c_l ");
//	printf("\tpe: %x pg: %x   ",cpu.cr0.pe, cpu.cr0.pg);
//	printf("\t cr3 0x%x\n",cpu.cr3.pdtr);
	OPERAND greg,creg;
	modrm_r_rm(eip + 1, &creg, &greg);
	creg.type = OPR_CREG;
	creg.data_size = greg.data_size = 32;
	operand_read(&greg);
//	printf("\treg : 0x%x\n",greg.val);
	creg.val = greg.val;
	operand_write(&creg);
//	printf("mov_r2c success pe: %x pg: %x  ",cpu.cr0.pe, cpu.cr0.pg);
//	printf("\t cr3 0x%x\n",cpu.cr3.pdtr);
	return 2;


}

make_instr_func(mov_rm2s_w){
	int len = 1;
	opr_dest.data_size = 16;
	opr_src.data_size = 16;
	opr_src.sreg = SREG_CS;
	len += modrm_r_rm(eip + 1, &opr_dest, &opr_src);
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	opr_dest.type = OPR_SREG;
	operand_write(&opr_dest);
	return len;
}

make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}
