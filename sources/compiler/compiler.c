/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** compiler header
*/

#include <stdio.h>
#include <sys/types.h>

#include "robot_factory.h"

void compute_coding_byte(arg_list_t *args, assm_cfg_t *assm_cfg)
{
    u_int8_t coding_byte = 0;

    for (int i = 0; i < MAX_ARGS_NUMBER; i += 1)
        coding_byte = (coding_byte << 2) | args[i].type;
    write_byte(coding_byte, assm_cfg);
}

int compile_line(op_t *operation, arg_list_t *args, assm_cfg_t *assm_cfg)
{
    write_byte(operation->code, assm_cfg);
    if (!(operation->code == 1 || operation->code == 9 || operation->code == 12
        || operation->code == 15))
        compute_coding_byte(args, assm_cfg);
    write_byte(0xff, assm_cfg);
    return RET_VALID;
}
