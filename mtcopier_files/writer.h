/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <fstream>
#include <iostream>
#include <string>
#include <deque>
#include <unistd.h>
#ifndef WRITER
#define WRITER

class Arg {
public:
    int id;
    void *object;
};

class writer {
   public:
    /**
     * creates the writer instance that writes out to the file
     **/
    writer(const std::string outfile, std::string *writeArray, int n, pthread_barrier_t *barrersRead, pthread_barrier_t *barrersWrite);
    ~writer();
    static void* runner(void* args);
    void run();

    int n;
   private:
    std::ofstream out;
    std::string *writeArray;
    bool running;
    pthread_barrier_t *barrersRead;
    pthread_barrier_t *barrersWrite;
};
#endif
