/*
Filename:
    shared_mem_seg.c

Authors:
    Ian Goetting
    Hang Ngo

Description of program:
    This is our solution to question 2 of the CS474 Programming #2 Assignment.
    This program will create a new shared memory segment and print out values associated with it using the shmid_ds structure.. 
    The values that are printed out include: Segment ID, Key, Mode, Owner UID, Size, and Number of attaches.
*/

#include<errno.h>
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>

//Function that prints out values for the shared memory segment that corresponds to the given segment_id.
void print_shm_seg_values(int segment_id){
    struct shmid_ds shmbuffer;
    //Get the values for the segment ID
    if(shmctl(segment_id,IPC_STAT,&shmbuffer) == -1){
        fprintf(stderr,"ERROR: Could not retrieve data for the shmid_ds shmbuffer! The error code is %d\n",errno);
        shmctl(segment_id,IPC_RMID,NULL);
        exit(-1);
    }
    //Print the values out
    printf("Values for the shared memory segment:\n");
    printf("-------------------------------------\n");
    printf("Segment ID: %d\n",segment_id);
    printf("Key: %d\n",shmbuffer.shm_perm._key);
    //The mode will include permissions + SHM_DEST flag + SHM_LOCKED flag
    //Since we are only interested in the permissions, a bitwise and is used on the mode with octal 777.
    unsigned short mode = shmbuffer.shm_perm.mode & 0777;
    //Show only the first nine bits of the mode (i.e. permission bits)
    printf("Mode (in octal, permission bits only): %ho\n",mode);
    printf("Owner UID: %u\n",shmbuffer.shm_perm.uid);
    printf("Size: %zu\n",shmbuffer.shm_segsz);
    printf("Number of attaches: %d\n",shmbuffer.shm_nattch);
}

int main(){
    int segment_id;
    struct shmid_ds shmbuffer;
    //Create the segment and gets its segment ID.
    //IPC_STAT requires 0400 (Read permission of owner) as part of the shmflg argument
    if((segment_id = shmget(5,32,0400 | IPC_CREAT)) == -1){
        fprintf(stderr, "ERROR: The shared memory segment for key 5 could not be created!");
        shmctl(segment_id,IPC_RMID,NULL);
        exit(-1);
    }
    //Pass the segment ID to the print function to print the segment's values out.
    print_shm_seg_values(segment_id);
    //Delete the shared memory segment based on the segment ID since we are finished with it.
    shmctl(segment_id, IPC_RMID, NULL);
}


