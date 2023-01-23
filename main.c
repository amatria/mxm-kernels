#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "mxmkernel.h"

int main(void)
{
    int n = 1024;

    // init matrices
    float *a, *b, *c;
    posix_memalign((void **) &a, 64, n * n * sizeof(float));
    posix_memalign((void **) &b, 64, n * n * sizeof(float));
    posix_memalign((void **) &c, 64, n * n * sizeof(float));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = ((float) rand()) / ((float) RAND_MAX) * 10.0;
            b[i * n + j] = ((float) rand()) / ((float) RAND_MAX) * 10.0;
        }
    }

    for (int i = 0; i < 4; i++) {
        memset(c, 0, n * n * sizeof(float));

        struct timespec t_start, t_end;
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_start);
        mxmkernel(a, b, c, n);
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);

        uint64_t start = (uint64_t) t_start.tv_sec * 1000000000 + (uint64_t) t_start.tv_nsec;
        uint64_t end = (uint64_t) t_end.tv_sec * 1000000000 + (uint64_t) t_end.tv_nsec;

        double dn = (double) n;
        double flops = dn * dn * dn * 2.0;
        double time = (end - start) * 1e-9;

        printf("%.2f GLFOP/S, %.2f ms\n", flops / time / 1e9, time * 1e3);
    }

    free(a);
    free(b);
    free(c);

    return 0;
}
