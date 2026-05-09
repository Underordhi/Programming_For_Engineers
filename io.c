#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"


int count_rows(const char *data) {
    FILE *fp = fopen(data, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open %s\n", data);
        return -1;
    }
    char line[512];
    int count = 0;
    fgets(line, sizeof(line), fp);   /* skip header */
    while (fgets(line, sizeof(line), fp) != NULL) {
        count++;
    }
    fclose(fp);
    return count;
}
