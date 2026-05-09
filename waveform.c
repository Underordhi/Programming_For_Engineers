#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"


int count_rows(const char *data) {
    FILE *fp = fopen(data, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open %s\n", data);
        return -1;
    }

    int count = 0;
    char line[256];

    fgets(line, sizeof(line), fp);   /* skip the header row */

    while (fgets(line, sizeof(line), fp) != NULL) {
        count++;
    }

    fclose(fp);
    return count;
}

int count_rows(const char *data);
void load_csv(const char *data, waveform *samples, int n);
double compute_rms(waveform *samples, int n);

void load_csv(const char *data, waveform *samples, int n) {
    FILE *fp = fopen(data, "r");
    char line[512];
    fgets(line, sizeof(line), fp);

    for (int i = 0; i < n; i++) {
        fscanf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
            &samples[i].timestamp,
            &samples[i].phase_A_voltage,
            &samples[i].phase_B_voltage,
            &samples[i].phase_C_voltage,
            &samples[i].line_current,
            &samples[i].frequency,
            &samples[i].power_factor,
            &samples[i].thd_percent);
    }
    fclose(fp);
}

double compute_rms(waveform *samples, int n) {
    double sum_sq = 0.0;
    for (int i = 0; i < n; i++) {
        sum_sq += samples[i].phase_A_voltage * samples[i].phase_A_voltage;
    }
    return sqrt(sum_sq / n);
}


int main(void) {
    const char *data = "power_quality_log.csv";

    /* --- 1. count rows --- */
    int n = count_rows(data);
    if (n <= 0) {
        fprintf(stderr, "Error: no data rows found.\n");
        return 1;
    }
    printf("Rows found: %d\n", n);


    waveform *samples = malloc(n * sizeof(waveform));
    if (samples == NULL) {
        fprintf(stderr, "Error: malloc failed — out of memory.\n");
        return 1;
    }


    load_csv(data, samples, n);


    double rms_A = compute_rms(samples, n);
    printf("Phase A RMS: %.2f V\n", rms_A);


    free(samples);
    samples = NULL;

    return 0;
}