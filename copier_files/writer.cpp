/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/

#include "writer.h"

/**
 * provide your implementation for the writer functions here
 **/
writer::writer(const std::string outfile, std::deque<std::string> *queue) {
    this->queue = queue;

    this->out.open(outfile);
}

void writer::run() {
    this->out << this->queue->front() << std::endl;
    this->queue->pop_front();
}
