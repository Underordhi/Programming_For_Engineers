#include <stdio.h>
#include "waveform.h"
#include "io.h"
#include <stdlib.h>

const char *filename = "power_quality_log.csv";

int main(int argc, char *argv[]) {

    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s power_quality_log.csv\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];


    int n = count_rows(filename);
    if (n <= 0) return 1;
    printf("Rows found: %d\n", n);


    waveform *samples = malloc(n * sizeof(waveform));
    if (samples == NULL) {
        fprintf(stderr, "Error: malloc failed - out of memory\n");
        return 1;
    }


    load_csv(filename, samples, n);


    double rms_A = compute_rms(samples, n, 0);
    double rms_B = compute_rms(samples, n, 1);
    double rms_C = compute_rms(samples, n, 2);

    double pp_A  = compute_peak_to_peak(samples, n, 0);
    double pp_B  = compute_peak_to_peak(samples, n, 1);
    double pp_C  = compute_peak_to_peak(samples, n, 2);

    double dc_A  = compute_dc_offset(samples, n, 0);
    double dc_B  = compute_dc_offset(samples, n, 1);
    double dc_C  = compute_dc_offset(samples, n, 2);

    int clip_A   = count_clipped(samples, n, 0);
    int clip_B   = count_clipped(samples, n, 1);
    int clip_C   = count_clipped(samples, n, 2);


    printf("--- Phase A ---\n");
    printf("  RMS Voltage : %.2f V\n", rms_A);
    printf("  Status      : %s\n", check_compliance(rms_A) ? "COMPLIANT" : "OUT OF TOLERANCE");
    printf("  Peak-to-Peak: %.2f V\n", pp_A);
    printf("  DC Offset   : %.4f V\n", dc_A);
    printf("  Clipped     : %d samples\n\n", clip_A);

    printf("--- Phase B ---\n");
    printf("  RMS Voltage : %.2f V\n", rms_B);
    printf("  Status      : %s\n", check_compliance(rms_B) ? "COMPLIANT" : "OUT OF TOLERANCE");
    printf("  Peak-to-Peak: %.2f V\n", pp_B);
    printf("  DC Offset   : %.4f V\n", dc_B);
    printf("  Clipped     : %d samples\n\n", clip_B);

    printf("--- Phase C ---\n");
    printf("  RMS Voltage : %.2f V\n", rms_C);
    printf("  Status      : %s\n", check_compliance(rms_C) ? "COMPLIANT" : "OUT OF TOLERANCE");
    printf("  Peak-to-Peak: %.2f V\n", pp_C);
    printf("  DC Offset   : %.4f V\n", dc_C);
    printf("  Clipped     : %d samples\n\n", clip_C);


    write_results("results.txt", samples, n);
    printf("\nResults written to results.txt\n");


    free(samples);
    samples = NULL;
    return 0;
}