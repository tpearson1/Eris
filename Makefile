CXXFLAGS:=-g -Og -Wall -Wextra -Werror -Wfatal-errors -Isrc/include -Imods/core/include
LDFLAGS:=-Wall
LIBS:=-ldl -lGLEW -lGL -lglfw -lpng mods/core/libcore.so

BUILD_DIR:=build

SOURCES:=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS:=$(patsubst src/%.cpp,$(BUILD_DIR)/%.cpp.o,$(SOURCES))

TARGET:=load

.PHONY: all build-core clean clean-mods clean-mod
.PRECIOUS: $(TARGET) $(OBJECTS)

all: build-core $(TARGET)

build-core:
	cd mods/core; $(MAKE) $(J)

$(BUILD_DIR)/%.cpp.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	g++ $(CXXFLAGS) -c -MMD $< -o $@

$(TARGET): $(OBJECTS)
	cd mods/core; $(MAKE) $(J)
	g++ $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@

clean: clean-mods
	-rm -r $(BUILD_DIR)
	-rm $(TARGET)

clean-mods:
	-rm -r mods/**/lib**.so
	-rm -r mods/**/$(BUILD_DIR)

clean-mod:
	-rm -r mods/$(MOD)/lib$(MOD).so
	-rm -r mods/$(MOD)/$(BUILD_DIR)

-include $(BUILD_DIR)/*.d
-include $(BUILD_DIR)/**/*.d

