/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <pthread.h>
#include "reader.h"
#include "writer.h"
#include <cstdlib>
/* global variables if needed go here */

int main(int argc, char** argv) {
    /* check command line arguments */
    /* load the file and copy it to the destination */
    std::string infile = argv[1];
    std::string outfile = argv[2];

    std::deque<std::string> *queue = new std::deque<std::string>;

    reader *read = new reader(infile, queue);
    writer *write = new writer(outfile, queue);

    write->run();
    read->run();

    while (!queue->empty()) {
        usleep(1);
    }

    delete read;
    delete write;
    delete queue;


    return EXIT_SUCCESS;
}
