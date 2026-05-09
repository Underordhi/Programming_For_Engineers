#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "waveform.h"

/* -------------------------------------------------------
   phase 0 = A, 1 = B, 2 = C
   'static' means it can only be used inside this file
   ------------------------------------------------------- */

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


/* -------------------------------------------------------
   PEAK-TO-PEAK
   Highest sample minus lowest sample
   ------------------------------------------------------- */
double compute_peak_to_peak(waveform *samples, int n, int phase) {
    double vmax = get_voltage(samples, phase);
    double vmin = vmax;
    waveform *ptr = samples + 1;
    while (ptr < samples + n) {
        double v = get_voltage(ptr, phase);
        if (v > vmax) vmax = v;
        if (v < vmin) vmin = v;
        ptr++;
    }
    return vmax - vmin;
}


double compute_dc_offset(waveform *samples, int n, int phase) {
    double sum = 0.0;
    waveform *ptr = samples;
    while (ptr < samples + n) {
        sum += get_voltage(ptr, phase);
        ptr++;
    }
    return sum / n;
}

/* -------------------------------------------------------
   CLIPPING DETECTION
   Counts samples where |voltage| >= 324.9 V (sensor limit)
   ------------------------------------------------------- */
int count_clipped(waveform *samples, int n, int phase) {
    int count = 0;
    waveform *ptr = samples;
    while (ptr < samples + n) {
        double v = get_voltage(ptr, phase);
        if (v < 0) v = -v;        /* make it positive (absolute value) */
        if (v >= 324.9) count++;
        ptr++;
    }
    return count;
}

/* -------------------------------------------------------
   TOLERANCE COMPLIANCE
   Checks that RMS is within ±10% of 230V
   Returns 1 (pass) or 0 (fail)
   ------------------------------------------------------- */
int check_compliance(double rms) {
    return (rms >= 207.0 && rms <= 253.0);
}