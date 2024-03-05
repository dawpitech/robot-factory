/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** compiler header
*/

#include <endian.h>
#include <stdio.h>
#include <sys/types.h>

#include "op.h"
#include "robot_factory.h"
#include "my.h"

void compute_coding_byte(arg_t *args, assm_cfg_t *assm_cfg)
{
    u_int8_t coding_byte = 0;

    for (int i = 0; i < MAX_ARGS_NUMBER; i += 1)
        coding_byte = (coding_byte << 2) | args[i].type;
    write_byte(coding_byte, assm_cfg);
}

int compute_arguments(arg_t *args, assm_cfg_t *assm_cfg, int idx)
{
    int nb = 0;

    for (int i = 0; i < MAX_ARGS_NUMBER && args[i].data != NULL; i += 1) {
        nb = my_getnbr(args[i].data);
        switch ((int)args[i].type) {
        case DIRECT:
            write_bytes(nb, idx ? IND_SIZE : DIR_SIZE, assm_cfg);
            break;
        case REGISTER:
            write_bytes(nb, 1, assm_cfg);
            break;
        case INDIRECT:
            write_bytes(nb, IND_SIZE, assm_cfg);
            break;
        default:
            return RET_ERROR;
        }
    }
    return RET_VALID;
}

int compile_line(op_t *operation, arg_t *args, assm_cfg_t *assm_cfg)
{
    int can_be_index = (operation->code == 8 || operation->code == 9 ||
        operation->code == 10 || operation->code == 11 ||
        operation->code == 13 || operation->code == 14);

    write_byte(operation->code, assm_cfg);
    if (!(operation->code == 1 || operation->code == 9 || operation->code == 12
        || operation->code == 15))
        compute_coding_byte(args, assm_cfg);
    if (compute_arguments(args, assm_cfg, can_be_index))
        return RET_ERROR;
    return RET_VALID;
}
