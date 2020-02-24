#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include <stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

extern uint32_t look_up_symtab(char *sym, bool *success);
extern void load_elf_tables(char * exec_file);
enum
{
	NOTYPE = 256,
	EQ,
	NUM,
	REG,
	SYMB,
	NEQ,
	SIXTEENUM,
	POINT,

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // white space
	{"\\+", '+'},	
	{"\\-", '-'},	
	{"\\*", '*'},	
	{"\\/", '/'},	
	{"==", EQ},
	{"!=", NEQ},
	{"&&", '&'},
	{"\\|\\|", '|'},
	{"!", '!'},
	{"0x[0-9a-f]+", SIXTEENUM},
	{"[0-9]+", NUM},
	{"\\$[a-ehiglpsx]{2,3}", REG},
	{"\\(", '('},
	{"\\)", ')'},
	{"[a-zA-Z][a-zA-Z_0-9]+",SYMB},

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				if (rules[i].token_type == NOTYPE) continue;
				bool flag = false;	
				switch (rules[i].token_type){
					case '+':flag = true;break;
					case '-':flag = true;break;
					case '*':flag = true;break;
					case '/':flag = true;break;
					case EQ:flag = true;break;
					case NEQ:flag = true;break;
					case '&':flag = true;break;
					case '|':flag = true;break;
					case NUM:flag = true;break;
					case SIXTEENUM:flag = true;break;
					case REG:flag = true;break;
					case '!':flag = true;break;
					case '(':flag = true;break;
					case ')':flag = true;break;
					case SYMB:flag = true;break;
					default:assert(0);
				}
				if (flag){
					tokens[nr_token].type = rules[i].token_type;
					strcpy(tokens[nr_token].str,substr_start);
					nr_token++;
				}
				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

static uint32_t find_dominated_op(int p, int q){
	int op = p;
	int sum_brackets = 0;
	for (int i = p; i <= q;i++){
		if (tokens[i].type == '('){
			sum_brackets++;
			continue;
		}
		if (tokens[i].type == ')'){
			sum_brackets--;
			continue;
		}
		if (sum_brackets != 0) continue;

		if (tokens[i].type == POINT) 
			continue;
		if (tokens[i].type == EQ || tokens[i].type == NEQ || tokens[i].type == '&' || tokens[i].type == '|')
			return i;
		if ((tokens[i].type == '-') && (tokens[i - 1].type == '+' || tokens[i - 1].type == '-' || tokens[i - 1].type == '*' || tokens[i - 1].type == '/'))
			continue;

		if (tokens[i].type == '+' || tokens[i].type == '-')
			op = i;
		if ((tokens[i].type == '*' || tokens[i].type == '/') && (!(tokens[op].type == '+' || tokens[op].type == '-')))
			op = i;
	}

	return op; 
}

static bool check_parentheses(int p, int q){
	if (!(tokens[p].type == '(' && tokens[q].type == ')'))
		return false;
	
	int sum_brackets = 0;
	for (int i = p + 1; i < q; i++){
		if (tokens[i].type == '(')
			sum_brackets++;
		if (tokens[i].type == ')'){
			if (sum_brackets == 0)
				return false;
			sum_brackets--;
		}
	}	
	return (sum_brackets == 0);
}

uint32_t eval(int p, int q){
	if (p > q)
		return false;
	if (p == q){
		int ans = 0;
		if (tokens[p].type == SIXTEENUM)
			sscanf(tokens[p].str,"%x",&ans);
		if (tokens[p].type == NUM)
			sscanf(tokens[p].str,"%d",&ans);
		if (tokens[p].type == SYMB){
			bool flag;
			//printf("%s\n",tokens[p].str);
			ans = look_up_symtab(tokens[p].str,&flag);
			if (!flag) return false;
		}
		if (tokens[p].type == REG){
			tokens[p].str[0] = '%';
			if (strcmp(tokens[p].str,"eip") == 0)
				ans = cpu.eip;
			else 
				for (int i = 0; i < 8; i++){
					if (strcmp(tokens[p].str,reg_names_l[i]) == 0){
						ans = cpu.gpr[i].val;
						break;
					}
				}
		}

		return ans;
	}

	if (check_parentheses(p, q))
		return eval(p + 1, q - 1);
	int op = find_dominated_op(p, q);

	if (op == p && tokens[p].type == POINT)
		return vaddr_read(eval(p + 1,q),SREG_DS,4);
	if (op == p && tokens[p].type == '-')
		return -eval(op + 1,q);
	if (op == p && tokens[p].type == '!')
		return !eval(op + 1,q);
	int val1 = eval(p, op - 1);
	int val2 = eval(op + 1, q);
	switch (tokens[op].type){
		case '+': return val1 + val2;
		case '-': return val1 - val2;
		case '*': return val1 * val2;
		case '/': return val1 / val2;
		case EQ: return val1 == val2;
		case NEQ: return val1 != val2;
		case '&': return val1 & val2;
		case '|': return val1 | val2;
		default:assert(0);
	}
	return 0;
}

uint32_t expr(char *e, bool *success){
	if (!make_token(e)){
		*success = false;
		return 0;
	}
	*success = true;
	for (int i = 0; i < nr_token; i++)
		if (tokens[i].type == '*' && (i == 0 || tokens[i - 1].type == '+' || tokens[i - 1].type == '-' || tokens[i - 1].type == '*' || tokens[i - 1].type == '/'))
			tokens[i].type = POINT;

	int p = 0, q = nr_token - 1;
	return eval(p,q);
}
