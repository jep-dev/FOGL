CC=clang
CXX=clang++
MKDIR=@mkdir -p
RM=@rm -f

DIR_ROOT?=
#DIR_DEBUG?=$(DIR_ROOT)debug/
DIR_TEST?=$(DIR_ROOT)test/
DIR_BOOST?=$(DIR_ROOT)../boost/
DIR_GL3W?=$(DIR_ROOT)../gl3w/

DIR_INCLUDE?=include/
DIR_SRC?=src/
DIR_BIN?=
DIR_LIB?=lib/

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
EXE_BASE?=fogl
TEST_EXT?=_test
DEBUG_EXT?=_debug
RELEASE_EXT?=

MODULE_DIRS?=util/ system/ math/ model/ view/
MAIN_MODULES?=util system/net system\
			  math math/affine math/quat math/dual\
			  model/ply model view view/shade
MAIN_H_ONLY?=util/types math/quat math/dual
MAIN_INCLUDES?=$(foreach inc,$(MAIN_H_ONLY) $(MAIN_MODULES),\
			   $(inc:%=%.hpp))

MAIN_SRCS?=$(foreach mod,$(MAIN_MODULES) main,$(DIR_ROOT_SRC)$(mod).cpp)
MAIN_OBJS=$(foreach mod,$(MAIN_MODULES),$(mod:%=$(DIR_ROOT_LIB)%.o))
MAIN_DEPS=$(MAIN_OBJS:%.o=%.d)

SENTINEL_DIRS?=$(DIR_BIN) $(foreach outer,$(DIR_LIB),\
			   $(foreach inner,. $(MODULE_DIRS),$(outer)$(inner)))

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
WFLAGS+=-Wall -Wno-unused
CFLAGS=-fPIC -fdata-sections -isystem $(DIR_GL3W_INCLUDE)
CPPFLAGS:=$(CFLAGS) -I$(DIR_ROOT_INCLUDE) \
		 -I$(DIR_GL3W_INCLUDE) -I$(DIR_BOOST_INCLUDE) \
		 -std=c++11 -pthread -fopenmp=libomp
LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(DIR_GL3W_LIB)
LDFLAGS:=-L$(DIR_BOOST_LIB) -L$(DIR_GL3W_LIB) \
	-lpthread -Wl,--gc-sections\
	-Wl,-rpath,$(DIR_BOOST_LIB):$(DIR_GL3W_LIB)\
	-lboost_system
RELEASE_LDFLAGS:=$(LDFLAGS) -lGL -lGLU -lglfw -ldl
#DEBUG_LDFLAGS=$(LDFLAGS) -lboost_system
TEST_LDFLAGS:=$(LDFLAGS) -lboost_unit_test_framework -lGL -lglfw -ldl
###############################################################################
LINK_CC=$(CC) $(CFLAGS) $(WFLAGS)
LINK_CXX=$(CXX) $(CPPFLAGS) $(WFLAGS)
COMPILE_CC=$(LINK_CC) -c
COMPILE_CXX=$(LINK_CXX) -include $(DIR_ROOT_INCLUDE)util.hpp -c
DEPEND_CC=$(LINK_CC) -M
DEPEND_CXX=$(LINK_CXX) -M
COMPILE_HPP=$(LINK_CXX) -x c++-header
###############################################################################

default:.sentinel $(MAIN_PCHS) release
all:default test #debug
vpath %.hpp $(DIR_ROOT_INCLUDE)
vpath %.cpp $(DIR_ROOT_SRC)

-include $(MAIN_DEPS)

release: $(RELEASE_EXE) ;
$(RELEASE_EXE): $(RELEASE_OBJ) $(MAIN_OBJS) $(GL3W_OBJS)
	$(LINK_CXX) -fPIE $< -lgl3w $(MAIN_OBJS) -o $@ $(RELEASE_LDFLAGS)

test: $(TEST_EXE) ; # use --log_level=error
$(TEST_EXE): $(TEST_OBJ) $(MAIN_OBJS) $(GL3W_OBJS)
	$(LINK_CXX) -fPIE $< $(MAIN_OBJS) $(GL3W_OBJS) -o $@ $(TEST_LDFLAGS)

$(DIR_GL3W)%.o: $(DIR_GL3W)$(DIR_SRC)*.c
	$(COMPILE_CC) -fPIC -o $@ $<
	$(LINK_CC) $(CFLAGS) -shared $< \
		-o $(@:$(DIR_GL3W_LIB)%.o=$(DIR_GL3W_LIB)lib%.so)

$(DIR_ROOT_LIB)%.o: $(DIR_ROOT_SRC)%.cpp $(DIR_ROOT_INCLUDE)%.hpp
	$(COMPILE_CXX) $< -o $@
	$(LINK_CXX) -I$(DIR_ROOT_INCLUDE) -shared $< -o $@
	$(DEPEND_CXX) $< -o $(@:.o=.d)

#%.hpp$(PCH_EXT): %.hpp
#	$(COMPILE_HPP) $< -o $@

.sentinel:
	$(MKDIR) $(SENTINEL_DIRS)
	@touch .sentinel

clean-deps:; $(RM) $(EXE_OBJS:.o=.d) $(MAIN_DEPS) 
clean-pchs:; $(RM) $(MAIN_PCHS)
clean-objs:; $(RM) $(EXE_OBJS) $(MAIN_OBJS)
clean-exes:; $(RM) $(EXES)
clean-sentinels:; $(RM) .sentinel
clean-gl3w:; $(RM) $(GL3W_OBJS)
clean: clean-deps clean-exes clean-objs clean-pchs clean-sentinels;

env:; @echo "$(foreach var,CC CXX CFLAGS CPPFLAGS WFLAGS\
	RELEASE_LDFLAGS TEST_LDFLAGS,\
		\r$(var) = ${$(var)}\n)"

.PHONY: all depends env release test debug \
	clean clean-exes clean-deps clean-objs clean-sentinels
