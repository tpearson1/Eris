CFLAGS:=-g -Og -Wall -Wextra -Wfatal-errors $(OPTFLAGS)
SOURCES:=$(wildcard $(NAME)/src/**/*.cpp $(NAME)/src/*.cpp)
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))

.PHONY: all clean
.PRECIOUS: $(TARGET) $(OBJECTS)

all: $(TARGET)
	@: # Prevent make from outputting 'Nothing to be done for...'

%.o: %.cpp
	g++ $(CFLAGS) -c -MMD $< -o $@

$(TARGET): $(OBJECTS)
	-mkdir -p $(NAME)/lib
	g++ $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-find $(NAME) -type f -name '*.o' -delete
	-find $(NAME) -type f -name '*.d' -delete
	-rm -r $(NAME)/lib

-include $(NAME)/src/*.d
-include $(NAME)/src/**/*.d
