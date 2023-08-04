#include "reader.h"


reader::reader(const std::string infile, std::string *writeArray, int n, pthread_barrier_t *barrers, bool *runing) {
    // initalising the verabes
    this->runing = runing;
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
        // std::cout << "read thread " << i << " join" << std::endl;
        pthread_join(this->threads[i], NULL);
    }
    delete this->threads;
}

void *reader::runner(void * args) {
    int id = (*(Arg*)args).id; // the id of the thread
    reader *object = (reader*)(*(Arg*)args).object;
    char s[CHARCOUNT];
    while (true) {
        // wait for turn id - 1 thread to finish
        pthread_barrier_wait(&(object->barrers[MININDEX(id, object->n)]));

        // brake if the program is done
        if (!(*object->runing)) {
            pthread_barrier_wait(&(object->barrers[id]));
            break;
        }

        // read file in 256 char chunks
        if (!object->in.read(s, CHARCOUNT)) {
            // when the file ends
            // this is needed to read the last of the file it goes to a specal spot 
            object->writeArray[object->n] = std::string(s, object->in.gcount());
            object->writeArray[id] = "";
            (*object->runing) = false;

            pthread_barrier_wait(&(object->barrers[id]));

            std::cout << "read thread " << id << " eof" << std::endl;
            pthread_barrier_wait(&(object->barrers[MININDEX(id, object->n)]));
            // std::cout << "read thread " << id << " free" << std::endl;


            break;
        } else {
            // add line to the write array at position i
            object->writeArray[id] = std::string(s, CHARCOUNT);
        }

        // signaling that the next read write can begin
        // std::cout << "read thread " << (*(Arg*)args).id << " waiting at barrer" << std::endl;
        pthread_barrier_wait(&(object->barrers[id]));
    }
    pthread_barrier_wait(&(object->barrers[object->n]));
    // std::cout << "read thread " << id << " done" << std::endl;
    delete (Arg*)args;
    pthread_exit(NULL);
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
