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
    pthread_barrier_wait(&(*(pthread_barrier_t*)args));
    pthread_exit(NULL);
}


int main(int argc, char** argv) {
    clock_t start = clock();
    bool t = false;
    if (argc != 4) {
        if (argc == 5) {
            t =true;
        } else {
            std::cout << "wrong number of inputs" << std::endl;
            return 1;
        }
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
    pthread_barrier_init(&barrers[n], 0, n+1);

    // where the read and write threads will store data
    std::string writeArray[n+1];
    // true when the program is running
    bool run = true;

    // reader and writer
    reader *read = new reader(infile, writeArray, n, barrers, &run);
    writer *write = new writer(outfile, writeArray, n, barrers, &run);

    // creates the read and write threads
    read->run();
    write->run();

    // starts the write by having 2 threads waiting at barrer[0]
    pthread_t thread;
    pthread_create(&thread, NULL, bootstrap, barrers);
    pthread_barrier_wait(&barrers[0]);
    pthread_join(thread, NULL);

    // waits for the read threads to be done
    pthread_barrier_wait(&barrers[n]);

    read->Stime.pop();
    write->Stime.pop();

    double readTotal = 0;
    double writeTotal = 0;
    double startDiff = 0;
    double endDiff = 0;
    int j = 0;

    while (!write->Etime.empty()) {
        readTotal += read->Stime.front() - read->Etime.front();
        writeTotal += write->Stime.front() - write->Etime.front();
        // startDiff += abs(read->Stime.front() - write->Stime.front());
        // endDiff += abs(read->Etime.front() - write->Etime.front());
        // std::cout << read->Etime.front() - write->Stime.front() <<"read to write time" << std::endl;
        // std::cout << write->Etime.front() - read->Stime.front() <<"write to read time" << std::endl;
        read->Stime.pop();
        read->Etime.pop();
        write->Stime.pop();
        write->Etime.pop();
        j++;
    }

    if (t) {
        std::cout << "read total" << readTotal / (double)CLOCKS_PER_SEC << std::endl;
        std::cout << "write total" << writeTotal / (double)CLOCKS_PER_SEC << std::endl;
        std::cout << "read avg" << readTotal / j << std::endl;
        std::cout << "write avg" << writeTotal / j << std::endl;
        // std::cout << "start diff in clocks" << (startDiff / j) << std::endl;
        // std::cout << "end diff in clock" << (endDiff / j) << std::endl;
        std::cout << "total" << (clock() - start) / (double)CLOCKS_PER_SEC << std::endl;
    }
    // clean up
    delete read;
    delete write;


    return EXIT_SUCCESS;
}
