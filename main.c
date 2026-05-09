#include <stdio.h>
#include "waveform.h"
#include "io.h"
#include <stdlib.h>

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