/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "writer.h"
#include <fstream>
#include <iostream>
#include <string>
#ifndef READER
#define READER



class reader {
public:
    // initaliser
    reader(const std::string infile, std::string *writeArray, int n, pthread_barrier_t *barrers);
    ~reader();

    // thread
    static void* runner(void* args);
    // thread runner
    void run();
    // number of threads
    int n;



private:
    // atributes
    std::ifstream in;
    std::string *writeArray;
    pthread_barrier_t *barrers;
    pthread_t *threads;
};
#endif
