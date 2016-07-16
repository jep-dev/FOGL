CC=clang
CXX=clang++
MKDIR=@mkdir -p

DIR_ROOT?=
DIR_TEST?=$(DIR_ROOT)test/
DIR_BOOST?=$(DIR_ROOT)../boost/
DIR_GL3W?=$(DIR_ROOT)gl3w/

DIR_INCLUDE=include/
DIR_SRC?=src/
DIR_LIB?=lib/
DIR_BIN?=bin/

DIR_ROOT_INCLUDE?=$(DIR_ROOT)$(DIR_INCLUDE)
DIR_ROOT_SRC?=$(DIR_ROOT)$(DIR_SRC)
DIR_ROOT_LIB?=$(DIR_ROOT)$(DIR_LIB)
DIR_ROOT_BIN?=$(DIR_ROOT)$(DIR_BIN)
DIR_BOOST_INCLUDE?=$(DIR_BOOST)
DIR_BOOST_LIB?=$(DIR_BOOST)stage/lib/
DIR_GL3W_INCLUDE?=$(DIR_GL3W)$(DIR_INCLUDE)
DIR_GL3W_SRC?=$(DIR_GL3W)$(DIR_SRC)
DIR_GL3W_LIB?=$(DIR_GL3W)

EXE_EXT?=
PCH_EXT?=.pch
EXE_BASE?=glomp
TEST_EXT?=-test
DEBUG_EXT?=-debug
RELEASE_EXT?=

MODULE_DIRS?=util/ system/ math/ model/ view/
MAIN_MODULES?=util system/net system math model/ply model view view/shade
MAIN_H_ONLY?=util/types math/quat math/dual math/affine
MAIN_INCLUDES?=$(foreach inc,$(MAIN_H_ONLY) $(MAIN_MODULES),\
			   $(inc:%=$(DIR_ROOT)$(DIR_INCLUDE)%.hpp))
MAIN_PCHS?=$(DIR_ROOT_INCLUDE)util.hpp$(PCH_EXT)
MAIN_SRCS?=$(foreach mod,$(MAIN_MODULES) main,$(DIR_ROOT_SRC)$(mod).cpp)
MAIN_OBJS=$(foreach mod,$(MAIN_MODULES),$(mod:%=$(DIR_ROOT_LIB)%.o))

TEST_EXE?=$(DIR_ROOT_BIN)$(EXE_BASE)$(TEST_EXT)$(EXE_EXT)
#DEBUG_EXE?=$(DIR_ROOT)$(DIR_BIN)$(EXE_BASE)$(DEBUG_EXT)$(EXE_EXT)
RELEASE_EXE?=$(DIR_ROOT_BIN)$(EXE_BASE)$(RELEASE_EXT)$(EXE_EXT)
EXES?=$(TEST_EXE) $(RELEASE_EXE)

GL3W_OBJS?=$(DIR_GL3W_LIB)gl3w.o
TEST_OBJ?=$(DIR_ROOT_LIB)main$(TEST_EXT).o
#DEBUG_OBJ=$(DIR_ROOT)$(DIR_LIB)main$(DEBUG_EXT).o
RELEASE_OBJ?=$(DIR_ROOT_LIB)main$(RELEASE_EXT).o
EXE_OBJS?=$(TEST_OBJ) $(RELEASE_OBJ)

###############################################################################
WFLAGS+=-Winvalid-pch
CFLAGS=-I$(DIR_BOOST) -I$(DIR_GL3W_INCLUDE)
CPPFLAGS=-std=c++11 -pthread -fopenmp=libomp $(CFLAGS) -I$(DIR_ROOT_INCLUDE)\
		 -include $(DIR_ROOT_INCLUDE)util.hpp
LDFLAGS=-L$(DIR_BOOST_LIB) -lpthread -lboost_system
RELEASE_LDFLAGS=-lGL -lglfw -ldl
DEBUG_LDFLAGS=$(RELEASE_LDFLAGS)
TEST_LDFLAGS=-lboost_unit_test_framework $(RELEASE_LDFLAGS)
###############################################################################
COMPILE_CC=$(CC) $(CFLAGS) $(WFLAGS) -c -o $@ $<
COMPILE_CXX=$(CXX) $(CPPFLAGS) $(WFLAGS) -c -o $@ $<
COMPILE_HPP=$(CXX) $(CPPFLAGS) $(WFLAGS) -x c++-header -o $@ $<
LINK_CC=$(CC) $(CFLAGS) $(WFLAGS) $(LDFLAGS) -o $@ $<
LINK_CXX=$(CXX) $(CPPFLAGS) $(WFLAGS) $(LDFLAGS) -o $@ $<
###############################################################################

default:.sentinel $(MAIN_PCHS) $(RELEASE_EXE)
all:.sentinel $(EXES)
vpath %.hpp $(DIR_ROOT_INCLUDE)
vpath %.cpp $(DIR_ROOT_SRC)

%.hpp$(PCH_EXT):%.hpp
	$(COMPILE_HPP)
$(RELEASE_EXE): $(RELEASE_OBJ) $(MAIN_OBJS) $(GL3W_OBJS) $(MAIN_PCHS)
	$(LINK_CXX) $(GL3W_OBJS) $(MAIN_OBJS) $(RELEASE_LDFLAGS)
# --log_level=error
$(TEST_EXE): $(TEST_OBJ) $(MAIN_OBJS) $(GL3W_OBJS)
	$(LINK_CXX) $(GL3W_OBJS) $(MAIN_OBJS) $(TEST_LDFLAGS)
#$(DEBUG_EXE): $(DEBUG_OBJ) $(MAIN_OBJS) $(GL3W_OBJS)
#	$(LINK_CXX) $(GL3W_OBJS) $(MAIN_OBJS) $(TEST_LDFLAGS)
$(DIR_GL3W)%.o: $(DIR_GL3W)$(DIR_SRC)*.c
	$(COMPILE_CC)
$(TEST_OBJ): $(DIR_TEST)$(DIR_SRC)*.cpp
	$(COMPILE_CXX)
$(DIR_ROOT_LIB)%.o: $(DIR_ROOT_SRC)%.cpp
	$(COMPILE_CXX)
$(DIR_ROOT_LIB)*/%.o: $(DIR_ROOT_SRC)*/%.cpp
	$(COMPILE_CXX)

.sentinel:
	$(MKDIR) $(foreach target,$(EXES) $(EXE_OBJS) $(MAIN_OBJS),\
		$(dir $(target)))
	@touch .sentinel

clean:
	@rm -f $(EXES) $(EXE_OBJS) $(MAIN_OBJS) $(MAIN_PCHS) .sentinel

.PHONY: all $(MAIN_MODULES) clean
