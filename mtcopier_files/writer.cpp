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
    this->out << this->writeArray[this->n];
    // closing the file
    this->out.close();
    // closing the write threads
     for (int i = 0; i < this->n; i++) {
         pthread_join(this->threads[i], NULL);
     }
     delete this->threads;
}

void *writer::runner(void * args) {
    int id = (*(Arg*)args).id; // id of the thread
    writer *object = (writer*)(*(Arg*)args).object; // basicaly this
    // preventing the 1 thread from writing an empty line at the start of the file
    if (id == 1) {
        pthread_barrier_wait(&(object->barrers[0]));
        pthread_barrier_wait(&(object->barrers[1]));
    }

    while (true) {
        // waiting for the -1 threads to finish
        pthread_barrier_wait(&(object->barrers[MININDEX(id, object->n)]));

        // if the program has stoped and sll the writing is done
        if (!(*object->runing) && object->writeArray[id].length() == CHARCOUNT) {
            // signaling the next threads to runn
            pthread_barrier_wait(&(object->barrers[id]));
            break;
        }
        // writing id -1 line to the file
        object->out << object->writeArray[MININDEX(id, object->n)];

        // signaling the next threads to runn
        pthread_barrier_wait(&(object->barrers[id]));

        // if the id read thread did the last read then wait for id -1 threads to finish
        if (object->writeArray[id].length() != CHARCOUNT) {
            pthread_barrier_wait(&(object->barrers[MININDEX(id, object->n)]));
            break;
        }
    }
    // delete the args object
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
