/*
    Simple program that creates 6 memory segments with different permissions.
    Used to test the shared_mem_seg.c file
*/

#include<errno.h>
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>

int main(int argv, char** argc){
    shmget(5,16,IPC_CREAT | 0400);
    shmget(6,32,IPC_CREAT | 0200);
    shmget(7,64,IPC_CREAT | 0040);
    shmget(8,128,IPC_CREAT | 0020);
    shmget(9,256,IPC_CREAT | 0004);
    shmget(10,512,IPC_CREAT | 0002);
}



