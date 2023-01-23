#include "mxmkernel.h"

void mxmkernel(float *__restrict__ a, float *__restrict__ b, float *__restrict__ c, int n)
{
    int i, j, k;

#pragma omp parallel for private(i, j, k) schedule(static)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                c[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
