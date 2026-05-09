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

/* --- write results to results.txt --- */
void write_results(const char *data, waveform *samples, int n) {
    FILE *fp = fopen(data, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not write %s\n", data);
        return;
    }

    fprintf(fp, "=== Power Quality Report ===\n\n");

    char *names[] = {"A", "B", "C"};
    for (int ph = 0; ph < 3; ph++) {
        double rms  = compute_rms(samples, n, ph);
        double pp   = compute_peak_to_peak(samples, n, ph);
        double dc   = compute_dc_offset(samples, n, ph);
        int    clip = count_clipped(samples, n, ph);

        fprintf(fp, "Phase %s:\n", names[ph]);
        fprintf(fp, "  RMS voltage    : %.2f V  [%s]\n", rms,
                check_compliance(rms) ? "COMPLIANT" : "OUT OF TOLERANCE");
        fprintf(fp, "  Peak-to-peak   : %.2f V\n", pp);
        fprintf(fp, "  DC offset      : %.4f V\n", dc);
        fprintf(fp, "  Clipped samples: %d\n\n", clip);
    }

    fclose(fp);
}