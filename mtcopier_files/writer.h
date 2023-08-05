/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <time.h>
#include <queue>
#ifndef WRITER
#define WRITER

// returns id - 1 mod n
#define MININDEX(i, n) (((i) + (n-1)) % (n))
// amount of character to get from the file at once
#define CHARCOUNT 2048
// dummy class used to pass args to threads
class Arg {
public:
    int id;
    void *object;
};

class writer {
public:
    // initaliser
    writer(const std::string outfile, std::string *writeArray, int n, pthread_barrier_t *barrers, bool *runing);
    ~writer();

    // thread
    static void* runner(void* args);
    // thread runner
    void run();
    // number of threads
    int n;
    std::queue<clock_t> Stime;
    std::queue<clock_t> Etime;
private:
    // atributes
    bool *runing;
    std::ofstream out;
    std::string *writeArray;
    pthread_barrier_t *barrers;
    pthread_t *threads;
};
#endif
