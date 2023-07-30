/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include <functional>
pthread_mutex_t rl;
pthread_cond_t condr[5];

class Argstr {
public:
    int id;
    reader *read;
};


reader::reader(const std::string infile, std::deque<std::string> *queue) {
    this->queue = queue;
    this->currt = 4;
    this->in.open(infile);
}

reader::~reader() {
    this->in.close();
}

void *reader::runner(void * args) {
    std::string s;
    bool run = true;

    while (run) {
        // std::cout << (*(Argstr*)args).id << "r" << std::endl;
        pthread_mutex_lock(&rl);
        // std::cout << "why" << std::endl;

        // std::cout << "r" << std::endl;
        if (getline((*(Argstr*)args).read->in, s)) {
            (*(Argstr*)args).read->queue->push_back(s);
        } else {
            (*(Argstr*)args).read->queue->push_back(s);

            // std::cout << "why" << std::endl;
            run = false;
            // std::cout << "why" << std::endl;
        }
        pthread_cond_signal(&condr[(*(Argstr*)args).id]);

        if ((*(Argstr*)args).id == 4) {
            pthread_cond_wait(&condr[0], &rl);
        } else {
            pthread_cond_wait(&condr[(*(Argstr*)args).id +1], &rl);
        }

        pthread_mutex_unlock(&rl);
    }
    pthread_cond_signal(&condr[(*(Argstr*)args).id]);
    pthread_exit(NULL);
}

void reader::run() {
    // std::cout << "r" << std::endl;

    pthread_t threads[5];
    int i;
    // void *ret_join;
    // pthread_condr_init(&condr, 0);

    for( i = 0; i < 5; i++ ) {
        Argstr *arg = new Argstr();
        arg->id = i;
        arg->read = this;
        pthread_create(&threads[i], NULL, reader::runner, arg);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
        pthread_cond_destroy(&condr[i]);
        // std::cout << "tree";
    }

    pthread_mutex_destroy(&rl);
    // std::cout << "r" << std::endl;

    // pthread_join(threads[i], &ret_join);
}
