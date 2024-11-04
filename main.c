#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "hash.h"

int main() {
    FILE* listing = fopen("../files/listing.txt", "w");
    FILE* bin_dump = fopen("../files/bin_dump.txt", "w");
    FILE* hex_dump = fopen("../files/hex_dump.txt", "w");
    fprintf(listing, "LISTING\n\nPLACE       PROGRAMME          KOP   ADDRESS      CODE\n");
    fprintf(bin_dump, "BIN_DUMP\n\n");
    fprintf(hex_dump, "HEX_DUMP\n\n");
    line_t *a = createLine_t();
    get_line_t(a);
    HashTableNames *names = createHashTableNames(50);
    HashTableMnemo *mnemo = createHashTableMnemo(50);
    int* adresses = (int*)malloc((a->size + 1) * sizeof(int));
    adresses[0] = 0;
    int start_address = 0, end_address = 0;

    insertMnemo(mnemo, "START", 0, 1, 0, 0);
    insertMnemo(mnemo, "END", 0, 1, 0, 0);
    insertMnemo(mnemo, "WORD", 0, 1, 0, 0);
    insertMnemo(mnemo, "RESW", 0, 1, 0, 0);
    insertMnemo(mnemo, "ADD", 0, 0, 0, 0x11);
    insertMnemo(mnemo, "ADDX", 0, 0, 0, 0x12);
    insertMnemo(mnemo, "LDA", 0, 0, 0, 0x21);
    insertMnemo(mnemo, "LDX", 0, 0, 0, 0x22);
    insertMnemo(mnemo, "STA", 0, 0, 0, 0x31);
    insertMnemo(mnemo, "HLT", 0, 0, 0, 0x0f);

    for (int i = 0; i < a->size; i++) {
        if (strcmp(a->entries[i]->dir, "START") == 0) {
            adresses[i + 1] = strtol(a->entries[i]->oper, NULL, 16);
            start_address = adresses[i + 1];
        } else if (strcmp(a->entries[i]->dir, "END") == 0) {
            adresses[i + 1] = adresses[i] + 4;
            end_address = adresses[i + 1];
        } else if (!strcmp(a->entries[i]->dir, "WORD"))  {
            adresses[i + 1] = adresses[i] + 3;
            insertNames(names, a->entries[i]->mark, adresses[i]+3, 1, 0);
        } else if (strcmp(a->entries[i]->mark, "")) {
            insertNames(names, a->entries[i]->mark, adresses[i], 1, 0);
            adresses[i + 1] = adresses[i] + 3;
        }
        else adresses[i + 1] = adresses[i] + 3;

    }
    for (int i = 0; i < a->size; i++) {
        if (!a->entries[i]->k) {
            HashEntryMnemo *mn = searchMnemo(mnemo, a->entries[i]->dir);
            HashEntryNames *nm = searchNames(names, a->entries[i]->oper);
            fprintf(listing, " %04x %6s %6s %6s \t\t%02x \t\t%04x \t%02x%04x\n", adresses[i], a->entries[i]->mark, a->entries[i]->dir, a->entries[i]->oper, mn->KOP, nm->address, mn->KOP, nm->address);
            fprintf(bin_dump, "%02x%04x", mn->KOP, nm->address);
            fprintf(hex_dump, ":03%04x00%02x%04xXX\n", adresses[i], mn->KOP, nm->address);
        }
        else if (!strcmp(a->entries[i]->dir, "HLT")) {
            HashEntryMnemo *mn = searchMnemo(mnemo, a->entries[i]->dir);
            fprintf(listing, " %04x %6s %6s %6s \t\t%02x \t\t%04x \t%02x%04x\n", adresses[i], a->entries[i]->mark, a->entries[i]->dir, a->entries[i]->oper, mn->KOP, 0, mn->KOP, 0);
            fprintf(bin_dump, "%02x%04x", mn->KOP, 0);
            fprintf(hex_dump, ":03%04x00%02x%04xXX\n", adresses[i], mn->KOP, 0);
        }
        else if (!strcmp(a->entries[i]->dir, "END")) {
            fprintf(listing, " %04x %6s %6s %6s\n", adresses[i], a->entries[i]->mark, a->entries[i]->dir, a->entries[i]->oper);
            fprintf(hex_dump, ":00%04x01XX", start_address);
        }
        else if (!strcmp(a->entries[i]->dir, "START")) {
            fprintf(listing, " %04x %6s %6s %6s\n", adresses[i], a->entries[i]->mark, a->entries[i]->dir, a->entries[i]->oper);
            fprintf(hex_dump, ":02%04x02%04xXX\n", start_address, end_address-start_address);
        }
        else {
            fprintf(listing, " %04x %6s %6s %6s \t\t%2s \t\t%4s \t%06x\n", adresses[i], a->entries[i]->mark, a->entries[i]->dir,
                    a->entries[i]->oper, "", "", strtol(a->entries[i]->oper, NULL, 10));
            fprintf(bin_dump, "%06x", strtol(a->entries[i]->oper, NULL, 10));
            fprintf(hex_dump, ":03%04x00%06xXX\n", adresses[i], strtol(a->entries[i]->oper, NULL, 10));
        }
    }

    fprintf(listing, "\nSTART ADDRESS: %xh \tSIZE OF PROGRAMME: %xh\n", start_address, end_address - start_address);
    fprintf(listing, "\n\t\t\tNames table\n%12s %12s %12s\n", "NAME", "SIGH", "ADDRESS");
    printSortedHashTableNames(listing, names);
    fprintf(listing, "\n\t\t\tMnemonic table\n%12s %12s %12s\n", "MNEMO", "COMMAND/DIR", "KOP");
    printSortedHashTableMnemo(listing, mnemo);

    freeHashTableMnemo(mnemo);
    freeHashTableNames(names);
    free(adresses);
    freeLineTable(a);
    fclose(listing);
    fclose(bin_dump);
    fclose(hex_dump);
    return 0;
}