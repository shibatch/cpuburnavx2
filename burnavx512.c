// This software was written by Naoki Shibata.   https://github.com/shibatch

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
