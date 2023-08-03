#include "writer.h"

writer::writer(const std::string outfile, std::string *writeArray, int n, pthread_barrier_t *barrers) {
    // initalising the verabes
    this->writeArray = writeArray;
    this->out.open(outfile);
    this->n = n;
    this->barrers = barrers;
}

writer::~writer() {
    // write the last of the file
    this->out << this->writeArray[0];
    // closing the file
    this->out.close();
    // closing the write threads
     for (int i = 0; i < this->n; i++) {
         // std::cout << "write thread " << i << " kill" << std::endl;
         pthread_cancel(this->threads[i]);
     }
     delete this->threads;
     // delete this->writeArray;
}

void *writer::runner(void * args) {
    int id = (*(Arg*)args).id;
    writer *object = (writer*)(*(Arg*)args).object;
    delete (Arg*)args;
    // preventing the 1 thread from writing an empty line at the start of the file
    if (id == 1) {
        pthread_barrier_wait(&(object->barrers[0]));
        pthread_barrier_wait(&(object->barrers[1]));
    }

    while (true) {
        // std::cout << "write thread " << (*(Arg*)args).id << " waiting at read" << std::endl;
        if (id == 0) {
            // waiting for turn
            pthread_barrier_wait(&(object->barrers[object->n -1]));
            // writing line from the write array from position -1
            object->out << object->writeArray[object->n -1];
        } else {
            // waiting for turn
            pthread_barrier_wait(&(object->barrers[id -1]));
            // writing line from the write array from position -1
            object->out << object->writeArray[id -1];
        }

        // signaling write is done
        // std::cout << "write thread " << (*(Arg*)args).id << " waiting at write" << std::endl;
        pthread_barrier_wait(&(object->barrers[id]));
    }
    pthread_exit(NULL);
}

void writer::run() {
    // making the thread ids
    this->threads = new pthread_t[this->n];

    // creating the threads
    for(int i = 0; i < this->n; i++ ) {
        Arg *arg = new Arg();
        arg->id = i;
        arg->object = this;
        pthread_create(&this->threads[i], NULL, writer::runner, arg);
    }
}
