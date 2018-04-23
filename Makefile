CC=cc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.c launch.c  pipeline.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=launch

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clear:
	rm *.o launch
