LDFLAGS_COMMON = -framework GLUT -framework OpenGL -framework Cocoa
CFLAGS_COMMON = -O3 -c -Wall -I./ -I./glvu/

CC         = g++
CFLAGS     = ${CFLAGS_COMMON} -O3 -Wno-unused
LDFLAGS    = ${LDFLAGS_COMMON}
EXECUTABLE = deferredViewer

SOURCES    = deferredViewer.cpp VEC3F.cpp OBJ.cpp ./glvu/glvu.cpp
OBJECTS    = $(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o
