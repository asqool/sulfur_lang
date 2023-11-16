#ifndef SULFUR_H
#define SULFUR_H

#include "memlib.h"

#define VERSION "2.8"
#define SUB_VERSION "0000"
#define COMPLETE_VERSION (VERSION "." SUB_VERSION)

typedef struct {
    int show_mem;
    int show_parse;
    int show_lexe;
    int show_lexe_include;
    int make_bytecode;

    int help;
    int version;

    char *filepath;
} sulfur_args_t;

sulfur_args_t *parse_main_args(int argc, char** argv);
int interactive_shell(sulfur_args_t *args);

extern Sulfur_ctx CTX;

extern void **DYN_LIBS;
extern int DYN_LIBS_COUNT;

void init_dyn_libs();

void add_dyn_lib(void *lib);

void free_dyn_libs();

void set_errno(int is_error, int type, int err_num, int extra);

#endif
