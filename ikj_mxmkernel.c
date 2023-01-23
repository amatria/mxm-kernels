#include "mxmkernel.h"

void mxmkernel(float *__restrict__ a, float *__restrict__ b, float *__restrict__ c, int n)
{
    int i, j, k;

#pragma omp parallel for private(i, j, k) schedule(static)
    for (i = 0; i < n; i++) {
        for (k = 0; k < n; k++) {
#pragma GCC ivdep
            for (j = 0; j < n; j++) {
                c[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
