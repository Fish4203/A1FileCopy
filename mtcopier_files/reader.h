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
    reader(const std::string infile, std::deque<std::string> *queue);
    ~reader();
    /* perform the reading from the file */
    static void* runner(void* args);

    void run();



   private:
    std::ifstream in;
    std::deque<std::string> *queue;
};
#endif
