#include "writer.h"

writer::writer(const std::string outfile, std::string *writeArray, int n, pthread_barrier_t *barrers, bool *runing) {
    // initalising the verabes
    this->runing = runing;
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
         std::cout << "write thread " << i << " join" << std::endl;
         pthread_join(this->threads[i], NULL);
     }
     delete this->threads;
     // delete this->writeArray;
}

void *writer::runner(void * args) {
    int id = (*(Arg*)args).id;
    writer *object = (writer*)(*(Arg*)args).object;
    // delete (Arg*)args;
    // preventing the 1 thread from writing an empty line at the start of the file
    if (id == 1) {
        pthread_barrier_wait(&(object->barrers[0]));
        pthread_barrier_wait(&(object->barrers[1]));
    }

    while (true) {
        pthread_barrier_wait(&(object->barrers[MININDEX(id, object->n)]));

        if (!(*object->runing) && object->writeArray[id] != "") {
            // std::cout << "write thread " << id << " exiting " << std::endl;
            pthread_barrier_wait(&(object->barrers[id]));
            break;
        }
        object->out << object->writeArray[MININDEX(id, object->n)];

        // signaling write is done
        // std::cout << "write thread " << (*(Arg*)args).id << " waiting at write" << std::endl;
        pthread_barrier_wait(&(object->barrers[id]));

        if (object->writeArray[id] == "") {
            // std::cout << "write thread " << id << " wait" << std::endl;
            pthread_barrier_wait(&(object->barrers[MININDEX(id, object->n)]));
            break;
        }
    }
    // std::cout << "write thread " << id << " done" << std::endl;
    delete (Arg*)args;
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
