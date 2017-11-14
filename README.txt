########################################
    CS474 - Programming Assignment 2      
########################################
----------------------------------------
 Submitted by Ian Goetting and Hang Ngo
----------------------------------------

%%%%%%%%%%%%%%%
1. INTRODUCTION
%%%%%%%%%%%%%%%

The files in this directory contain our solutions to the CS474 Programming Assignment 2. 
pipes.c is our solution to the first problem of the assignment, shared_mem_seg.c is our
solution to the second problem of the assignment, and matrix_mul.c is our solution
to the third problem of the assignment. The following sections describe how to compile
and run our code, as well as details about the code and why we designed the code the way
we did. 

%%%%%%%%%%%%%%%%%
2. HOW TO COMPILE
%%%%%%%%%%%%%%%%%

To compile the files in this directory, simply open a terminal inside this directory and type
"make". This will generate three executable files. pipes is the executable file for pipes.c, 
shm_seg is the executable file for shared_mem_seg.c, and matrix_mul is the executable file for
matrix_mul.c. Each of the following sections describes how to run each file. 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
3. pipes.c | SOLUTION TO PROBLEM 1 OF THE ASSIGNMENT
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pipes.c contains our solution to problem 1 of the programming assignment. This program will create
a child process as well as two pipes in order to facilitate the submission and retrieval of messages
between the parent process and child process. For our solution, we treat the child process as the 
"first" process and the parent process as the "second" process. That is to say, the child process 
will take an input message from the user and send it to the parent process via a pipe. The parent 
process will then receive the message from the pipe, reformat it so that all lowercase letters 
become uppercase and all uppercase letters become lowercase, and then send the formatted message 
back to the child process via a second pipe. The child process will read the formatted message from 
the second pipe and print it out to the user. For our output, we print out the message that the 
parent process receives BEFORE it reformats it, and we print out the formatted message the child 
process receives from the parent.

To run the program pipes.c, first compile with make then type

./pipes

You will be asked to provide an input message. Type a message in and press enter, you will then see the output appear on the terminal.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
4. shared_mem_seg.c | SOLUTION TO PROBLEM 2 OF THE ASSIGNMENT
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

shared_mem_seg.c contains our solution to problem 2 of the programming assignment. This program 
takes as input, from the command line, a shared memory segment identifier and invokes the
shmctl() function to obtain the shmid_ds structure that corresponds to the shared memory segment.
The program then prints out the Segment ID, Key, Mode, Owner UID, Size, and the Number of Attaches
for the shared memory segment. According to the man page for shmctl (http://man7.org/linux/man-pages/man2/shmctl.2.html),
the mode that is returned from the ipc_perm structure contains the permission bits + SHM_DEST 
+ SHM_LOCKED. Therefore, for the mode, we purposefully apply a bitwise AND to it with octal 777 so 
that only the permission bits are shown. We also print out the english meaning of the mode as its
described in the table shown in the assignment instructions.

To run shared_mem_seg.c, first compile with make then type

./shm_seg SHARED_MEMORY_SEGMENT_ID

where SHARED_MEMORY_SEGMENT_ID is the identifier of an existing shared memory segment. 
To see existing shared memory segment identifiers you can use the command "ipcs". Note that in order
for our program to output information about the segment, you must have permission to read the 
segment. According to the manpage for shmctl(), for the IPC_STAT value, "The caller must have read 
permission on the shared memory segment." If you pass our program a shared memory segment identifier
that you do not have permission to read, our program will print an error and abort.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
5. matrix_mul.c | SOLUTION TO PROBLEM 3 OF THE ASSIGNMENT
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

matrix_mul.c contains our solution to problem 3 of the programming assignment. This program will 
create two matrices and use pthreads to multiply the two matrices by giving each thread the row 
number of the first matrix and the column number of the second matrix to facilitate the dot product.
The program takes as input two text files that represent the matrices, creates matrices based on 
those files, multiplies the matrices together using pthreads, and outputs the resulting product 
matrix to the terminal.

To run matrix_mul.c, first compile it with make and then type

./matrix_mul A.txt B.txt

where A.txt is the file containing the first matrix and B.txt is the file containing the second
matrix. We have included two sample matrix files (A.txt and B.txt) in this directory that you can 
run with our program. The format of these files obeys the requirements defined in the assignment 
instructions. Our program also contains default matrices (as defined in the assignment instructions) 
that will be multiplied if no input files are provided. 

To run the program with the default matrices, first compile it with make then type

./matrix_mul

(Notice there are no arguments given)

One important note on our implementation is that we have global pointers representing the three 
matrices and the matrices are allocated on the heap. The reason we chose to do this is that it 
allows us to reuse pointers and functions for the default case (i.e. no input files given) and
the general case. We have tested this program with valgrind's memcheck tool and it has
reported in all the cases we've tried that the heap is fully deallocated by the end of execution. 
That is to say, there are no memory leaks. 

%%%%%%%%%%%
EXTRA NOTES
%%%%%%%%%%%

We have tested each of the above programs using valgrinds memcheck tool and have found that for 
each there are no memory leaks. The command we used to do this is:

valgrind --tool=memcheck ./program_name

