/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include <functional>
pthread_mutex_t rl;


reader::reader(const std::string infile, std::deque<std::string> *queue) {
    this->queue = queue;

    this->in.open(infile);
}

reader::~reader() {
    this->in.close();
}

void *reader::runner(void * args) {
    std::string s;

    while (true) {
        pthread_mutex_lock(&rl);
        if (getline((*(reader*)args).in, s)) {
            (*(reader*)args).queue->push_back(s);
        } else {
            (*(reader*)args).queue->push_back(s);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&rl);
    }
}

void reader::run() {


    pthread_t threads[5];
    int i;

    for( i = 0; i < 5; i++ ) {
        pthread_create(&threads[i], NULL, reader::runner, this);
    }
}
