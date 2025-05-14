######################## SYSTEMC MAKEFILE ##########################
# Please change SRC (your input *.cpp file)                        #
####################################################################

#! /bin/zsh

SRC=GCD_USER.cpp

OBJ = $(SRC:.cpp=.o)
OUT = $(basename $(firstword $(SRC)))
CXX = g++
CXXFLAGS = -I. -I$(SYSTEMC_HOME)/include
LDFLAGS = -L. -L$(SYSTEMC_HOME)/lib -Wl,-rpath=$(SYSTEMC_HOME)/lib
LDLIBS = -lsystemc -lm
all: $(OUT)

$(OUT): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)

exec:
	./$(OUT)
show:
	gtkwave -S gtkwave.tcl &

####################################################################
#                                                                  #
####################################################################

