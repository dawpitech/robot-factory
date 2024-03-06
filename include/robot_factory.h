/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** robot header
*/

#ifndef ROBOT_FACTORY_ROBOT_FACTORY_H
    #define ROBOT_FACTORY_ROBOT_FACTORY_H
    #include "op.h"
    #include <bits/types/FILE.h>
    #define RET_ERROR 84
    #define RET_VALID 0
    #define MAGIC_BYTES (-209458688)
typedef enum {
    NA,
    COMMENT,
    NAME
} raw_str_type_e;
typedef struct {
    char *command;
    char *label;
} line_t;
typedef enum {
    DIRECT = 0x2,
    INDIRECT = 0x3,
    REGISTER = 0x1,
    PADDING = 0x0
} arg_type_e;
typedef struct {
    char *data;
    arg_type_e type;
} arg_t;
typedef struct label_s {
    char *name;
    unsigned int address;
    int is_idx;
    struct label_s *next;
} label_t;
typedef struct {
    char *buffer;
    int line_nb;
    unsigned int buffer_size;
    FILE *output_file;
    label_t *labels;
    label_t *labels_tolink;
    header_t *header;
    line_t *line;
} assm_cfg_t;
int compile_line(op_t *operation, arg_t *args, assm_cfg_t *assm_cfg);
int robot_factory(int argc, char **argv);
int parse_file(char *file_path, assm_cfg_t *assm_cfg);
int write_byte(char byte, assm_cfg_t *assm_cfg);
void write_bytes(int num, int byte_nb, assm_cfg_t *assm_cfg);
int write_to_header(char *str, assm_cfg_t *assm_cfg, raw_str_type_e type);
int write_buff_to_output(assm_cfg_t *assm_cfg);
int write_header_to_output(assm_cfg_t *assm_cfg);
void compute_coding_byte(arg_t *args, assm_cfg_t *assm_cfg);
int link_labels(assm_cfg_t *assm_cfg);
void add_to_label(int address, char *label, label_t **node);
int compute_arguments(arg_t *args, assm_cfg_t *assm_cfg, int idx);
#endif //ROBOT_FACTORY_ROBOT_FACTORY_H
