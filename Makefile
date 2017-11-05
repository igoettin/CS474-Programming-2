all: shm_seg


shm_seg: shared_mem_seg.c
	gcc -o shm_seg shared_mem_seg.c

clean:
	rm -f shm_seg
