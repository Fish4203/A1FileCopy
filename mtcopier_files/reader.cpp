#include "reader.h"

reader::reader(const std::string infile, std::string *writeArray, int n, pthread_barrier_t *barrers) {
    // initalising the verabes
    this->writeArray = writeArray;
    this->in.open(infile);
    this->n = n;
    this->barrers = barrers;
}

reader::~reader() {
    // closing the file
    this->in.close();

    // closing all the threads
    for (int i = 0; i < this->n; i++) {
        // std::cout << "read thread " << i << " kill" << std::endl;
        pthread_cancel(this->threads[i]);
    }
    delete this->threads;
}

void *reader::runner(void * args) {
    int id = (*(Arg*)args).id;
    reader *object = (reader*)(*(Arg*)args).object;
    delete (Arg*)args;
    char s[256];
    while (true) {
        // wait for turn to read
        // std::cout << "read thread " << (*(Arg*)args).id << " waiting at barrer -1" << std::endl;
        if ((*(Arg*)args).id == 0) {
            pthread_barrier_wait(&(object->barrers[object->n -1]));
        } else {
            pthread_barrier_wait(&(object->barrers[id -1]));
        }
        // read file in 256 char chunks
        // std::cout << "read thread " << (*(Arg*)args).id << " start read" << std::endl;
        if (!object->in.read(s, 256)) {
            // this is needed to read the last of the file
            object->writeArray[0] = std::string(s, object->in.gcount());
            // std::cout << "read thread " << (*(Arg*)args).id << " end" << std::endl;
            // if the file is done then signal the exit barrer
            pthread_barrier_wait(&(object->barrers[object->n]));
            // exit
            pthread_exit(NULL);
        }
        // add line to the write array at position i
        object->writeArray[id] = std::string(s, 256);

        // signaling that the next read write can begin
        // std::cout << "read thread " << (*(Arg*)args).id << " waiting at barrer" << std::endl;
        pthread_barrier_wait(&(object->barrers[id]));
    }
}

void reader::run() {
    // making the thread ids
    this->threads = new pthread_t[this->n];

    // creating the threads
    for (int i = 0; i < this->n; i++ ) {
        Arg *arg = new Arg();
        arg->id = i;
        arg->object = this;
        pthread_create(&this->threads[i], NULL, reader::runner, arg);
    }
}
