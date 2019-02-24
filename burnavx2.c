// This software was written by Naoki Shibata in 2018.   https://github.com/shibatch
// No copyright is claimed, and the software is hereby placed in the public domain.

// gcc -fopenmp -mavx2 -mfma -O3 burnavx2.c

#include <stdio.h>
#include <x86intrin.h>

#define VL 4
#define NR 16

void doThings(double *a) {
  __m256d r[NR];
  for(int i=0;i<NR;i++) r[i] = _mm256_loadu_pd(&a[i*VL]);

  for(int i=(1 << 27);i>=0;i--) {
    for(int j=0;j<NR;j++) r[j] = _mm256_fmsub_pd(r[j], r[(j+1)%NR], r[(j+2)%NR]);
  }

  __m256d c = _mm256_set1_pd(0);
  for(int i=0;i<NR;i++) {
    c = _mm256_or_pd(c, _mm256_cmp_pd(r[i], _mm256_set1_pd(2.0), _CMP_NEQ_UQ));
  }
  __m256i g = _mm256_andnot_si256(_mm256_castpd_si256(c), _mm256_set1_epi32(-1));

  if (!_mm_test_all_ones(_mm_and_si128(_mm256_extractf128_si256(g, 0), _mm256_extractf128_si256(g, 1)))) {
    fprintf(stderr, "Error detected\n");
    exit(-1);
  }
}

#pragma GCC optimize ("O0")

int main(int argc, char **argv) {
  double a[NR*VL];
  for(int i=0;i<NR*VL;i++) a[i] = 2.0;

  for(;;) {
    int i;
#pragma omp parallel for
    for(i=0;i<65536;i++) {
      doThings(a);
    }
  }
}
