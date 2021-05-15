INC_DIR := ./include
SRC_DIR :=  ./src/threadPool ./src/netlib
CC := g++
CPPFLAGS := -g
LIBS := -lpthread
INC_FILE := $(addprefix -I, $(INC_DIR))
CPP_FILE := $(wildcard ./src/threadPool/*.cc)  $(wildcard ./src/netlib/*.cc) 

OBJS := $(patsubst %.cc , %.o, $(CPP_FILE))


TARGET := dog.exe

$(TARGET):$(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^  $(INC_FILE) $(LIBS)

%.o:%.cc
	$(CC) $(CPPFLAGS) -o $@ -c $< $(INC_FILE) $(LIBS)

object:
	@echo $(OBJS)

clean:
	rm -rf $(TARGET) $(OBJS)
