
##
 # startup code provided by Paul Miller for COSC1114 - Operating Systems
 # Principles
 ##

# provide make targets here to build the two programs


.default: all

all: copier mtcopier

clean: cclean mtclean

cclean:
	rm -rf copier copier_files/*.o copier_files/*.dSYM

mtclean:
	rm -rf mtcopier mtcopier_files/*.o mtcopier_files/*.dSYM

copier: copier_files/writer.o copier_files/reader.o copier_files/main.o
	g++ -Wall -Werror -std=c++20 -g -O -o $@ $^

copier_files/%.o: %.cpp
	g++ -Wall -Werror -std=c++20 -g -O -c copier_files/$^

mtcopier: mtcopier_files/writer.o mtcopier_files/reader.o mtcopier_files/main.o
	g++ -Wall -Werror -std=c++20 -lpthread -g -O -o $@ $^

mtcopier_files/%.o: %.cpp
	g++ -Wall -Werror -std=c++20 -g -O -c mtcopier_files/$^
