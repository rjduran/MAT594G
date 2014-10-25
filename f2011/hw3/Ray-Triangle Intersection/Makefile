CC         = g++
EXECUTABLE = HW3
CFLAGS     = -c -Wall -I./

SOURCES    = HW3.cpp OBJ.cpp
OBJECTS    = $(SOURCES:.cpp=.o)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
