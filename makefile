SOURCES=main.cc ReaktorMapBuilder.cc ReaktorMapBuilder_6_0_1_R1526.cc posix.cc
EXECUTABLE=reaktormap

CPPFLAGS=-std=c++14 -c
LDFLAGS=
OBJECTDIR=obj/
SOURCEDIR=src/
BINDIR=bin/

CXX=g++
RM=rm

OBJECTS=$(SOURCES:.cc=.o)
CPPSOURCES=$(addprefix $(SOURCEDIR),$(SOURCES))
CPPOBJECTS=$(addprefix $(OBJECTDIR),$(OBJECTS))
CPPEXECUTABLE=$(addprefix $(BINDIR),$(EXECUTABLE))

all: $(CPPSOURCES) $(CPPEXECUTABLE)

$(CPPEXECUTABLE): $(CPPOBJECTS)
	$(CXX) $(LDFLAGS) $(CPPOBJECTS) -o $@

$(OBJECTDIR)%.o: $(SOURCEDIR)%.cc
	$(CXX) $(CPPFLAGS) $< -o $@
