#include "../include/instruction.h"
#include "../include/utilities.h"
#include "../include/sulfur.h"
#include "../include/bytecode.h"

char instruction_sizes[] = {
	1,
    9,
    9,
    9,
    1,
    1,
    1,
    1,
    9,
    9,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    9,
    9,
    9,
    9,
    9,
    1,
    1,
    9,
    9,
   	1,
    1,
    1,
    1,
    1,
    9,
    9,
    9,
    9,
    9,
    9,
    1,
    1,
    1,
    1,
    17,
    1,
    1,
    1,
    9,
    9,
    9,
    1
};
char instruction_opcode[] = {
 	0xFF,
    0,
    7,
    8,
    2,
    1,
    3,
    4,
    5,
    6,
    9,
    40,
    41,
   	42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    10,
    11,
    12,
    15,
    13,
    16,
    14,
    16,
    17,
    18,
	19,
    20,
	21,
    22,
    23,
    25,
    24,
    26,
    27,
    29,
    28,
    30,
    0b10101010,
    31,
    32,
    33,
    34,
    35,
    36,
    37,
    38,
    39
};

int get_instruction_size(int type) {
	return instruction_sizes[type - BASE_INSTRUCTION];
}

char get_opcode(int type) {
	return instruction_opcode[type - BASE_INSTRUCTION];
}

/*
jmp +1
INST

-> jmp + 9
	INST

because jmp is of size 9
*/
void make_jumps_relative_to_size(Instruction *code, int len) {
	for (int i = 0; i < len; i++) {
		switch (code[i].type) {
			case inst_S_rjmp_t:
			case inst_S_rjmpif_t:
			case inst_S_rjmpifn_t:
			case inst_S_call_t:
				if (code[i].value.jmp_length > 0) {
					int jmp_size = 0;
					for (int k = 0; k < code[i].value.jmp_length; k++)
						jmp_size += get_instruction_size(code[i + k].type);
					code[i].value.jmp_length = jmp_size;
				}
				if (code[i].value.jmp_length < 0) {
					int jmp_size = 0;
					for (int k = 0; k < -code[i].value.jmp_length; k++)
						jmp_size += get_instruction_size(code[i - k - 1].type);
					code[i].value.jmp_length = jmp_size;
				}
				break;
			default:
				break;
		}
	}
}

void bytes_append_str(uti_Bytes *bytes, char *str, char add_zero) {
	bytes->vals = realloc(bytes->vals, sizeof(unsigned char) * (bytes->len + strlen(str) + 1));
	while (*str) {
		bytes->vals[bytes->len] = *str;
		str++;
		(bytes->len)++;
	}
	if (!add_zero)
		return ;
	bytes->vals[bytes->len] = '\0';
	str++;
	(bytes->len)++;
}

void bytes_append_u8(uti_Bytes *bytes, char val) {
	bytes->vals = realloc(bytes->vals, sizeof(unsigned char) * (bytes->len + 1));
	bytes->vals[bytes->len] = val;
	(bytes->len)++;
}

void bytes_append_u64(uti_Bytes *bytes, int val) {
	bytes->vals = realloc(bytes->vals, sizeof(unsigned char) * (bytes->len + 8));
	for (int i = 0; i < 8; i++) 
		bytes->vals[bytes->len + i] = 0;
	memcpy(&(bytes->vals[bytes->len]), &val, sizeof(int) <= 8 ? sizeof(int) : 8);
	(bytes->len) += 8;
}

void bytes_set_u64_at(uti_Bytes *bytes, int val, int idx) {
	memcpy(&(bytes->vals[idx]), &val, sizeof(int));
}

void bytes_append_double(uti_Bytes *bytes, double val) {
	bytes->vals = realloc(bytes->vals, sizeof(unsigned char) * (bytes->len + 8));
	for (int i = 0; i < 8; i++) 
		bytes->vals[bytes->len + i] = 0;
	memcpy(&(bytes->vals[bytes->len]), &val, sizeof(double) <= 8 ? sizeof(double) : 8);
	(bytes->len) += 8;
}

int get_str_arr_len(char **arr) {
	int len = 0;
	while (*arr != NULL) {
		len++;
		arr++;
	}
	return len;
}

void relink_push_str(Instruction *code, int len, int *strs) {
	for (int i = 0; i < len; i++)
		if (code[i].type == inst_S_push_str_t)
			code[i].value.var_idx = strs[code[i].value.var_idx];
}

uti_Bytes make_bytecode_file(Instruction *code, int len) {

	int *strings_addrs = malloc(CTX.strings_constants_len * sizeof(int));

	int var_left_addr_p = 0;
	int var_right_addr_p = 0;
	int str_constant_addr_p = 0;
	int extra_data_addr_p = 0;
	uti_Bytes res = {.len = 0, .vals = NULL};
	bytes_append_str(&res, BYTECODE_MAGIC_NUM, 0);
	bytes_append_u64(&res, 0);
	bytes_append_u8(&res, BYTECODE_FTYPE_EXE);
	//add time JJ/MM/YYYY|h/m:s
	bytes_append_str(&res, "FAKE_TIME", 1);
	var_left_addr_p = res.len;
	bytes_append_u64(&res, 0);
	var_right_addr_p = res.len;
	bytes_append_u64(&res, 0);
	str_constant_addr_p = res.len;
	bytes_append_u64(&res, 0);
	extra_data_addr_p = res.len;
	bytes_append_u64(&res, 0);

	//add requested vars left
	bytes_set_u64_at(&res, res.len, var_left_addr_p);
	{
		int var_len = get_str_arr_len(CTX.requested_vars);
		bytes_append_u64(&res, var_len);
		for (int i = 0; i < var_len; i++)
			bytes_append_str(&res, CTX.requested_vars[i], 1);
	}
	bytes_set_u64_at(&res, res.len, var_right_addr_p);
	{
		int var_len = get_str_arr_len(CTX.requested_vars_right);
		bytes_append_u64(&res, var_len);
		for (int i = 0; i < var_len; i++)
			bytes_append_str(&res, CTX.requested_vars_right[i], 1);
	}
	bytes_set_u64_at(&res, res.len, str_constant_addr_p);
	{
		bytes_append_u64(&res, CTX.strings_constants_len);
		for (int i = 0; i < CTX.strings_constants_len; i++) {
			strings_addrs[i] = res.len;
			bytes_append_str(&res, CTX.strings_constants[i], 1);
		}
	}
	
	bytes_set_u64_at(&res, res.len, strlen(BYTECODE_MAGIC_NUM));
	relink_push_str(code, len, strings_addrs);
	bytes_append_code(&res, code, len);
	bytes_append_u8(&res, inst_S_push_nil_t);
	bytes_append_u8(&res, inst_S_ret_t);

	bytes_set_u64_at(&res, res.len, extra_data_addr_p);
	{
		bytes_append_u64(&res, 1);
		bytes_append_str(&res, "LINES", 1);
		bytes_append_u64(&res, (CTX.addr_line_len - 1) * (8 * 2));
		for (int i = 0; i < CTX.addr_line_len; i++) {
			//printf("[%x %d]\n", CTX.addr_line[i].a, CTX.addr_line[i].b);
			bytes_append_u64(&res, CTX.addr_line[i].a);
			bytes_append_u64(&res, CTX.addr_line[i].b);
		}
	}

	free(strings_addrs);
	return res;
}

void bytes_append_code(uti_Bytes *res, Instruction *code, int len) {
	int total_size = 0;
	for (int i = 0; i < len; i++) {
		if (code[i].type == inst_S_op_t)
			total_size += get_instruction_size(code[i].value.ount);
		else
			total_size += get_instruction_size(code[i].type);
	}
	res->vals = realloc(res->vals, sizeof(unsigned char) * (res->len + total_size));
	int last_line = -1;
	for (int i = 0; i < len; i++) {
		Instruction current = code[i];
		if (current.real_line > 0) {
			if (current.real_line != last_line) {
				last_line = current.real_line;
				CTX.addr_line_len++;
				CTX.addr_line = realloc(CTX.addr_line, sizeof(int_pairs) * CTX.addr_line_len);
				CTX.addr_line[CTX.addr_line_len - 1].a = res->len;
				CTX.addr_line[CTX.addr_line_len - 1].b = last_line;
			}
		}
		if (current.type == inst_S_op_t) {
			res->vals[res->len] = get_opcode(current.value.op);
			(res->len)++;
			continue;
		}
		int current_size = get_instruction_size(current.type);
		if (current_size == 1) {
			res->vals[res->len] = get_opcode(current.type);
			(res->len)++;
		}
		else if (current_size == 9) {
			res->vals[res->len] = get_opcode(current.type);
			(res->len)++;
			if (current.type == inst_S_push_floap_t) {
				for (int i = 0; i < 8; i++) 
					res->vals[res->len + i] = 0;
				memcpy(&(res->vals[res->len]), &current.value.floap, sizeof(double) <= 8 ? sizeof(double) : 8);
				(res->len) += 8;
			}
			else {
				for (int i = 0; i < 8; i++) 
					res->vals[res->len + i] = 0;
				memcpy(&(res->vals[res->len]), &current.value.ount, sizeof(int) <= 8 ? sizeof(int) : 8);
				(res->len) += 8;
			}
		}
		else if (current_size == 17) {
			//normally on for
			res->vals[res->len] = get_opcode(current.type);
			(res->len)++;
			bytes_append_u64(res, current.value.for_bc_info[0]);
			bytes_append_u64(res, current.value.for_bc_info[1]);
		}
		else {
			PRINT_ERR("ERROR during convertion bytecode to file instruction wrong size\n");
			exit(1);
		}
	}
}



uti_Bytes compile_to_bytecode(char *text, char *path, sulfur_args_t *args) {
    make_context();

	Token *tokens = lexe(text);
    int tokens_len = token_len(tokens);

	if (args->show_lexe)
        tokens_print(tokens, "\n");
	
	tokens = make_include(tokens, &tokens_len, args->filepath);
    CTX.max_line = tokens[tokens_len - 1].line + 1;

	if (args->show_lexe_include)
        tokens_print(tokens, "\n");

	int instruction_len = 0;
    Instruction *code = parse(tokens, -1, -1, NULL, &instruction_len);

    if (args->show_parse_ast) {
        instructions_print(code, instruction_len);
    }

	if (args->show_parse) {
        if (CTX.requested_vars[0] != NULL) DEBUG("req vars left\n");
        for(int i = 0; CTX.requested_vars[i] != NULL; i++) {
            DEBUG("    %d : %s\n", i, CTX.requested_vars[i]);
        }
    }
    if (args->show_parse) {
        if (CTX.requested_vars_right[0] != NULL) DEBUG("req vars right\n");
        for(int i = 0; CTX.requested_vars_right[i] != NULL; i++) {
            DEBUG("    %d : %s\n", i, CTX.requested_vars_right[i]);
        }
    }
	code = make_jmp_links(code, instruction_len);
    Instruction *old_code = code;
    int old_cold_len = instruction_len;
    code = finish_instrcutions(code, &instruction_len);

	if (args->show_parse) {
        if (CTX.strings_constants_len != 0) DEBUG("string constants\n");
        for(int i = 0; i < CTX.strings_constants_len; i++) {
            DEBUG("    %d : %s\n", i, CTX.strings_constants[i]);
        }
    }

	instruction_free_array(old_code, old_cold_len);

    if (args->show_parse) {
        instructions_print(code, instruction_len);
    }

    uti_Bytes bytecode = make_bytecode_file(code, instruction_len);

	for (int i = 0; i < CTX.strings_constants_len; i++) {
        free(CTX.strings_constants[i]);
    }
    free(CTX.strings_constants);
    free(CTX.addr_line);

	for(int i = 0; i < tokens_len; i++){
        free_tok_val(tokens[i]);
    }
    instruction_free_array(code, instruction_len);
    free(tokens);

	for (int i = 0; CTX.requested_vars[i] != NULL; i++) {
        free(CTX.requested_vars[i]);
    }
    free(CTX.requested_vars);
    for (int i = 0; CTX.requested_vars_right[i] != NULL; i++) {
        free(CTX.requested_vars_right[i]);
    }
    free(CTX.requested_vars_right);
	return bytecode;
}