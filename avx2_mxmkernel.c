#include <immintrin.h>

#include <stdlib.h>

#include "mxmkernel.h"

void mxmkernel(float *__restrict__ a, float *__restrict__ b, float *__restrict__ c, int n)
{
    float *__restrict__ bb;
    posix_memalign((void **) &bb, 64, n * n * sizeof(float));

    int i, j, k;

#pragma omp parallel
{

#pragma omp for schedule(static) private(i, j, k)
    for (i = 0; i < n; i += 8) {
#pragma GCC ivdep
        for (j = 0; j < n; j++) {
            for (k = 0; k < 8; k++) {
                bb[i * n + j * 8 + k] = b[j * n + i + k];
            }
        }
    }

#pragma omp for schedule(static) private(i, j, k) nowait
    for (i = 0; i < n; i += 4) {
        for (j = 0; j < n; j += 8 * 2) {
            __m256 cc00 = _mm256_setzero_ps();
            __m256 cc01 = _mm256_setzero_ps();
            __m256 cc10 = _mm256_setzero_ps();
            __m256 cc11 = _mm256_setzero_ps();
            __m256 cc20 = _mm256_setzero_ps();
            __m256 cc21 = _mm256_setzero_ps();
            __m256 cc30 = _mm256_setzero_ps();
            __m256 cc31 = _mm256_setzero_ps();
            for (k = 0; k < n; k++) {
                // c[i * n + j] += a[i * n + k] * b[k * n + j];
                __m256 aa0 = _mm256_broadcast_ss(&a[i * n + k]);
                __m256 aa1 = _mm256_broadcast_ss(&a[(i + 1) * n + k]);
                __m256 aa2 = _mm256_broadcast_ss(&a[(i + 2) * n + k]);
                __m256 aa3 = _mm256_broadcast_ss(&a[(i + 3) * n + k]);
                __m256 bb0 = _mm256_load_ps(&bb[j * n + k * 8]);
                __m256 bb1 = _mm256_load_ps(&bb[(j + 8) * n + k * 8]);
                cc00 = _mm256_fmadd_ps(aa0, bb0, cc00);
                cc01 = _mm256_fmadd_ps(aa0, bb1, cc01);
                cc10 = _mm256_fmadd_ps(aa1, bb0, cc10);
                cc11 = _mm256_fmadd_ps(aa1, bb1, cc11);
                cc20 = _mm256_fmadd_ps(aa2, bb0, cc20);
                cc21 = _mm256_fmadd_ps(aa2, bb1, cc21);
                cc30 = _mm256_fmadd_ps(aa3, bb0, cc30);
                cc31 = _mm256_fmadd_ps(aa3, bb1, cc31);
            }
            _mm256_store_ps(&c[i * n + j], cc00);
            _mm256_store_ps(&c[i * n + j + 8], cc01);
            _mm256_store_ps(&c[(i + 1) * n + j], cc10);
            _mm256_store_ps(&c[(i + 1) * n + j + 8], cc11);
            _mm256_store_ps(&c[(i + 2) * n + j], cc20);
            _mm256_store_ps(&c[(i + 2) * n + j + 8], cc21);
            _mm256_store_ps(&c[(i + 3) * n + j], cc30);
            _mm256_store_ps(&c[(i + 3) * n + j + 8], cc31);
        }
    }
}

    free(bb);
}
