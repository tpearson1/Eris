MOD_PATH:=../../$(MOD)
BUILD_DIR:=$(MOD_PATH)/tests/build

INCLUDE_DIRS:=$(INCLUDE) -I../include -I$(MOD_PATH)/include/$(MOD) -I$(MOD_PATH)/include
CXXFLAGS:=-g -Og -Wall -Wextra -Werror -Wfatal-errors
LINK_FLAGS:=-Wall

SOURCES:=$(wildcard $(MOD_PATH)/tests/**/*.cpp $(MOD_PATH)/tests/*.cpp)

OBJECTS:=$(patsubst $(MOD_PATH)/tests/%.cpp,$(BUILD_DIR)/%.cpp.o,$(SOURCES))

TARGET:=$(MOD_PATH)/tests/tests

.PHONY: all
.PRECIOUS: $(TARGET) $(OBJECTS)

all: $(TARGET)
	@: # Prevent make from outputting 'Nothing to be done for...'

generatedmain.o: generatedmain.cpp
ifndef SILENT
	$(info Compiling test runner)
endif
	g++ $(CXXFLAGS) -I../include -c -MMD $< -o $@

$(BUILD_DIR)/%.cpp.o: $(MOD_PATH)/tests/%.cpp
ifndef SILENT
	$(info $<)
endif
	@mkdir -p $(BUILD_DIR)
	g++ $(CXXFLAGS) $(INCLUDE_DIRS) -c -MMD $< -o $@

$(TARGET): $(OBJECTS) generatedmain.o
# If there are no files in the testing directory, we do not need to build
ifneq ($(OBJECTS),)
ifndef SILENT
	$(info Linking)
endif
	g++ $(OBJECTS) generatedmain.o $(LINK_FLAGS) $(LIBS) -o $@
endif

-include $(BUILD_DIR)/*.d
-include $(BUILD_DIR)/**/*.d
-include generatedmain.d

