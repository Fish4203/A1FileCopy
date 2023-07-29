/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <fstream>
#include <iostream>
#include <string>
#include <deque>
#ifndef WRITER
#define WRITER
class writer {
   public:
    /**
     * creates the writer instance that writes out to the file
     **/
    writer(const std::string outfile, std::deque<std::string> *queue);
    ~writer();
    /**
     * does the actual writing
     **/
    void run();

   private:
    std::ofstream out;
    std::deque<std::string> *queue;
};
#endif
