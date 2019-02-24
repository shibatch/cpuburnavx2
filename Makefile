all : burnavx2 burnavx512

burnavx2 : burnavx2.c
	$(CC) -fopenmp -mavx2 -mfma -O3 burnavx2.c -o burnavx2

burnavx512 : burnavx512.c
	$(CC) -fopenmp -mavx512f -O3 burnavx512.c -o burnavx512

clean :
	rm -f burnavx2 burnavx512 *~
