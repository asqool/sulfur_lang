#ifndef SYSCALL_H
#define SYSCALL_H

#include <type.h>

#define WATFUNC_ADDR 0x1ffff7

#define SERIAL_PORT_A 0x3F8
#define SERIAL_PORT_B 0x2F8

#define UNUSED(x) (void)(x)
#define ARYLEN(x) (int)(sizeof(x) / sizeof((x)[0]))

#define c_ckprint(message, color) c_ckprint_at(message, -1, -1, color)
#define c_kprint_rgb(message, color, bg_color) c_rgb_print_at(message, -1, -1, color, bg_color)
#define c_kprint(message) c_ckprint(message, c_white)

#define c_run_ifexist(path, argc, argv) c_run_ifexist_full(path, argc, argv, 0, 0, 0)

// nothing better than shit code art
#define hi_func_addr(id) ((int (*)(int)) *(int *)WATFUNC_ADDR)(id)

#define KB_A 16
#define KB_Z 17
#define KB_E 18
#define KB_R 19
#define KB_T 20
#define KB_Y 21
#define KB_U 22
#define KB_I 23
#define KB_O 24
#define KB_P 25
#define KB_Q 30
#define KB_S 31
#define KB_D 32
#define KB_F 33
#define KB_G 34
#define KB_H 35
#define KB_J 36
#define KB_K 37
#define KB_L 38
#define KB_M 39
#define KB_N 49
#define KB_W 44
#define KB_X 45
#define KB_C 46
#define KB_V 47
#define KB_B 48

#define KB_ESC 1
#define KB_ALT 56
#define KB_CTRL 29
#define KB_SHIFT 42
#define KB_MAJ 58
#define KB_TAB 15

// keyboard scancodes
#define SC_MAX 57

#define LSHIFT 42
#define RSHIFT 54
#define LEFT 75
#define RIGHT 77
#define OLDER 72
#define NEWER 80
#define BACKSPACE 14
#define DEL 83
#define ENTER 28

#define KB_released_value 128
#define KB_released(key) (key + KB_released_value)

#define c_blue      0x09
#define c_green     0x0a
#define c_cyan      0x0b
#define c_red       0x0c
#define c_magenta   0x0d
#define c_yellow    0x0e
#define c_grey      0x07
#define c_white     0x0f

#define c_dblue     0x01
#define c_dgreen    0x02
#define c_dcyan     0x03
#define c_dred      0x04
#define c_dmagenta  0x05
#define c_dyellow   0x06
#define c_dgrey     0x08


#define c_fs_get_used_sectors ((uint32_t (*)(void)) hi_func_addr(0))
#define c_fs_get_sector_count ((uint32_t (*)(void)) hi_func_addr(1))
#define c_fs_get_element_name ((void (*)(uint32_t, char *)) hi_func_addr(2))
#define c_fs_make_dir ((uint32_t (*)(char *, char *)) hi_func_addr(3))
#define c_fs_make_file ((uint32_t (*)(char *, char *)) hi_func_addr(4))
#define c_fs_read_file ((void (*)(char *, uint8_t *)) hi_func_addr(5))
#define c_fs_write_in_file ((void (*)(char *, uint8_t *, uint32_t)) hi_func_addr(6))
#define c_fs_get_file_size ((uint32_t (*)(char *)) hi_func_addr(7))
#define c_fs_get_dir_size ((int (*)(char *)) hi_func_addr(8))
#define c_fs_declare_read_array ((void *(*)(char *)) hi_func_addr(9))
#define c_fs_does_path_exists ((int (*)(char *)) hi_func_addr(10))
#define c_fs_get_sector_type ((int (*)(uint32_t)) hi_func_addr(11))
#define c_fs_get_dir_content ((void (*)(char *, uint32_t *)) hi_func_addr(12))
#define c_fs_path_to_id ((uint32_t (*)(char *)) hi_func_addr(13))
#define c_fs_delete_file ((void (*)(char *)) hi_func_addr(14))
#define c_fs_delete_dir ((void (*)(char *)) hi_func_addr(15))

#define c_mem_get_alloc_size ((uint32_t (*)(uint32_t)) hi_func_addr(16))
#define c_mem_alloc ((uint32_t (*)(uint32_t, uint32_t, int)) hi_func_addr(17))
#define c_mem_free_addr ((int (*)(uint32_t)) hi_func_addr(18))
#define c_mem_get_info ((int (*)(int, int)) hi_func_addr(19))
#define c_mem_free_all ((int (*)(int)) hi_func_addr(20))

#define c_time_get ((void (*)(i_time_t *)) hi_func_addr(21))
#define c_timer_get_ms ((uint32_t (*)(void)) hi_func_addr(22))

#define c_font_get ((uint8_t *(*)(int)) hi_func_addr(23))
#define c_clear_screen ((void (*)(void)) hi_func_addr(24))
#define c_ckprint_at ((void (*)(char *, int, int, char)) hi_func_addr(25))
#define c_kprint_backspace ((void (*)(void)) hi_func_addr(26))
#define c_set_cursor_offset ((void (*)(int)) hi_func_addr(27))
#define c_get_cursor_offset ((int (*)(void)) hi_func_addr(28))
#define c_gt_get_max_rows ((int (*)(void)) hi_func_addr(29))
#define c_gt_get_max_cols ((int (*)(void)) hi_func_addr(30))
#define c_cursor_blink ((void (*)(int)) hi_func_addr(31))
#define c_vesa_set_pixel ((void (*)(int, int, uint32_t)) hi_func_addr(32))
#define c_vesa_get_width ((uint32_t (*)(void)) hi_func_addr(33))
#define c_vesa_get_height ((uint32_t (*)(void)) hi_func_addr(34))

#define c_kb_scancode_to_char ((char (*)(int, int)) hi_func_addr(35))
#define c_kb_get_scancode ((int (*)(void)) hi_func_addr(36))
#define c_kb_reset_history ((void (*)(void)) hi_func_addr(37))
#define c_kb_get_scfh ((int (*)(void)) hi_func_addr(38))

#define c_sys_reboot ((void (*)(void)) hi_func_addr(39))
#define c_sys_shutdown ((void (*)(void)) hi_func_addr(40))
#define c_run_ifexist_full ((int (*)(char *, int, char **, uint32_t, uint32_t, uint32_t)) hi_func_addr(41))
#define c_sys_kinfo ((void (*)(char *)) hi_func_addr(42))

#define c_serial_print ((void (*)(int, char *)) hi_func_addr(43))
#define c_mouse_call ((int (*)(int, int)) hi_func_addr(44))

#define c_ramdisk_get_size ((uint32_t (*)(void)) hi_func_addr(45))
#define c_ramdisk_get_used ((uint32_t (*)(void)) hi_func_addr(46))

#define c_process_create ((int (*)(void (*func)(), int, char *)) hi_func_addr(47))
#define c_process_sleep ((void (*)(int, uint32_t)) hi_func_addr(48))
#define c_process_wakeup ((void (*)(int)) hi_func_addr(49))
#define c_process_kill ((void (*)(int)) hi_func_addr(50))
#define c_process_get_pid ((int (*)(void)) hi_func_addr(51))
#define c_process_get_ppid ((int (*)(int)) hi_func_addr(52))
#define c_process_generate_pid_list ((int (*)(int *, int)) hi_func_addr(53))
#define c_process_get_name ((int (*)(int, char *)) hi_func_addr(54))
#define c_process_get_state ((int (*)(int)) hi_func_addr(55))
#define c_process_get_run_time ((uint32_t (*)(int)) hi_func_addr(56))

#define c_exit_pid ((int (*)(int)) hi_func_addr(57))

#endif