INCDIR=./inc/
SRCDIR=./src/
OBJDIR=./obj/
LIBDIR=./lib/
BINDIR=./bin/

vpath %.cpp $(SRCDIR)
vpath %.hpp $(INCIDR)

CXX?=clang++
CPPFLAGS?=-std=c++11 -pthread
EXE?=$(BINDIR)citadel

LDFLAGS?=-fopenmp -lpthread -lGL -lGLU \
		 -lsfml-system -lsfml-graphics 


INCS=$(wildcard $(INCDIR)*.hpp)
SRCS=$(wildcard $(SRCDIR)*.cpp)
OBJS=$(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRCS))

all:$(EXE)

$(OBJDIR)%.o:$(SRCDIR)%.cpp 
	@echo Compiling $@
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(EXE):$(OBJS) $(INCS)
	@echo Linking $@
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

check:
	@echo "TODO: Check dependencies
	@$(CXX) --version

clean:
	@rm -f $(EXE) $(OBJS)

do:$(EXE)
	@echo "Running $(EXE)"
	@$(EXE)

.PHONY: all clean check do
