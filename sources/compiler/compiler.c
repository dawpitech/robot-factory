/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** compiler header
*/

#include <stdio.h>
#include <sys/types.h>

#include "op.h"
#include "robot_factory.h"
#include "my.h"

void compute_coding_byte(arg_list_t *args, assm_cfg_t *assm_cfg)
{
    u_int8_t coding_byte = 0;

    for (int i = 0; i < MAX_ARGS_NUMBER; i += 1)
        coding_byte = (coding_byte << 2) | args[i].type;
    write_byte(coding_byte, assm_cfg);
}

u_int8_t get_bytes(int num, int byteNumber)
{
    u_int8_t result = 0;

    if (byteNumber < 1)
        return 0;
    for (int i = 0; i < byteNumber; ++i) {
        result |= (u_int8_t)(num & 0xFF) << (8 * i);
        num >>= 8;
    }
    return result;
}

int compute_arguments(arg_list_t *args, assm_cfg_t *assm_cfg)
{
    u_int8_t curr_arg = 0;
    int nb = 0;

    for (int i = 0; i < MAX_ARGS_NUMBER; i += 1) {
        if (args[i].data == NULL)
            continue;
        nb = my_getnbr(args[i].data);
        switch ((int)args[i].type) {
        case DIRECT:
            curr_arg = get_bytes(nb, DIR_SIZE);
            break;
        case REGISTER:
            curr_arg = get_bytes(nb, REG_SIZE);
            break;
        case INDIRECT:
            curr_arg = get_bytes(nb, IND_SIZE);
        }
        write_byte(curr_arg, assm_cfg);
    }
    return 0;
}

int compile_line(op_t *operation, arg_list_t *args, assm_cfg_t *assm_cfg)
{
    write_byte(operation->code, assm_cfg);
    if (!(operation->code == 1 || operation->code == 9 || operation->code == 12
        || operation->code == 15))
        compute_coding_byte(args, assm_cfg);
    if (compute_arguments(args, assm_cfg))
        return RET_ERROR;
    //write_byte(0xff, assm_cfg);
    return RET_VALID;
}
