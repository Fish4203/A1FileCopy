/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <pthread.h>
#include "reader.h"
#include "writer.h"
#include <cstdlib>

// bootstraps the program by activating the barrier[0]
void *bootstrap(void *args) {
    // std::cout << "bootstrap running" << std::endl;
    pthread_barrier_wait(&(*(pthread_barrier_t*)args));
    pthread_exit(NULL);
}


int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "wrong number of inputs" << std::endl;
        return 1;
    }
    // number of threads
    int n = std::stoi(argv[1]);
    if (n < 25 || n > 100) {
        std::cout << "n not between 25 and 100" << std::endl;
        return 1;
    }

    // in and out files
    std::string infile = argv[2];
    std::string outfile = argv[3];

    /*
    I used barrers to syncronise threads
    for the first n barrers wait for 4 threads befor alowing execution
    each read and write thread i start by waiting at barrer[i-1]
    after they execute a read/write they wait at barrer[i]
    because the i + 1 read/write threads will be waiting at i + 1 - 1
    there will be 4 threads at the barrer[i] so read/write threads i + 1 will begin and threads i will reset
    */
    pthread_barrier_t barrers[n+1];
    for (int i = 0; i < n; i++ ) {
        pthread_barrier_init(&barrers[i], 0, 4);
    }
    // the n barrer thread will cause the main thread to wait for a read thread to reach the end of the file
    pthread_barrier_init(&barrers[n], 0, 2);

    // where the read and write threads will store data
    std::string writeArray[n];

    // reader and writer
    reader *read = new reader(infile, writeArray, n, barrers);
    writer *write = new writer(outfile, writeArray, n, barrers);

    // creates the read and write threads
    read->run();
    write->run();


    // starts the write by having 2 threads waiting at barrer[0]
    pthread_t thread;
    pthread_create(&thread, NULL, bootstrap, barrers);
    pthread_barrier_wait(&barrers[0]);

    // waits for a read thread
    pthread_barrier_wait(&barrers[n]);

    delete read;
    delete write;
    // for (int i = 0; i <= n; i++ ) {
    //     pthread_barrier_destroy(&barrers[i]);
    // }
    // delete thread;

    return EXIT_SUCCESS;
}
