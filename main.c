// This software was written by Naoki Shibata.   https://github.com/shibatch

#define VL 8
#define NR 16

void doThings(double *a);

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
