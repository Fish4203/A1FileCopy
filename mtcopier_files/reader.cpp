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

    // std::cout << "read down time" << this->downtime / (double)CLOCKS_PER_SEC << std::endl;
}

void *reader::runner(void * args) {
    int id = (*(Arg*)args).id; // the id of the thread
    reader *object = (reader*)(*(Arg*)args).object; // basicaly this
    char s[CHARCOUNT];
    while (true) {
        // wait for turn id - 1 thread to finish
        pthread_barrier_wait(&(object->barrers[MININDEX(id, object->n)]));
        object->Stime.push(clock());
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
            // keeping track of the last write
            object->writeArray[id] = "";
            // tells the other threads to stop
            (*object->runing) = false;

            object->Etime.push(clock());

            // signals the next thread to start
            pthread_barrier_wait(&(object->barrers[id]));
            // waits for the previous thread to finish
            pthread_barrier_wait(&(object->barrers[MININDEX(id, object->n)]));
            break;
        } else {
            // add line to the write array at position id
            object->writeArray[id] = std::string(s, CHARCOUNT);
        }

        object->Etime.push(clock());
        // signaling that the next threads can begin
        pthread_barrier_wait(&(object->barrers[id]));
    }
    // waiting for all the threads to finish
    pthread_barrier_wait(&(object->barrers[object->n]));
    // deleat the args object
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
