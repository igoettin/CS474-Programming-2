all: shm_seg matrix_mul

matrix_mul: matrix_mul.c
	gcc -o matrix_mul matrix_mul.c -lpthread -g

shm_seg: shared_mem_seg.c
	gcc -o shm_seg shared_mem_seg.c -g

clean:
	rm -f shm_seg matrix_mul
