INCDIR=./inc/
SRCDIR=./src/
ASMDIR=./asm/
OBJDIR=./obj/
LIBDIR=./lib/
BINDIR=./bin/

vpath %.cpp $(SRCDIR)
vpath %.hpp $(INCIDR)

CXX?=g++
CPPFLAGS?=-std=c++11 -pthread
EXE?=$(BINDIR)glomp

LDFLAGS?=-fopenmp -lpthread \
		 -lGL -lGLU -lGLEW \
		 -lsfml-graphics -lsfml-window -lsfml-system 

ASMFLAGS?=-fverbose-asm -g -masm=intel -S
LSTFLAGS?=-alhnd

INCS=$(wildcard $(INCDIR)*.hpp)
SRCS=$(wildcard $(SRCDIR)*.cpp)
OBJS=$(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRCS))
ASMS=$(patsubst $(SRCDIR)%.cpp,$(ASMDIR)%.s,$(SRCS))
LSTS=$(patsubst $(SRCDIR)%.cpp,$(ASMDIR)%.lst,$(SRCS))

all:$(EXE)

$(OBJDIR)%.o:$(SRCDIR)%.cpp 
	@echo Compiling $@
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(EXE):$(OBJS) $(INCS)
	@echo Linking $@
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(ASMDIR)%.s:$(SRCDIR)%.cpp
	@echo Generating assembly $@ from $<
	$(CXX) $(CPPFLAGS) $(ASMFLAGS) -o $@ $<

$(ASMDIR)%.lst:$(ASMDIR)%.s
	@echo Interleaving $< with source
	as $(LSTFLAGS) $< > $@

asm:$(LSTS) $(ASMS)
	@rm a.out
## TODO depend without generating a.out

clean:
	@rm -f $(EXE) $(OBJS) $(ASMS) $(LSTS)

do:$(EXE)
	@echo "Running $(EXE)"
	@$(EXE)

.PHONY: all asm clean do
