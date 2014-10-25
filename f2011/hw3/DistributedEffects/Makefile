CC         = g++
EXECUTABLE = main
CFLAGS     = -c -Wall -I./

SOURCES    = main.cpp
OBJECTS    = $(SOURCES:.cpp=.o)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
