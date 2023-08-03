/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#ifndef WRITER
#define WRITER

// dummy class used to pass args to threads
class Arg {
public:
    int id;
    void *object;
};

class writer {
public:
    // initaliser
    writer(const std::string outfile, std::string *writeArray, int n, pthread_barrier_t *barrers);
    ~writer();

    // thread
    static void* runner(void* args);
    // thread runner
    void run();
    // number of threads
    int n;
private:
    // atributes
    std::ofstream out;
    std::string *writeArray;
    pthread_barrier_t *barrers;
    pthread_t *threads;
};
#endif
