/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include <functional>

reader::reader(const std::string infile, std::deque<std::string> *queue) {
    this->queue = queue;

    this->in.open(infile);
}

reader::~reader() {
    this->in.close(); 
}

bool reader::run() {
    std::string s;

    if (getline(this->in, s)) {
        this->queue->push_back(s);
        return true;
    } else {
        this->queue->push_back(s);
        return false;
    }
}
