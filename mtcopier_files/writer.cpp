/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/

#include "writer.h"
pthread_mutex_t wl;

/**
 * provide your implementation for the writer functions here
 **/
writer::writer(const std::string outfile, std::deque<std::string> *queue) {
    this->queue = queue;

    this->out.open(outfile);
}

writer::~writer() {
    this->out.close();
}

void *writer::runner(void * args) {

    while (true) {
        pthread_mutex_lock(&wl);
        if ((*(writer*)args).queue->empty()) {
            usleep(1);
        } else {

            (*(writer*)args).out << (*(writer*)args).queue->front() << std::endl;
            (*(writer*)args).queue->pop_front();
        }
        pthread_mutex_unlock(&wl);
    }
}

void writer::run() {
    pthread_t threads[5];
    int i;

    for( i = 0; i < 5; i++ ) {
        pthread_create(&threads[i], NULL, writer::runner, this);
    }
}
