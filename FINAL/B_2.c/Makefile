SRC = dh_mod.cpp
OBJ = $(SRC:.cpp=.o)
OUT = $(basename $(firstword $(SRC)))
RSAREF_OBJS = desc.o digit.o md2c.o md5c.o nn.o prime.o \
	rsa.o r_encode.o r_dh.o r_enhanc.o r_keygen.o r_random.o r_stdlib.o
RSAREFLIB = rsaref.a
SYSTEMC_HOME = /opt/systemc

CXX = g++
CC = cc
AR = ar

CXXFLAGS = -I. -I$(SYSTEMC_HOME)/include -Wno-write-strings
LDFLAGS = -L. -L$(SYSTEMC_HOME)/lib -Wl,-rpath=$(SYSTEMC_HOME)/lib
LDLIBS = -lsystemc -lm

all: $(RSAREFLIB) $(OUT)

$(RSAREFLIB): $(RSAREF_OBJS)
	$(AR) rcs $@ $^

$(OUT): $(OBJ)
	$(CXX) -o $@ $^ $(RSAREFLIB) $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) -I. -O -c -DPROTOTYPES=1 $< -o $@

clean:
	rm -f $(OBJ) $(RSAREF_OBJS) $(RSAREFLIB) $(OUT)

exec:
	./$(OUT)

show:
	gtkwave -S gtkwave.tcl &