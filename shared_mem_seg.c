/*
Filename:
    shared_mem_seg.c

Authors:
    Ian Goetting
    Hang Ngo

Description of program:
    This is our solution to question 2 of the CS474 Programming #2 Assignment.
    This program will print out values for a shared memory segment using the shmid_ds structure in conjunction with the shmctl() function.
    The values that are printed out include: Segment ID, Key, Mode, Owner UID, Size, and Number of attaches.
*/

#include<errno.h>
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>

//Function that prints out information about the shared memory segment that corresponds to the given a segment ID.
//Precondition: There must be a shared memory segment that already exists for the given segment ID.
void print_shared_memory_values(int segment_id){
    struct shmid_ds shmbuffer;
    //Obtain the shmid_ds structure for the shared memory segment
    //Abort if there is no segment that exists for the given segment_id
    if(shmctl(segment_id,IPC_STAT,&shmbuffer) == -1){
        //errno = 22 when the segment does not exist for the given segment_id
        if(errno == 22)
            fprintf(stderr,"ERROR: There is no shared memory segment for the given segment ID\n"); 
        //errno = 13 when the segment exists for the given segment_id, but the caller of shmctl with IPC_stat does not have read permission.
        else if(errno == 13)
            fprintf(stderr,"ERROR: Cannot read the segment; the permissions on the segment do not allow it to be read.\n");
        else
            fprintf(stderr,"ERROR: Could not retrieve the shared memory segment's information. errno was set to %d\n",errno);
        exit(0);
    }
    //Print the values out
    printf("\nValues for the shared memory segment:\n");
    printf("-------------------------------------\n");
    printf("Segment ID: %d\n",segment_id);
    printf("Key (in hexadecimal): 0x%x\n",shmbuffer.shm_perm.__key);
    //The mode will include permissions + SHM_DEST flag + SHM_LOCKED flag
    //Since we are only interested in the permissions, a bitwise and is used on the mode with octal 777.
    unsigned short mode = shmbuffer.shm_perm.mode & 0777;
    printf("Mode (in octal, permission bits only): %ho\n",mode);
    //Print the english meaning of the mode
    printf("English meaning of the mode for the shared memory segment:\n");
    if(mode & 0400) printf("\tRead permission of owner.\n");
    if(mode & 0200) printf("\tWrite permission of owner.\n");
    if(mode & 0040) printf("\tRead permission of group.\n");
    if(mode & 0020) printf("\tWrite permission of group.\n");
    if(mode & 0004) printf("\tRead permission of world.\n");
    if(mode & 0002) printf("\tWrite permission of world.\n");
    printf("Owner UID: %u\n",shmbuffer.shm_perm.uid);
    printf("Size: %zu\n",shmbuffer.shm_segsz);
    printf("Number of attaches: %d\n\n",shmbuffer.shm_nattch);
}

int main(int argv, char** argc){
    //Check an argument was given
    if(argc[1] == NULL){
        fprintf(stderr,"ERROR: No segment ID was passed to the program\n");
        exit(0);
    }
    //Convert the string given to an integer, pass it to the function
    print_shared_memory_values(atoi(argc[1]));
}



