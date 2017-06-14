MOD_PATH:=../../$(MOD)
INCLUDE_DIRS:=$(INCLUDE) -I../include -I$(MOD_PATH)/include/$(MOD) -I$(MOD_PATH)/include
CFLAGS:=-g -Og -Wall -Wextra -Werror -Wfatal-errors
PREFIX?=/usr/local

SOURCES:=$(wildcard $(MOD_PATH)/tests/**/*.cpp $(MOD_PATH)/tests/*.cpp)
SOURCES+=generatedmain.cpp

OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))

TARGET:=$(MOD_PATH)/tests/tests

.PHONY: all clean
.PRECIOUS: $(TARGET) $(OBJECTS)

all: $(TARGET)
	@: # Prevent make from outputting 'Nothing to be done for...'

%.o: %.cpp $(HEADERS)
ifndef SILENT
	$(info $<)
endif
	g++ $(CFLAGS) $(INCLUDE_DIRS) -c -MMD $< -o $@

$(TARGET): $(OBJECTS)
# If there are no files in the testing directory, we do not need to build
ifneq ($(OBJECTS), generatedmain.o)
	g++ $(OBJECTS) -Wall $(LIBS) -o $@
endif

clean:
	-find $(MOD_PATH)/tests -type f -name '*.o' -delete
	-find $(MOD_PATH)/tests -type f -name '*.d' -delete
	-rm $(MOD_PATH)/tests/tests

-include $(MOD_PATH)/tests/*.d
-include $(MOD_PATH)/**/*.d
-include generatedmain.d
