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
    /* create a reader that reads each line of the file and appends it to the
     * writer's queue
     */
    reader(const std::string infile, std::string *writeArray, int n, pthread_barrier_t *barrersRead, pthread_barrier_t *barrersWrite);
    ~reader();
    /* perform the reading from the file */
    static void* runner(void* args);

    void run();



    int n;
private:
    std::ifstream in;
    std::string *writeArray;
    pthread_barrier_t *barrersRead;
    pthread_barrier_t *barrersWrite;
};
#endif
