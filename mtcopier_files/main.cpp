/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <pthread.h>
#include "reader.h"
#include "writer.h"
#include <cstdlib>
/* global variables if needed go here */

void *bootstrap(void *args) {
    std::cout << "bootstrap running" << std::endl;
    pthread_barrier_wait(&(*(pthread_barrier_t*)args));
    pthread_exit(NULL);
}


int main(int argc, char** argv) {
    /* check command line arguments */
    /* load the file and copy it to the destination */
    int n = std::stoi(argv[3]);
    std::string infile = argv[1];
    std::string outfile = argv[2];

    //barrers
    pthread_barrier_t barrersRead[n];
    pthread_barrier_t barrersWrite[n];

    for (int i = 0; i < n; i++ ) {
        pthread_barrier_init(&barrersRead[i], 0, 4);
        pthread_barrier_init(&barrersWrite[i], 0, 2);
    }

    std::string writeArray[n];
    writeArray[n-1] = "";

    reader *read = new reader(infile, writeArray, n, barrersRead, barrersWrite);
    writer *write = new writer(outfile, writeArray, n, barrersRead, barrersWrite);

    read->run();
    write->run();
    pthread_t thread;


    sleep(3);
    std::cout << "bootstrap running" << std::endl;
    pthread_create(&thread, NULL, bootstrap, barrersRead);
    pthread_barrier_wait(&barrersRead[0]);


    // for (int i = 0; i < n-1; i++ ) {
    //     // sleep(2);
    //     pthread_barrier_wait(&barrersWrite[i]);
    // }
    //
    // for (int i = n; i > 0; i-- ) {
    //     pthread_barrier_wait(&barrersWrite[i]);
    // }


    sleep(100);

    delete read;
    delete write;
    // delete writeArray;

    return EXIT_SUCCESS;
}
