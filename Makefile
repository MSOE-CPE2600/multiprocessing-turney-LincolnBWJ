CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=-ljpeg
SOURCES= mandel.c jpegrw.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mandel

all: $(EXECUTABLE) mandelmovie

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o: 
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d

mandelmovie: mandelmovie.o
	$(CC) mandelmovie.o -o mandelmovie

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d mandelmovie mandelmovie.o
