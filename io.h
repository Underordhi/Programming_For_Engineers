#ifndef io_h
#define io_h
#include "waveform.h"

int  count_rows(const char *filename);
void load_csv(const char *filename, waveform *samples, int n);
void write_results(const char *filename, waveform *samples, int n);


#endif