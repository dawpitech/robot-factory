/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** toolbox header
*/

#ifndef ROBOT_FACTORY_TOOLBOX_H
    #define ROBOT_FACTORY_TOOLBOX_H
    #define NOT_NEG(x) ((x) < 0 ? 0 : (x))
char *strip_label(char *ptr, char **label_str);
void my_memcpy(void *dest, void *src, size_t size);
char *my_strchr(char const *str, char token);
void remove_commentaries(char *ptr);
char *extract_from_quotes(char const *input);
char *my_strtok(char *str, char separator);
char *my_strtok_2(char *str, char *separator);
#endif //ROBOT_FACTORY_TOOLBOX_H
