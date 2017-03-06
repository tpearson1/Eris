CFLAGS:=-g -Og -Wall -Wextra -Werror -Wfatal-errors -Isrc/include -Imods/core/include $(OPTFLAGS)
LDFLAGS?=-Wl,-rpath=\$$ORIGIN/mods/core/lib
LIBS:=$(OPTLIBS) -ldl -lGLEW -lGL -lglfw -lpng mods/core/lib/libcore.so
PREFIX?=/usr/local

SOURCES:=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))

TARGET:=load

.PHONY: all build-core clean clean-mods
.PRECIOUS: $(TARGET) $(OBJECTS)

all: $(TARGET)

build-core:
	cd mods/core; $(MAKE) $(J)

%.o: %.cpp $(HEADERS)
	g++ $(CFLAGS) -c -MMD $< -o $@

$(TARGET): build-core $(OBJECTS)
	g++ $(OBJECTS) -Wall $(LDFLAGS) $(LIBS) -o $@

clean: clean-mods
	-find ./src -type f -name '*.o' -delete
	-find ./src -type f -name '*.d' -delete
	-rm $(TARGET)

clean-mods:
	-rm -r mods/*/lib
	-find mods/* -type f -name '*.o' -delete
	-find mods/* -type f -name '*.d' -delete

clean-mod:
	-rm -r mods/$(mod)/lib
	-find mods/$(mod) -type f -name '*.o' -delete
	-find mods/$(mod) -type f -name '*.d' -delete

-include src/*.d
-include src/**/*.d
