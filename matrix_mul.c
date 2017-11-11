/*
    Filename:
        matrix_mul.c

    Authors:
        Ian Goetting
        Hang Ngo

    Description:
        This program will perform matrix multiplication using pthreads and output the resulting matrix's contents to the terminal.
        It takes as input two text files which represent the matrices that are being multiplied.
        The first line of each file should contain the number of rows in the matrix, which should be an integer.
        The second line of each file should contain the number of columns in the matrix, which should be an integer.
        The rest of the lines in the file describe the matrix's contents through space separated integers.
        An example of the format is shown below for a 3x2 matrix:
        
        3
        2
        1 4
        2 5
        3 6

        If no input files are given, the program will calculate the multiplication of two default matrices.
        

*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

//Dimensions of the array, set to default
int M = 3;
int K = 2;
int N = 3;
//Arrays for when files are given
int ** A;
int ** B;
int ** C;

typedef struct{
    int i; //row
    int j; //column
} thread_info;

//Function that contains the work performed by each worker thread.
void * multiply_row_with_col(void * param){
    thread_info * data = (thread_info *) param;
    int i,sum = 0;
    //Multiply the column elements of A with the row elements of B, sum them up.
    for(i = 0; i < K; i++)
        sum += A[data->i][i] * B[i][data->j];
    C[data->i][data->j] = sum;
}

//Function that allocates a matrix.
//S_row is the number of rows in the matrix, S_col is the number of columns in the matrix.
int ** allocate_matrix(int S_row, int S_col){
    int i;
    int ** S = malloc(sizeof(int *) * S_row);
        for(i = 0; i < S_row; i++){
            S[i] = malloc(sizeof(int) * S_col);
        }
    return S;
}

//Function that deallocates a matrix. 
//S is a reference to the matrix, S_row is the number of rows in the matrix
void deallocate_matrix(int ** S, int S_row){
    int i;
    for(i = 0; i < S_row; i++)
        free(S[i]);
    free(S);
}

//Simple function that prints the contents of the matrix C
//Precondition: C must be already allocated.
void print_matrix_C(){
    printf("\nThe product array C of the arrays A and B is calculated as: \n");
    int i,j;
    for(i = 0; i < M; i++){
        for(j = 0; j < N; j++)
            printf("%d ",C[i][j]);
        printf("\n");
    }
    printf("\n");
}
//Function that creates each work thread and assigns it to complete the work in multiply_row_with_col
//The threads are then joined, and the allocated data structure is deallocated.
void create_threads(){
    int i, j, counter = 0;
    pthread_t thread_ids[M*N];
    thread_info * threads = (thread_info *) malloc(sizeof(thread_info) * (M*N));
    for(i = 0; i < M; i++)
         for(j = 0; j < N; j++){
             thread_info * data = &threads[counter];
             data->i = i;
             data->j = j;
             pthread_create(&thread_ids[counter++],NULL,multiply_row_with_col,data);
         }
    
    for(i = 0; i < M*N; i++)
         pthread_join(thread_ids[i],NULL);
    free(threads);
}

void multiply_matrices(FILE * A_file, FILE * B_file){
    //First case, no files given, multiply the default matrices.
    if(A_file == NULL && B_file == NULL){
        int i;
        //Dynamically allocate the A array with M rows and K columns.
        A = allocate_matrix(M,K);
        //Fill the A array with values
        A[0][0] = 1;
        A[0][1] = 4;
        A[1][0] = 2;
        A[1][1] = 5;
        A[2][0] = 3;
        A[2][1] = 6;
        //Dynamically allocate the B array with K rows and N columns.
        B = allocate_matrix(K,N);
        //Fill the B array with values
        B[0][0] = 8;
        B[0][1] = 7;
        B[0][2] = 6;
        B[1][0] = 5;
        B[1][1] = 4;
        B[1][2] = 3;
        //Dynamically allocate the C array with M rows and N columns.
        C = allocate_matrix(M,N);
        //Create the threads, perform the math
        create_threads();
        //Print matrix C
        print_matrix_C();
        //deallocate the matrices 
        deallocate_matrix(A,M);
        deallocate_matrix(B,K);
        deallocate_matrix(C,M);
    }
    //Second case, both files given, read the files and multiply the matrices.
    else {
        int A_row, A_col, B_row, B_col, i, j, temp;
        //Get num rows for A
        if(fscanf(A_file,"%d",&A_row) == EOF){
            fprintf(stderr,"The number of rows for matrix A is missing from the file!\n");
            exit(0);
        }
        //Get num cols for A
        else if(fscanf(A_file,"%d",&A_col) == EOF){
            fprintf(stderr,"The number of columns for matrix A is missing from the file!\n");
            exit(0);
        }
        //Get num rows for B
        if(fscanf(B_file,"%d",&B_row) == EOF){
            fprintf(stderr,"The number of rows for matrix B is missing from the file!\n");
            exit(0);
        }
        //Check number of A columns matches number of B rows
        if(A_col != B_row){
            fprintf(stderr,"The number of columns for matrix A is not the same as the number of rows in matrix B. Cannot multiply.\n");
            exit(0);
        }
        //Get num cols for B
        else if(fscanf(B_file,"%d",&B_col) == EOF){
            fprintf(stderr,"The number of columns for matrix B is missing!\n");
            exit(0);
        }
        //Dynamically allocate the A array.
        A = allocate_matrix(A_row,A_col);
        //Fill the A array with values
        for(i = 0; i < A_row; i++)
            for(j = 0; j < A_col; j++){
                if(fscanf(A_file,"%d",&temp) == EOF){
                    fprintf(stderr, "There are not enough values given to fully fill the A array!\n");
                    exit(0);
                }
                A[i][j] = temp;
            }
        //Dynamically allocate the B array.
        B = allocate_matrix(B_row,B_col);
        //Fill the B array with values
        for(i = 0; i < B_row; i++)
            for(j = 0; j < B_col; j++){
                if(fscanf(B_file,"%d",&temp) == EOF){
                    fprintf(stderr, "There are not enough values given to fully fill the A array!\n");
                    exit(0);
                }
                B[i][j] = temp;
            } 
        //Set the global variables for dimensions
        M = A_row;
        K = A_col;
        N = B_col;
        //Dynamically allocate the C array.
        C = allocate_matrix(M,N);
        //Create the threads
        create_threads(); 
        //Print the result
        print_matrix_C();
        //Deallocate matrices A,B, and C
        deallocate_matrix(A,M);
        deallocate_matrix(B,K);
        deallocate_matrix(C,M);
    }
}

void process_input(int argv, char ** argc){
    //Check if no arguments given
    if(argv == 1){
        //Handle default case, multiply default matrices
        multiply_matrices(NULL,NULL);
        
    }
    //Check if the first filename was given but not the second
    else if(argv == 2){
        fprintf(stderr,"ERROR: Only the first filename was given, two are needed.");
        exit(0);
    }
    //Two arguments were given
    else{
        FILE * A_file = fopen(argc[1],"r");
        FILE * B_file = fopen(argc[2],"r");   
        if(A_file == NULL){
            fprintf(stderr,"ERROR: Could not open the first file!\n");
            exit(0);
        }
        else if(B_file == NULL){
            fprintf(stderr,"ERROR: Could not open the second file!\n");
            exit(0);
        }
        else
            multiply_matrices(A_file,B_file);
        fclose(A_file);
        fclose(B_file);
    }
}

int main(int argv, char** argc){
    process_input(argv,argc);
}
