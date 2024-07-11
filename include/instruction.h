#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "token_class.h"
#include "memlib.h"
#include "Ast.h"




struct Instruction;

typedef struct varset{
    char*name;
    char*type;
    struct Ast*val;

}varset;



typedef struct  Elif{
    struct Ast*condition;
    int endif_p;
}Elif;

typedef struct If{
    struct Ast*condition;
    int endif_p;
}If;


typedef struct For{
    int var_name;
    struct Ast*start;
    struct Ast*end;
    S_sulfur_int endfor;
}For;

typedef struct While{
    struct Ast*condition;
    int endwhile;
}While;



typedef struct Func_info{
    char* description;//can be shown with help()
    char* name;
}Func_info;


typedef struct Funcdef_code{
    Func_info info;

    char** args;
    int args_len;
    char args_mod;  // 'o' = only args_len,  '+' = args_len or more

    struct Instruction* code;
    int code_len;
}Funcdef_code;


typedef struct Instruction{
    short int type;
    union {
        varset *vs;
        Funcdef_code *fc;
        If *i;
        For *fo;
        While *wh;
        int endwhile;
        int endfor;
        struct Ast *ret;
        struct Ast *expr;
        int endif;
        int endifelse;
        Elif *el;
        char *section;
        char *goto_sec;
        int jmp;
        Object *obj;
        int jmp_length;
        int op;
        S_ount_t ount;
        int var_idx;
        double floap;
        int for_bc_info[2];
        int str_idx;
    }value;
    int line;
    char facultative; // used by expression / pass
    int real_line;
}Instruction;

enum instruction_type{
    inst_varset_t,
    inst_new_varset_t,//use .vs without setting type
    inst_endif,
    inst_endifelse,
    inst_if_t,
    inst_elif_t,
    inst_else_t,
    inst_expr_t,
    inst_endfor_t,
    inst_endwhile_t,
    inst_for_t,
    inst_while_t,
    inst_return_t,
    inst_swap_t,
    inst_section_t,
    inst_goto_t,
    inst_pass_t,
    inst_funcdef_t,
    inst_proceed_t,
    inst_stop_t,
    inst_jmp_t, // jump to index of instruction
    // instructions of bytecode
    inst_S_op_t,
    inst_S_push_str_t,
    inst_S_push_var_t,
    inst_S_push_global_var_t,
    inst_S_push_1b_t,
    inst_S_push_0b_t,
    inst_S_push_nil_t,
    inst_S_push_i_t,
    inst_S_push_ount_t,
    inst_S_push_floap_t,
    inst_S_pop_t,
    inst_S_op_add_t,
    inst_S_op_sub_t,
    inst_S_op_mul_t,
    inst_S_op_div_t,
    inst_S_op_pow_t,
    inst_S_op_fldiv_t,
    inst_S_op_mod_t,
    inst_S_op_eq_t,
    inst_S_op_ge_t,
    inst_S_op_geq_t,
    inst_S_op_le_t,
    inst_S_op_leq_t,
    inst_S_op_lshift_t,
    inst_S_op_rshift_t,
    inst_S_op_noteq_t,
    inst_S_op_negate_t,
    inst_S_op_not_t,
    inst_S_op_unpack_t,
    inst_S_rjmp_t,
    inst_S_rjmpif_t,
    inst_S_rjmpifn_t,
    inst_S_dot_get_t,
    inst_S_dot_set_t,
    inst_S_col_get_t,
    inst_S_col_set_t,
    inst_S_var_set_t,
    inst_S_global_var_set_t,
    inst_S_prepare_fcall_t,
    inst_S_fcall_t,
    inst_S_fret_t,
    inst_S_ret_t,
    inst_S_call_t,
    inst_S_var_inc_t,
    inst_S_var_dec_t,
    inst_S_global_var_inc_t,
    inst_S_global_var_dec_t,
    inst_S_dot_inc_t,
    inst_S_dot_dec_t,
    inst_S_col_inc_t,
    inst_S_col_dec_t,
    inst_S_nop_t,
    inst_S_dup_t,
    inst_S_for_t, // 2 param (var idx & start idx of loop)
    inst_S_fstart_def_t,
    inst_S_fset_mod_fixed,
    inst_S_fset_mod_packed,
    inst_S_fset_param_len_t,
    inst_S_fadd_param_t,
    inst_S_fset_name_t,
    inst_S_fend_def_t
};

enum instruction_op_stack {
    instOp_add,
    instOp_sub,
    instOp_mul,
    instOp_div,
    instOp_fl_div,
    instOp_negate,
    instOp_not,
    instOp_pow,
};

//to acces an element
//just name.element
//if name==" " just element
typedef struct Lib{
    char*name;

    Funcdef*funcs;
    int nbr_funcs;

    Object*vars;
    char**vars_name;
    int nbr_vars;
}Lib;

int inst_to_str(Instruction*i);



extern Funcdef*FUNCDEFS;
extern long long int FUNCDEFS_len;

extern Lib*LIBS;
extern long long int LIBS_len;


void instruction_free_array(Instruction *code, int len);

void instruction_free(Instruction code);
#endif