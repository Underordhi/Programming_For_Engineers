#ifndef waveform_h
#define waveform_h

typedef struct {
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} waveform;

double compute_rms(waveform *samples, int n, int phase);
double compute_peak_to_peak(waveform *samples, int n, int phase);
double compute_dc_offset(waveform *samples, int n, int phase);
int    count_clipped(waveform *samples, int n, int phase);
int    check_compliance(double rms);

#endif