/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include "writer.h"
#include <time.h>
#include <cstdlib>
/* global variables if needed go here */
int main(int argc, char** argv) {
    clock_t start = clock();

    bool t = false;
    if (argc != 3) {
        if (argc == 4) {
            t =true;
        } else {
            std::cout << "wrong number of inputs" << std::endl;
            return 1;
        }
    }
    /* check command line arguments */
    /* load the file and copy it to the destination */
    std::string infile = argv[1];
    std::string outfile = argv[2];

    std::deque<std::string> *queue = new std::deque<std::string>;

    reader read = reader(infile, queue);
    writer write = writer(outfile, queue);
    bool running = true;

    while (running) {
        running = read.run();
        write.run();
    }

    delete queue;

    if (t) {
        std::cout << "time taken" << (clock() - start) / (double)CLOCKS_PER_SEC << std::endl;

    }
    return EXIT_SUCCESS;
}
