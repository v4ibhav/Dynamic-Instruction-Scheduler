CC = g++
OPT = -O3
#OPT = -g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) std=c++1y $(LIB)


# List all your .cc/.cpp files here (source files, excluding header files)
SIM_SRC =  sim_proc.cc pipeline.cc 
# sim_proc.cc fetch.cc decode.cc regRead.cc dispatch.cc issue.cc execute.cc writeback.cc retire.cc

# List corresponding compiled object files here (.o files)
SIM_OBJ =  sim_proc.o  pipeline.o 
#  sim_proc.o fetch.o decode.o regRead.o dispatch.o issue.o execute.o writeback.o retire.o
#################################

# default rule

all: sim
	@echo "my work is done here..."


# rule for making sim

sim: $(SIM_OBJ)
	$(CC) -o sim -std=c++1y $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH sim-----------"


# generic rule for converting any .cpp file to any .o file
 
.cc.o:
	$(CC) $(CFLAGS)  -c $*.cc

.cpp.o:
	$(CC) $(CFLAGS)  -c $*.cpp


# type "make clean" to remove all .o files plus the sim binary

clean:
	rm -f *.o sim


# type "make clobber" to remove all .o files (leaves sim binary)

clobber:
	rm -f *.o


