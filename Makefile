CC=clang
CXX=clang++
MKDIR=@mkdir -p

DIR_ROOT?=
#DIR_DEBUG?=$(DIR_ROOT)debug/
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
MAIN_MODULES?=util system/net system math math/affine math/quat math/dual\
			  model/ply model view view/shade
MAIN_H_ONLY?=util/types math/quat math/dual
MAIN_INCLUDES?=$(foreach inc,$(MAIN_H_ONLY) $(MAIN_MODULES),\
			   $(inc:%=%.hpp))
MAIN_PCHS?=$(DIR_ROOT_INCLUDE)util.hpp$(PCH_EXT)
MAIN_SRCS?=$(foreach mod,$(MAIN_MODULES) main,$(DIR_ROOT_SRC)$(mod).cpp)
MAIN_OBJS=$(foreach mod,$(MAIN_MODULES),$(mod:%=$(DIR_ROOT_LIB)%.o))
MAIN_DEPS=$(MAIN_OBJS:%.o=%.d)

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
CPPFLAGS=-std=c++11 -pthread -fopenmp=libomp $(CFLAGS)\
		 -I$(DIR_ROOT_INCLUDE)
LDFLAGS=-L$(DIR_BOOST_LIB) -lpthread -lboost_system
RELEASE_LDFLAGS=$(LDFLAGS) -lGL -lglfw -ldl
#DEBUG_LDFLAGS=$(LDFLAGS)
TEST_LDFLAGS=$(LDFLAGS) -lboost_unit_test_framework -lGL -lglfw -ldl
###############################################################################
LINK_CC=$(CC) $(CFLAGS) $(WFLAGS)
LINK_CXX=$(CXX) $(CPPFLAGS) $(WFLAGS)
COMPILE_CC=$(LINK_CC) -c
COMPILE_CXX=$(LINK_CXX) -include $(DIR_ROOT_INCLUDE)util.hpp -c
DEPEND_CXX=$(LINK_CXX) -M
COMPILE_HPP=$(LINK_CXX) -x c++-header
###############################################################################

default:.sentinel $(MAIN_PCHS) $(RELEASE_EXE)
all:.sentinel $(MAIN_PCHS) $(EXES)
vpath %.hpp $(DIR_ROOT_INCLUDE)
vpath %.cpp $(DIR_ROOT_SRC)

-include $(MAIN_DEPS)

$(RELEASE_EXE): $(RELEASE_OBJ) $(MAIN_OBJS) $(GL3W_OBJS) $(MAIN_PCHS)
	$(LINK_CXX) $(RELEASE_LDFLAGS) $< $(GL3W_OBJS) $(MAIN_OBJS) -o $@
$(TEST_EXE): $(TEST_OBJ) $(MAIN_OBJS) $(GL3W_OBJS)
	$(LINK_CXX) $(TEST_LDFLAGS) $< $(MAIN_OBJS) $(GL3W_OBJS) -o $@

$(RELEASE_OBJ): $(DIR_ROOT_SRC)main.cpp $(MAIN_PCHS) $(MAIN_INCLUDES)
	$(COMPILE_CXX) $< -o $@
	$(DEPEND_CXX) $< -o $(@:%.o=%.d)
# --log_level=error
$(TEST_OBJ): $(DIR_TEST)$(DIR_SRC)*.cpp
	$(COMPILE_CXX) $< -o $@
	$(DEPEND_CXX) $< -o $(@:%.o=%.d)


$(DIR_GL3W)%.o: $(DIR_GL3W)$(DIR_SRC)*.c ; $(COMPILE_CC) -o $@ $<
$(DIR_ROOT_LIB)%.o: $(DIR_ROOT_SRC)%.cpp $(DIR_ROOT_INCLUDE)%.hpp
	$(COMPILE_CXX) $< -o $@
	$(DEPEND_CXX) $< -o $(@:%.o=%.d)
$(DIR_ROOT_LIB)*/%.o: $(DIR_ROOT_SRC)*/%.cpp
	$(COMPILE_CXX) $< -o $@
	$(DEPEND_CXX) $< -o $(@:%.o=%.d)

%.hpp$(PCH_EXT): %.hpp
	$(COMPILE_HPP) $< -o $@

.sentinel:
	$(MKDIR) $(foreach target,$(EXES) $(EXE_OBJS) $(MAIN_OBJS),\
		$(dir $(target)))
	@touch .sentinel

clean-depends:
	@rm -f $(MAIN_DEPS)

clean:
	@rm -f $(EXES) $(EXE_OBJS) $(MAIN_OBJS)\
		$(MAIN_PCHS) $(EXE_OBJS:.o=.d) $(MAIN_OBJS:.o=.d) .sentinel

.PHONY: all depends depends-clean clean
