CC = g++
CFLAGS = -ggdb -std=gnu++17 -Wall -Wno-switch -MMD
BDIR = ../build/
EXECUTABLE=../process
OBJECTS=../build/processor.o ../build/data_store_base.o

$(EXECUTABLE) : processor.o data_store_base.o 
	$(CC) $(CFLAGS) -o $@ $(BDIR)processor.o $(BDIR)data_store_base.o

processor.o : processor.cpp processor.hpp 
	$(CC) $(CFLAGS) -o $(BDIR)processor.o -c processor.cpp -DDS_PROC_BUILD

data_store_base.o : data_store_base.hpp data_store_base.cpp
	$(CC) $(CFLAGS) -o $(BDIR)data_store_base.o -c data_store_base.cpp

-include $(OBJECTS:%.o=%.d)
