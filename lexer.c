#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "lexer.h"

#define MAX_LINE_LENGTH 256
#define COM_POSITION 50
#define NUM_MATCHES 5

line_t * createLine_t() {
    line_t *table = (line_t *)malloc(sizeof(line_t));
    if (table == NULL) {
        perror("Failed to allocate memory for line_t");
        exit(EXIT_FAILURE);
    }
    table->size = 0;
    table->entries = NULL;
    return table;
}

void add_entry(line_t* table, char* mark, char* dir, char* oper, char* comm, int k) {
    line_e* entry = (line_e*)malloc(sizeof(line_e));
    if (entry == NULL) {
        perror("Failed to allocate memory for line_e");
        exit(EXIT_FAILURE);
    }
    entry->mark = strdup(mark);
    entry->dir = strdup(dir);
    entry->oper = strdup(oper);
    entry->comm = strdup(comm);
    entry->k = k;

    table->entries = (line_e**)realloc(table->entries, sizeof(line_e*) * (table->size + 1));
    if (table->entries == NULL) {
        perror("Failed to allocate memory for entries");
        exit(EXIT_FAILURE);
    }
    table->entries[table->size] = entry;
    table->size++;
}

void remove_last_char(char *str) {
    if (str == NULL || *str == '\0') return;
    size_t len = strlen(str);
    if (len > 0) str[len - 1] = '\0';
}

void pat(line_t *line_table, FILE *out, char *line, regmatch_t *matches, char *suffix_part) {
    char **groups = NULL;
    int num_groups = 0;
    for (int i = 1; i < NUM_MATCHES; i++) {
        if (matches[i].rm_so != -1) {
            int len = matches[i].rm_eo - matches[i].rm_so;
            char *group_text = (char *)malloc((len + 1) * sizeof(char));
            if (group_text == NULL) {
                perror("Failed to allocate memory for group_text");
                exit(EXIT_FAILURE);
            }
            strncpy(group_text, line + matches[i].rm_so, len);
            group_text[len] = '\0';
            groups = (char **)realloc(groups, (num_groups + 1) * sizeof(char *));
            if (groups == NULL) {
                perror("Failed to allocate memory for groups");
                exit(EXIT_FAILURE);
            }
            groups[num_groups] = group_text;
            num_groups++;
        } else {
            groups = (char **)realloc(groups, (num_groups + 1) * sizeof(char *));
            if (groups == NULL) {
                perror("Failed to allocate memory for groups");
                exit(EXIT_FAILURE);
            }
            groups[num_groups] = (char *)malloc(sizeof(char));
            if (groups[num_groups] == NULL) {
                perror("Failed to allocate memory for groups[num_groups]");
                exit(EXIT_FAILURE);
            }
            groups[num_groups][0] = '\0';
            num_groups++;
        }
    }
    remove_last_char(groups[0]);
    fprintf(out, "mark %8s dir %8s oper %8s comm %s\n", groups[0], groups[1], groups[2], suffix_part);

    int k = 0;
    if (!strcmp(groups[1], "START")||!strcmp(groups[1], "END")||!strcmp(groups[1], "WORD")||!strcmp(groups[1], "RESB")||!strcmp(groups[1], "RESW")||!strcmp(groups[1], "HLT"))
        k = 1;
    add_entry(line_table, groups[0], groups[1], groups[2], groups[3], k);

    for (int i = 0; i < num_groups; i++) free(groups[i]);
    free(groups);
}

void get_line_t(line_t *line_table) {
    char input_string[MAX_LINE_LENGTH];
    regex_t pattern;
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    if (in == NULL || out == NULL) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }
    regcomp(&pattern, "^([ ]*[a-zA-Z]*[:])?[ ]*(START|END|LDX|LDA|ADDX|STA|HLT|WORD|RESB|RESW)?[ ]*([a-zA-Z0-9]*)?[ ]*(.*)?$", REG_EXTENDED);
    while (fgets(input_string, MAX_LINE_LENGTH, in) != NULL) {
        input_string[strcspn(input_string, "\n")] = '\0';
        char prefix_part[COM_POSITION + 1];
        strncpy(prefix_part, input_string, COM_POSITION);
        prefix_part[COM_POSITION] = '\0';
        char *suffix_part = input_string + COM_POSITION;
        if (strlen(input_string) <= COM_POSITION) suffix_part = "";
        regmatch_t matches[NUM_MATCHES];
        int p = regexec(&pattern, prefix_part, NUM_MATCHES, matches, 0);
        if (!p) pat(line_table, out, prefix_part, matches, suffix_part);
    }
    fclose(in);
    fclose(out);
    regfree(&pattern);
}

void freeLineTable(line_t * table) {
    if (table == NULL) return;
    for (int i = 0; i < table->size; i++) {
        free(table->entries[i]->mark);
        free(table->entries[i]->dir);
        free(table->entries[i]->oper);
        free(table->entries[i]->comm);
        free(table->entries[i]);
    }
    free(table->entries);
    free(table);
}
