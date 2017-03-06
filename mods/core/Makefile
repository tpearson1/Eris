INCLUDE_DIRS:=-Iinclude/core -Iinclude -I../rapidjson/include
CFLAGS:=-g -Og -Wall -Wextra -Werror -Wfatal-errors -fpic $(INCLUDE_DIRS) $(OPTFLAGS)
LDFLAGS:=-shared
LIBS:=$(OPTLIBS) -ldl -lGLEW -lGL -lglfw -lpng
PREFIX?=/usr/local

SOURCES:=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))

TARGET:=lib/libcore.so

.PHONY: all clean
.PRECIOUS: $(TARGET) $(OBJECTS)

all: $(TARGET)
	@: # Prevent make from outputting 'Nothing to be done for...'

%.o: %.cpp $(HEADERS)
	g++ $(CFLAGS) -c -MMD $< -o $@

$(TARGET): $(OBJECTS)
	-mkdir -p lib
	g++ $(OBJECTS) -Wall $(LDFLAGS) $(LIBS) -o $@

clean:
	-find . -type f -name '*.o' -delete
	-find . -type f -name '*.d' -delete
	-rm -r lib

-include src/*.d
-include src/**/*.d
