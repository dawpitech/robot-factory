/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** compiler header
*/

#include <assert.h>
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

static
long add_to_label_maybe(arg_t *args, assm_cfg_t *assm_cfg, int idx, int addr)
{
    if (my_strstr(args->data, ":")) {
        add_to_label(addr, my_strdup(args->data),
            &assm_cfg->labels_tolink, assm_cfg->buffer_size);
        assm_cfg->labels_tolink->is_idx = idx;
    }
    return my_getnbr(args->data);
}

static
op_t *get_op(char *ins_)
{
    static char *ins = NULL;

    if (ins_ != NULL)
        ins = ins_;
    for (int i = 0; op_tab[i].comment != 0; i += 1)
        if (my_strcmp(op_tab[i].mnemonique, ins) == 0)
            return &op_tab[i];
    return NULL;
}

static
int check_ins(long nb, arg_t arg, int i)
{
    op_t *op = get_op(NULL);

    if ((i < op->nbr_args) && nb == -133769420)
        return my_put_stderr("Wrong number of arguments.\n");
    if (nb == -133769420)
        return RET_VALID;
    if (my_strlen(arg.data) == 1 && arg.data[0] == DIRECT_CHAR)
        return my_put_stderr("Invalid argument.\n");
    if (((i + 1) > op->nbr_args))
        return my_put_stderr("Wrong number of arguments.\n");
    if (!(arg.type_e & op->type[i]))
        return my_put_stderr("Invalid argument.\n");
    if ((nb < 1 || nb > REG_NUMBER) && arg.type == REGISTER)
        return my_put_stderr("Invalid register number.\n");
    return RET_VALID;
}

int compute_arguments(arg_t *args, assm_cfg_t *assm_cfg, int idx, int addr)
{
    long nb = 0;
    int i = 0;

    for (i = 0; i < MAX_ARGS_NUMBER && args[i].data != NULL; i += 1) {
        nb = add_to_label_maybe(&args[i], assm_cfg, idx, addr);
        if (check_ins(nb, args[i], i) == RET_ERROR)
            return RET_ERROR;
        switch ((int)args[i].type) {
        case DIRECT:
            write_bytes(nb, idx ? IND_SIZE : DIR_SIZE, assm_cfg);
            break;
        case REGISTER:
            write_bytes(nb, REG_PARAM_SIZE, assm_cfg);
            break;
        case INDIRECT:
            write_bytes(nb, IND_SIZE, assm_cfg);
            break;
        }
    }
    return check_ins(-133769420, args[0], i);
}

int compile_line(op_t *operation, arg_t *args, assm_cfg_t *assm_cfg)
{
    int can_be_index = (operation->code == 9 || operation->code == 10 ||
        operation->code == 11 || operation->code == 12 ||
        operation->code == 14 || operation->code == 15);
    int addr = 0;

    write_byte(operation->code, assm_cfg);
    get_op(operation->mnemonique);
    addr = assm_cfg->buffer_size;
    if (!(operation->code == 1 || operation->code == 9 || operation->code == 12
        || operation->code == 15))
        compute_coding_byte(args, assm_cfg);
    if (compute_arguments(args, assm_cfg, can_be_index, addr))
        return RET_ERROR;
    return RET_VALID;
}
