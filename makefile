CC = g++
LFLAGS = -lncurses
CFLAGS = -ggdb -std=gnu++17 -Wall -MMD
BDIR = build/
RDIR = release/
SOURCES = $(wildcard ./*.cpp)
OBJECTS = $(addprefix $(BDIR),$(notdir $(SOURCES:.cpp=.o)))
EXECUTABLE=$(RDIR)exe

all: $(SOURCES) processor $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS) 
	$(CC) $(CFLAGS) $(LFLAGS) $(BDIR)data_store_base.o $(OBJECTS) -o $@

processor:
	$(MAKE) -C data_store

$(BDIR)%.o: ./%.cpp
	g++ $(CFLAGS) -c -o $@ $<

clean :
	rm $(BDIR)*.o $(BDIR)*.d $(RDIR)exe process $(RDIR)*.dat

-include $(OBJECTS:%.o=%.d)
