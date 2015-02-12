/* ----------------------------------------------------------------------------
SOURCE FILE

Name:           main.c

Program:        a1 (COMP 8005, Threads vs Processes)

Developer:      Andrew Burian

Created On:     2015-01-09

Functions:
        int main (int argc, char* argv[])


Description:
        Everything besides the prime decomp is handled here

Revisions:
        (none)

---------------------------------------------------------------------------- */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/wait.h>

#include "primedecompose.h"

void threads_setup(size_t numThreads);
void process_setup(size_t numProcess);
void* work(void* id);

int count = 0;


/* ----------------------------------------------------------------------------
FUNCTION

Name:           Main

Prototype:      int main(int argc, char* argv[])

Developer:      Andrew Burian

Created On:     2015-01-09

Parameters:
        int argc    number of command line arguments
        char* argv  array of arguments

Return Values:
        0   success
        -1  error

Description:
        Parses the commad line arguments, and calls either the thread workers
        or the process workers function

Revisions:
        (none)

---------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{

    int useThreads = 0;
    int useProcess = 0;
    int workers = 5;

    char c = 0;
    int optionIndex = 0;

    // Command line argument setup
    struct option longOpts[] = {
        {"threads",     no_argument,        0,  't'},
        {"processes",   no_argument,        0,  'p'},
        {"count",       required_argument,  0,  'c'},
        {"workers",     required_argument,  0,  'w'},

        // null terminated list
        {0,0,0,0}
    };
    char* shortOpts = "tpc:w:";

    // process command line args
    while ((c = getopt_long(argc, argv, shortOpts,
        longOpts, &optionIndex)) != -1){
        switch (c){

        case 'c':
            sscanf(optarg, "%d", &count);
            break;

        case 't':
            useThreads = 1;
            break;

        case 'p':
            useProcess = 1;
            break;

        case 'w':
            sscanf(optarg, "%d", &workers);
            //workers = (workers < 5 ? 5 : workers);
            break;

        case 0:
            // Set a flag
            break;

        default:
            // Encountered an error
            perror("Arguments encountered an error");
            return -1;
        }
    }

    // check to ensure settings are valid
    if(useProcess + useThreads != 1 || count == 0){
        fprintf(stderr, "Invalid argument combination\n");
        return -1;
    }

    // run the work
    if(useProcess){
        process_setup(workers);
    }
    if(useThreads){
        threads_setup(workers);
    }

    // done
    return 0;
}


/* ----------------------------------------------------------------------------
FUNCTION

Name:           Process Setup

Prototype:      void process_setup(size_t numProcess)

Developer:      Andrew Burian

Created On:     2014-01-09

Parameters:
        size_t numProcesses     the number of worker processes to run

Return Values:
        (none)

Description:
        Creates and runs numProcesses of worker processes, each which will
        do the work function.
        Waits for all workers to complete before returning.
        **Worker errors will cause this to exit the program**

Revisions:
        (none)

---------------------------------------------------------------------------- */
void process_setup(size_t numProcess){

    pid_t* pids = (pid_t*)malloc(sizeof(pid_t) * numProcess);
    size_t i = 0;
    int status = 0;

    // start child processes
    for(i = 0; i < numProcess; ++i){
        if(!(pids[i] = fork())){
            // exit after completeing and return the value returned by work
            exit((int)work((void*)i));
        }
        if(pids[i] == -1){
            perror("Fork failed");
            exit(-1);
        }
    }

    // wait for all children to terminate
    for(i = 0; i < numProcess; ++i){
        waitpid(pids[i], &status, 0);
        if(status){
            fprintf(stderr, "Child returned non-zero: %d\n", status);
            exit(-1);
        }
    }

    // cleanup
    free(pids);
}


/* ----------------------------------------------------------------------------
FUNCTION

Name:           Threads Setup

Prototype:      void threads_setup(size_t numThreads)

Developer:      Andrew Burian

Created On:     2014-01-09

Parameters:
        size_t numThreads       the number of worker threads to run

Return Values:
        (none)

Description:
        Creates and runs numThreads of worker threads, each which will
        do the work function.
        Waits for all workers to complete before returning.
        **Worker errors will cause this to exit the program**

Revisions:
        (none)

---------------------------------------------------------------------------- */
void threads_setup(size_t numThreads){

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * numThreads);
    size_t i = 0;

    // spawn threads
    for(i = 0; i < numThreads; ++i){
        if(pthread_create(&threads[i], NULL, work, (void *)i)){
            perror("Thread create failed");
            exit(-1);
        }
    }

    // wait on threads to finish
    for(i = 0; i < numThreads; ++i){
        pthread_join(threads[i], NULL);
    }

    // cleanup
    free(threads);

}


/* ----------------------------------------------------------------------------
FUNCTION

Name:           Work

Prototype:      void* work(void* id)

Developer:      Aman Abdulla
                Andrew Burian

Created On:     2015-01-09

Parameters:
        The worker's id value (int)

Return Values:
        0   success
        -1  failure

Description:
        Factors all numbers between 1 and count and saves the results into
        a file called worked<id>.tmp

Revisions:
        Andrew Burian
        2015-01-09
        Took original work by Aman and set it to output to the named file

---------------------------------------------------------------------------- */

void* work(void* id){

    char filename[32] = {0};
    FILE* outputFile = 0;
    unsigned long int num = 0;

    mpz_t dest[1024];
    mpz_t n;
    int i, l;

    // make the filename
    snprintf(filename, 32, "worker%d.tmp", (int)id);

    // open the file
    outputFile = fopen(filename, "w");
    if(!outputFile){
        perror("File failed to open");
        return (void*)-1;
    }

    for(num = 1; num < count; ++num){

        mpz_init_set_ui(n, num);
        l = decompose(n, dest);
        fprintf(outputFile, "%d: ", num);
        for(i=0; i < l; i++)
        {
                gmp_fprintf(outputFile, "%s%Zd", i?" * ":"", dest[i]);
                mpz_clear(dest[i]);
        }
        fprintf(outputFile, "\n");
    }

    return 0;

}
