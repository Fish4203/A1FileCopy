/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/

#include "writer.h"
/**
 * provide your implementation for the writer functions here
 **/
writer::writer(const std::string outfile, std::string *writeArray, int n, pthread_barrier_t *barrersRead, pthread_barrier_t *barrersWrite) {
    this->writeArray = writeArray;
    this->running = true;
    this->out.open(outfile);
    this->n = n;
    this->barrersRead = barrersRead;
    this->barrersWrite = barrersWrite;
}

writer::~writer() {
    this->out.close();
}

void *writer::runner(void * args) {
    while ((*(writer*)(*(Arg*)args).object).running) {
        // wait for read
        std::cout << "write thread " << (*(Arg*)args).id << " waiting at read" << std::endl;
        if ((*(Arg*)args).id == 0) {
            pthread_barrier_wait(&((*(writer*)(*(Arg*)args).object).barrersRead[(*(writer*)(*(Arg*)args).object).n -1]));
        } else {
            pthread_barrier_wait(&((*(writer*)(*(Arg*)args).object).barrersRead[(*(Arg*)args).id -1]));
        }
        // write to file
        // std::cout << "write thread " << (*(Arg*)args).id << " writeing" << std::endl;

        if ((*(Arg*)args).id == 0) {
            (*(writer*)(*(Arg*)args).object).out << (*(writer*)(*(Arg*)args).object).writeArray[(*(writer*)(*(Arg*)args).object).n -1] << std::endl;
        } else {
            (*(writer*)(*(Arg*)args).object).out << (*(writer*)(*(Arg*)args).object).writeArray[(*(Arg*)args).id -1] << std::endl;
        }

        std::cout << "write thread " << (*(Arg*)args).id << " waiting at write" << std::endl;
        pthread_barrier_wait(&((*(writer*)(*(Arg*)args).object).barrersRead[(*(Arg*)args).id]));
    }
    pthread_exit(NULL);
}

void writer::run() {
    pthread_t threads[this->n];

    for(int i = 0; i < this->n; i++ ) {
        std::cout << "creating write thread" << i << std::endl;

        Arg *arg = new Arg();
        arg->id = i;
        arg->object = this;
        pthread_create(&threads[i], NULL, writer::runner, arg);
    }

    // std::cout << "w" << std::endl;

}
