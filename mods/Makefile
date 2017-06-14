CFLAGS:=-g -Og -Wall -Wextra -Wfatal-errors -fpic
INCLUDE_DIRS:=$(INCLUDE) -I$(MOD)/include/$(MOD) -I$(MOD)/include -I$(MOD)/src/include
SOURCES:=$(wildcard $(MOD)/src/**/*.cpp $(MOD)/src/*.cpp)
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))

LINK_FLAGS:=-shared $(LIBS)

.PHONY: all clean
.PRECIOUS: $(TARGET) $(OBJECTS)

all: $(TARGET)
	@: # Prevent make from outputting 'Nothing to be done for...'

%.o: %.cpp
ifndef SILENT
	$(info $<)
endif
	g++ $(CFLAGS) $(INCLUDE_DIRS) -c -MMD $< -o $@

$(TARGET): $(OBJECTS)
	-mkdir -p $(MOD)/lib
	g++ $(OBJECTS) -Wall $(LINK_FLAGS) -o $@

clean:
	-find $(MOD) -type f -name '*.o' -delete
	-find $(MOD) -type f -name '*.d' -delete
	-rm -r $(MOD)/lib

-include $(MOD)/src/*.d
-include $(MOD)/src/**/*.d
