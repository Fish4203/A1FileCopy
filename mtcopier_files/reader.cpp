/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include <functional>


reader::reader(const std::string infile, std::string *writeArray, int n, pthread_barrier_t *barrersRead, pthread_barrier_t *barrersWrite) {
    this->writeArray = writeArray;
    this->in.open(infile);
    this->n = n;
    this->barrersRead = barrersRead;
    this->barrersWrite = barrersWrite;
}

reader::~reader() {
    this->in.close();
}

void *reader::runner(void * args) {
    std::string s;
    bool run = true;

    while (run) {
        std::cout << "read thread " << (*(Arg*)args).id << " waiting at read -1" << std::endl;
        if ((*(Arg*)args).id == 0) {
            pthread_barrier_wait(&((*(reader*)(*(Arg*)args).object).barrersRead[(*(reader*)(*(Arg*)args).object).n -1]));
        } else {
            pthread_barrier_wait(&((*(reader*)(*(Arg*)args).object).barrersRead[(*(Arg*)args).id -1]));
        }

        std::cout << "read thread " << (*(Arg*)args).id << " waiting at write" << std::endl;
        pthread_barrier_wait(&((*(reader*)(*(Arg*)args).object).barrersWrite[(*(Arg*)args).id]));

        // read line
        // std::cout << "read thread " << (*(Arg*)args).id << " start read" << std::endl;
        if (!getline((*(reader*)(*(Arg*)args).object).in, s)) {
            run = false;
            std::cout << "end of file" << std::endl;
        }
        // write line to the write array
        (*(reader*)(*(Arg*)args).object).writeArray[(*(Arg*)args).id] = s;

        // wait for write to be done
        std::cout << "read thread " << (*(Arg*)args).id << " read done read" << std::endl;
        pthread_barrier_wait(&((*(reader*)(*(Arg*)args).object).barrersRead[(*(Arg*)args).id]));
    }
    pthread_exit(NULL);
}

void reader::run() {
    // std::cout << "r" << std::endl;

    pthread_t threads[this->n];
    // void *ret_join;
    // pthread_condr_init(&condr, 0);

    for (int i = 0; i < this->n; i++ ) {
        std::cout << "creating read thread" << i << std::endl;

        Arg *arg = new Arg();
        arg->id = i;
        arg->object = this;
        pthread_create(&threads[i], NULL, reader::runner, arg);
    }

    // for (i = 0; i < 5; i++) {
    //     pthread_join(threads[i], NULL);
    //     pthread_cond_destroy(&condr[i]);
    //     // std::cout << "tree";
    // }

    // std::cout << "r" << std::endl;

    // pthread_join(threads[i], &ret_join);
}
