/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/

#include "writer.h"
pthread_mutex_t wl;
pthread_cond_t condw[5];

class Argstw {
public:
    int id;
    writer *write;
};

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
        // std::cout << (*(Argstw*)args).id << std::endl;

        pthread_mutex_lock(&wl);
        if ((*(Argstw*)args).write->queue->empty()) {
            usleep(1);
        } else {
            (*(Argstw*)args).write->out << (*(Argstw*)args).write->queue->front() << std::endl;
            (*(Argstw*)args).write->queue->pop_front();
        }

        pthread_cond_signal(&condw[(*(Argstw*)args).id]);

        if ((*(Argstw*)args).id == 4) {
            pthread_cond_wait(&condw[0], &wl);
        } else {
            pthread_cond_wait(&condw[(*(Argstw*)args).id +1], &wl);
        }

        pthread_mutex_unlock(&wl);
    }
}

void writer::run() {
    pthread_t threads[5];
    int i;

    for( i = 0; i < 5; i++ ) {
        Argstw *arg = new Argstw();
        arg->id = i;
        arg->write = this;
        pthread_create(&threads[i], NULL, writer::runner, arg);
    }
}
