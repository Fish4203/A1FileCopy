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
class writer {
   public:
    /**
     * creates the writer instance that writes out to the file
     **/
    writer(const std::string outfile, std::deque<std::string> *queue);
    ~writer();
    static void* runner(void* args);
    void run();

   private:
    std::ofstream out;
    std::deque<std::string> *queue;
};
#endif
