/*
** EPITECH PROJECT, 2024
** robot-factory

T
** File description:
** tests_writer header
*/

#include <criterion/criterion.h>
#include <stdio.h>

#include "../include/robot_factory.h"

Test(unit, writer) {
    assm_cfg_t cfg = {0};
    write_byte((char) 112, &cfg);
    write_byte((char) 23, &cfg);
    cr_expect_eq(cfg.buffer_size, 2);
    cr_expect_eq(cfg.buffer[0], (char) 112);
    cr_assert_eq(cfg.buffer[1], (char) 23);
}

Test(unit, writer_multiples_bytes) {
    assm_cfg_t cfg= {0};
    write_bytes((u_int16_t) 1234, 2, &cfg);
    cr_expect_eq(cfg.buffer_size, 2);
    cr_expect_eq(cfg.buffer[0], (u_int8_t) 210);
    printf("%x %x\n", cfg.buffer[0], cfg.buffer[1]);
    cr_assert_eq(cfg.buffer[1], (u_int8_t) 4);
}
