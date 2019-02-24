// This software was written by Naoki Shibata in 2018.   https://github.com/shibatch
// No copyright is claimed, and the software is hereby placed in the public domain.

// gcc -fopenmp -mavx2 -mfma -O3 stressavx2.c

#include <stdio.h>
#include <x86intrin.h>

#define VL 8
#define NR 16

void doThings(double *a) {
  __m512d r[NR];
  for(int i=0;i<NR;i++) r[i] = _mm512_loadu_pd(&a[i*VL]);

  for(int i=(1 << 27);i>=0;i--) {
    for(int j=0;j<NR;j++) r[j] = _mm512_fmsub_pd(r[j], r[(j+1)%NR], r[(j+2)%NR]);
  }

  __mmask8 m = 0;
  for(int i=0;i<NR;i++) {
    m |= _mm512_cmp_pd_mask(r[i], _mm512_set1_pd(2.0), _CMP_NEQ_UQ);
  }

  if (m != 0) {
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
