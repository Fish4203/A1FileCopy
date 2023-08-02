
##
 # startup code provided by Paul Miller for COSC1114 - Operating Systems
 # Principles
 ##

# provide make targets here to build the two programs


.default: all

all: copy

clean:
	rm -rf copy *.o *.dSYM

copy: writer.o reader.o main.o
	g++ -Wall -Werror -std=c++20 -g -O -o $@ $^
	# g++ -std=c++14 -g -O -o $@ $^

%.o: mtcopier_files/%.cpp
	g++ -Wall -Werror -std=c++20 -g -O -c $^
	# g++ -std=c++14 -g -O -c $^
