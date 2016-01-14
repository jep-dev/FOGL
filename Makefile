INCDIR=./inc/
SRCDIR=./src/
OBJDIR=./obj/
LIBDIR=./lib/
BINDIR=./bin/

vpath %.cpp $(SRCDIR)
vpath %.hpp $(INCIDR)

# CXX?=clang++
CXX?=g++
CPPFLAGS?=-std=c++11 -pthread
EXE?=$(BINDIR)citadel
WFLAGS?=# -Wno-non-template-friend
LDFLAGS?=-fopenmp -lpthread \
		 -lGL -lGLU \
		 -lsfml-system -lsfml-graphics -lsfml-window


INCS=$(wildcard $(INCDIR)*.hpp)
SRCS=$(wildcard $(SRCDIR)*.cpp)
OBJS=$(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRCS))

all:$(EXE)

$(OBJDIR)%.o:$(SRCDIR)%.cpp 
	@echo Compiling $@
	$(CXX) $(CPPFLAGS) $(WFLAGS) -c -o $@ $<

$(EXE):$(OBJS) $(INCS)
	@echo Linking $@
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

clean:
	@rm -f $(EXE) $(OBJS)

do:$(EXE)
	@echo "Running $(EXE)"
	@$(EXE)

.PHONY: all clean do
