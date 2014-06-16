BUILDDIR    = ./bin/
INCLUDEDIR  = ./include/
SOURCEDIR   = ./

DEFINES     =

SDIRS       = $(wildcard $(SOURCEDIR)*/)
VPATH       = $(SOURCEDIR):$(SDIRS):$(foreach dir, $(SDIRS), $(wildcard $(dir)*/))

IFILES      = $(shell find $(SOURCEDIR) -name '*.cpp')
IFILES     := $(IFILES)

OFILES      = $(subst $(SOURCEDIR),  $(BUILDDIR), $(addsuffix .o, $(notdir $(shell find $(SOURCEDIR)  -name '*.cpp'))))
OFILES     := $(OFILES)

CC          = g++
CCFLAGS     = -c -O3 -std=c++0x -I$(INCLUDEDIR)
LINKFLAGS   = -lboost_system -lboost_filesystem


TARGET = debate-duplicates

all: $(TARGET)

$(TARGET): $(foreach file, $(OFILES), $(BUILDDIR)$(file))
	$(CC) $^ $(LINKFLAGS) -o $@

$(BUILDDIR)%.cpp.o: %.cpp
	$(CC) $(foreach def, $(DEFINES), -D $(def)) $(CCFLAGS) $< -o $@


.PHONY: clean
clean:
	rm $(BUILDDIR)*
	rm $(TARGET)