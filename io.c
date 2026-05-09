#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
int folder() {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
fclose(fp);
return 0;
}
