#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "waveform.h"


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

static double get_voltage(waveform *s, int phase) {
    if (phase == 0) return s->phase_A_voltage;
    if (phase == 1) return s->phase_B_voltage;
    return s->phase_C_voltage;
}

double compute_rms(waveform *samples, int n, int phase) {
    double sum_sq = 0.0;
    waveform*ptr = samples;
    waveform *end = samples + n;
    while (ptr < end) {
        double v = get_voltage(ptr, phase);
        sum_sq += v * v;
        ptr++;
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


    double rms_A = compute_rms(samples, n, 0);
    double rms_B = compute_rms(samples, n, 1);
    double rms_C = compute_rms(samples, n, 2);

    printf("Phase A RMS: %.2f V\n", rms_A);
    printf("Phase B RMS: %.2f V\n", rms_B);
    printf("Phase C RMS: %.2f V\n", rms_C);
    free(samples);
    samples = NULL;

    return 0;
}