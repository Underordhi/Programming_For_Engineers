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

void load_csv(const char *data, waveform *samples, int n) {
    FILE *fp = fopen(data, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open %s\n", data);
        return;
    }
    char line[512];
    fgets(line, sizeof(line), fp);   /* skip header */

    waveform *ptr = samples;
    waveform *end = samples + n;
    while (ptr < end && fgets(line, sizeof(line), fp) != NULL) {
        sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
            &ptr->timestamp,
            &ptr->phase_A_voltage,
            &ptr->phase_B_voltage,
            &ptr->phase_C_voltage,
            &ptr->line_current,
            &ptr->frequency,
            &ptr->power_factor,
            &ptr->thd_percent);
        ptr++;
    }
    fclose(fp);
}

