INCLUDE_DIRS:=-Iinclude/core -Iinclude -I../rapidjson/include -I../test/include
CXXFLAGS:=-g -Og -std=c++1z -Wall -Wextra -Werror -Wfatal-errors -fpic
LDFLAGS:=-Wall -shared
LIBS:=-ldl

BUILD_DIR:=build

SOURCES:=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS:=$(patsubst src/%.cpp,$(BUILD_DIR)/%.cpp.o,$(SOURCES))

TARGET:=libcore.so

.PHONY: all
.PRECIOUS: $(TARGET) $(OBJECTS)

all: $(TARGET)
	@: # Prevent make from outputting 'Nothing to be done for...'

$(BUILD_DIR)/%.cpp.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	g++ $(CXXFLAGS) $(INCLUDE_DIRS) -c -MMD $< -o $@

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@

-include $(BUILD_DIR)/*.d
-include $(BUILD_DIR)/**/*.d

