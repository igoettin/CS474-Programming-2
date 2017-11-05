#include<errno.h>
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<string.h>

void print_shm_seg_values(int segment_id){
    struct shmid_ds shmbuffer;
    //Get the values for the segment ID
    if(shmctl(segment_id,IPC_STAT,&shmbuffer) == -1){
        fprintf(stderr,"ERROR: Could not retrieve data for the shmid_ds shmbuffer! The error code is %d\n",errno);
        shmctl(segment_id,IPC_RMID,NULL);
        exit(-1);
    }
    //Print the values out
    //printf("SHM_DEST IS %ho\n",shmbuffer.shm_perm.SHM_DEST);
    printf("Values for the shared memory segment:\n");
    printf("-------------------------------------\n");
    printf("Segment ID: %d\n",segment_id);
    printf("Key: %d\n",shmbuffer.shm_perm._key);
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
    //Print the values for the ID
    print_shm_seg_values(segment_id);
    //Remove the shared memory segment based on the segment ID.
    shmctl(segment_id, IPC_RMID, NULL);
}


