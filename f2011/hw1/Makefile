CC         = g++
EXECUTABLE = HW1
CFLAGS     = -c -Wall -I./

SOURCES    = HW1.cpp
OBJECTS    = $(SOURCES:.cpp=.o)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
