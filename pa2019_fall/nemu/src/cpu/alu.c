#include "cpu/cpu.h"

/*
uint32_t sign_ext(uint32_t res, size_t data_size){
	uint32_t tmp = ((res >> (data_size - 1)) ^ 1) & 0x1;
	uint32_t key = ((0xFFFFFFFF >> data_size) + tmp) << data_size;
	return res | key;
}

uint32_t sign(uint32_t res){
	return (res >> 31) & 0x1;
}

*/
void set_CF_add(uint32_t res, uint32_t src, size_t data_size){
	res = sign_ext(res & (0xFFFFFFFF >> (32 - data_size)),data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)),data_size);
	cpu.eflags.CF = res < src;
}


void set_CF_adc(uint32_t res, uint32_t src, uint32_t ci, size_t data_size){
	
	res = sign_ext(res & (0xFFFFFFFF >> (32 - data_size)),data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)),data_size);
	cpu.eflags.CF = (res < src) || ((res == src) && (ci == 1));

}

void set_ZF(uint32_t res, size_t data_size){
	res = sign_ext(res & (0xFFFFFFFF >> (32 - data_size)),data_size);
	cpu.eflags.ZF = (res == 0); 
}

void set_SF(uint32_t res, size_t data_size){
	res = sign_ext(res & (0xFFFFFFFF >> (32 - data_size)),data_size);
	cpu.eflags.SF = sign(res);
}

void set_PF(uint32_t res, size_t data_size){
	res = res & (0xFFFFFFFF >> (32 - data_size));
	int sum=0;
	for (int i = 0; i < 8; i++){
		sum += res & 0x1;
		res >>= 1;
	}
	cpu.eflags.PF = (~(sum & 1)) & 0x1;
}

void set_OF(uint32_t res, uint32_t src, uint32_t dest, size_t data_size){
	switch (data_size){
		case 16:
			res = sign_ext(res & 0xFFFF,16);
			src = sign_ext(src & 0xFFFF,16);
			dest = sign_ext(dest & 0xFFFF,16);
			break;
		case 8:
			res = sign_ext(res & 0xFF,8);
			src = sign_ext(src & 0xFF,8);
			dest = sign_ext(dest & 0xFF,8);
			break;
		default:break;
	}
	if (sign(src) == sign(dest)){
		if (sign(res) != sign(src))
			cpu.eflags.OF = 1;
		else 
			cpu.eflags.OF = 0;
	} else 
		cpu.eflags.OF = 0;
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
	res = dest + src;
	res = res & (0xFFFFFFFF >> (32 - data_size));
	
	set_CF_add(res,src,data_size);
	set_ZF(res,data_size);
	set_OF(res,src,dest,data_size);
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;

/*	
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif

*/
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
	res = dest + src + cpu.eflags.CF;
	res = res & (0xFFFFFFFF >> (32 - data_size));
	
	set_CF_adc(res,src,cpu.eflags.CF,data_size);
	set_ZF(res,data_size);
	set_OF(res,src,dest,data_size);
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;

	/*
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
	*/
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{

	uint32_t res = 0;
	res = dest - src;
	res = res & (0xFFFFFFFF >> (32 - data_size));
	
	set_CF_add(dest,res,data_size);
	set_ZF(res,data_size);
	set_OF(dest,res,src,data_size);
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;
	/*
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = dest - src - cpu.eflags.CF;
	res = res & (0xFFFFFFFF >> (32 - data_size));
	
	set_CF_adc(dest,src,cpu.eflags.CF,data_size);
	set_ZF(res,data_size);
	set_OF(dest,src,res,data_size);
	set_SF(res,data_size);
	set_PF(res,data_size);
	
	return res;
	/*
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif */
}

void set_CF_mul(uint64_t res, size_t data_size){
	res = res >> data_size;
	cpu.eflags.CF = (res != 0);
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
//	uint64_t res = src * dest & (0xFFFFFFFFFFFFFFFF >> (64 - (data_size << 1)));	
	uint64_t res = (uint64_t) src * dest;
	set_CF_mul(res,data_size);
	set_ZF(res,data_size);
	cpu.eflags.OF = cpu.eflags.CF;
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
	uint32_t q0 = 0;
	uint32_t q1 = src & 0x1;
	uint64_t k = dest;
	uint64_t res = 0;
	for (int i = 0; i < data_size; i++){
		if (q0 == 0 && q1 == 1){
			res = res - k;
		}
		if (q1 == 0 && q0 == 1){
			res = res + k;
		}
		q0 = q1;
		k <<= 1;
		src >>= 1;
		q1 = src & 1;
	}

	return res;
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
	if (src == 0){
		printf("error 400!");
		assert(0);
		return 0;
	}
	uint32_t res = dest / src;
//	set_CF_mul(res,data_size);
	set_ZF(res,data_size);
//	cpu.eflags.OF = cpu.eflags.CF;
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
	if (src == 0){
		printf("error 400!");
		assert(0);
		return 0;
	}
	uint32_t res = dest / src;
//	set_CF_mul(res,data_size);
	set_ZF(res,data_size);
//	cpu.eflags.OF = cpu.eflags.CF;
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
	uint32_t res = dest % src;
	return res;
}

int32_t alu_imod(int64_t src, int64_t dest)
{
	uint32_t res = dest % src;
	return res;
}


uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = (src & dest) & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = 0;
	set_ZF(res,data_size);
	set_OF(res,src,dest,data_size);
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = (src ^ dest) & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = 0;
	set_ZF(res,data_size);
	cpu.eflags.OF = 0;
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = (src | dest) & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = 0;
	set_ZF(res,data_size);
	set_OF(res,src,dest,data_size);
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;
}

void set_CF_shl(uint32_t src, uint32_t dest, size_t data_size){
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	if (src > data_size){
		cpu.eflags.CF = (dest & 0);
		return;
	}
	dest = dest >> (data_size - src);
	cpu.eflags.CF = (dest & 1);
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{

	uint32_t res = (dest << src) & (0xFFFFFFFF >> (32 - data_size));
	set_CF_shl(src,dest,data_size);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t key = sign(dest);
	uint32_t res = (dest & (0xFFFFFFFF >> (32 - data_size))) >> src;
	cpu.eflags.CF = (dest >> (src - 1)) & 1;
	set_ZF(res,data_size);
	
	cpu.eflags.OF = (key != sign(res)); 

	set_SF(res,data_size);
	set_PF(res,data_size);
	return res;
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t key = (dest & (1 << (data_size - 1))) >> src;
	key = ~key + 1;
	uint32_t res = (dest & (0xFFFFFFFF >> (32 - data_size))) >> src;
	res = (res | key) & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = (dest >> (src - 1)) & 1;
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_PF(res,data_size);

	return res;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
	return alu_shl(src,dest,data_size);
}
