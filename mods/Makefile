CXXFLAGS:=-g -Og -std=c++1z -Wall -Wextra -Wfatal-errors -fpic
INCLUDE_DIRS:=$(INCLUDE) -I$(MOD)/include/$(MOD) -I$(MOD)/include -I$(MOD)/src/include

BUILD_DIR:=$(MOD)/build

SOURCES:=$(wildcard $(MOD)/src/**/*.cpp $(MOD)/src/*.cpp)
OBJECTS:=$(patsubst $(MOD)/src/%.cpp,$(BUILD_DIR)/%.cpp.o,$(SOURCES))

LINK_FLAGS:=-Wall -shared $(LIBS)

.PHONY: all
.PRECIOUS: $(TARGET) $(OBJECTS)

all: $(TARGET)
	@: # Prevent make from outputting 'Nothing to be done for...'

$(BUILD_DIR)/%.cpp.o: $(MOD)/src/%.cpp
ifndef SILENT
	$(info $<)
endif
	@mkdir -p $(BUILD_DIR)
	g++ $(CXXFLAGS) $(INCLUDE_DIRS) -c -MMD $< -o $@

$(TARGET): $(OBJECTS)
ifndef SILENT
	$(info Linking)
endif
	g++ $(OBJECTS) $(LINK_FLAGS) -o $@

-include $(BUILD_DIR)/*.d
-include $(BUILD_DIR)/**/*.d

