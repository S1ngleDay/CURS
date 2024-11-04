#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#define MAX_LINE_LENGTH 256
#define COM_POSITION 50

typedef struct {
    char* mark;
    char* dir;
    char* oper;
    char* comm;
    int k;
} line_e;

typedef struct {
    line_e** entries;
    int size;
} line_t;

line_t * createLine_t();
void add_entry(line_t* table, char* mark, char* dir, char* oper, char* comm, int k);
void remove_last_char(char *str);
void pat(line_t *line_table, FILE *out, char *line, regmatch_t *matches, char *suffix_part);
void get_line_t(line_t *line_table);
void freeLineTable(line_t * table);
