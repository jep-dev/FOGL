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

EXE_EXT?=.exe
OBJ_EXT=.o
DLL_EXT?=.so
DEP_EXT?=.d
PCH_EXT?=.pch
EXE_BASE?=fogl
TEST_EXT?=_test
#DEBUG_EXT?=_debug
RELEASE_EXT?=

MODULE_DIRS?=util/ system/ math/ model/ view/
MAIN_MODULES?=util math model view control
SYSTEM_SUBMODULES?=net printer
MATH_SUBMODULES?=affine quat dual
VIEW_SUBMODULES?=shade input
MODEL_SUBMODULES?=ply obj
MAIN_SUBMODULES?=$(SYSTEM_SUBMODULES) $(MATH_SUBMODULES)\
		$(VIEW_SUBMODULES) $(MODEL_SUBMODULES) $(MAIN_MODULES)
# TODO automate this too?
MAIN_SUBMODULE_PATHS?=\
		$(foreach mod,$(SYSTEM_SUBMODULES),system/$(mod))\
		$(foreach mod,$(MATH_SUBMODULES),math/$(mod))\
		$(foreach mod,$(VIEW_SUBMODULES),view/$(mod))\
		$(foreach mod,$(MODEL_SUBMODULES),model/$(mod))
#MAIN_SUBMODULES?=system/net\
				view/shade view/input\
				math/affine math/quat math/dual\
				model/ply model/obj
MAIN_H_ONLY?=util/types math/quat math/dual system
MAIN_INCLUDES?=$(foreach inc,$(MAIN_H_ONLY)\
			   $(MAIN_MODULES) $(MAIN_SUBMODULE_PATHS),$(inc:%=%.hpp))

MAIN_SRCS?=$(foreach mod,$(MAIN_MODULES) $(MAIN_SUBMODULE_PATHS) main,\
	  $(DIR_ROOT_SRC)$(mod).cpp)
MAIN_OBJS=$(foreach mod,$(MAIN_MODULES) $(MAIN_SUBMODULE_PATHS),\
	  $(mod:%=$(DIR_ROOT_LIB)%$(OBJ_EXT)))
MAIN_DLL_DIRS=-L$(DIR_ROOT_LIB) $(foreach dir,$(MODULE_DIRS),\
		  -L$(DIR_ROOT_LIB)$(dir))
#MAIN_DLLS=$(foreach dir,./ $(MODULE_DIRS),\
		  $($(wildcard $(DIR_ROOT_SRC)$(dir)*.cpp):\
		  $(DIR_ROOT_SRC)$(dir)%.cpp=$(DIR_ROOT_LIB)$(dir)lib%$(DLL_EXT)))
MAIN_DLLS=$(foreach mod,main $(MAIN_SUBMODULES),\
		  $(DIR_ROOT_LIB)lib$(mod)$(DLL_EXT))
#MAIN_DLL_LINKS=$(foreach dll,$(MAIN_DLLS),$(dll:lib%$(DLL_EXT)=-l%))
MAIN_DLL_LINKS=$(foreach mod,$(MAIN_MODULES) $(MAIN_SUBMODULES) main,-l$(mod))
#MAIN_DLLS=$(foreach dir,. $(MODULE_DIRS) $(MAIN_SUBMODULE_DIRS),\
		  $(foreach src,$(wildcard $(DIR_ROOT_SRC)$(dir)*.cpp),\
	  $(src:$(DIR_ROOT_SRC)$(dir)%.cpp=$(DIR_ROOT_LIB)$(dir)lib%$(DLL_EXT))))
MAIN_DEPS=$(MAIN_OBJS:%$(OBJ_EXT)=%$(DEP_EXT))

SENTINEL_DIRS?=$(DIR_BIN) $(foreach outer,$(DIR_LIB),\
			   $(foreach inner,. $(MODULE_DIRS),$(outer)$(inner)))

TEST_EXES?=$(foreach mod,math model,\
		   $(DIR_ROOT_BIN)$(mod)$(TEST_EXT)$(EXE_EXT))
#DEBUG_EXE?=$(DIR_ROOT)$(DIR_BIN)$(EXE_BASE)$(DEBUG_EXT)$(EXE_EXT)
RELEASE_EXE?=$(DIR_ROOT_BIN)$(EXE_BASE)$(RELEASE_EXT)$(EXE_EXT)
EXES?=$(TEST_EXES) $(RELEASE_EXE)

GL3W_OBJS?=$(DIR_GL3W_LIB)gl3w$(OBJ_EXT) $(DIR_GL3W_LIB)libgl3w$(DLL_EXT)
TEST_OBJ?=$(DIR_ROOT_LIB)main$(TEST_EXT)$(OBJ_EXT)
#DEBUG_OBJ=$(DIR_ROOT)$(DIR_LIB)main$(DEBUG_EXT).o
RELEASE_OBJ?=$(DIR_ROOT_LIB)main$(RELEASE_EXT)$(OBJ_EXT)
EXE_OBJS?=$(TEST_OBJ) $(RELEASE_OBJ)

###############################################################################
WFLAGS+=-Wall -Wno-unused
CFLAGS=-fPIC -fdata-sections \
	   -isystem $(DIR_GL3W_INCLUDE) -isystem $(DIR_GL3W_INCLUDE)
CPPFLAGS:=$(CFLAGS) -std=c++11 -pthread -fopenmp=libomp\
	-I$(DIR_BOOST_INCLUDE) -I$(DIR_ROOT_INCLUDE) 
LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(DIR_GL3W_LIB)
LDFLAGS:=-L$(DIR_BOOST_LIB) -L$(DIR_GL3W_LIB) -lpthread\
	-Wl,--gc-sections -Wl,-rpath,$(DIR_BOOST_LIB):$(DIR_GL3W_LIB)\
	-lboost_coroutine -lboost_system
RELEASE_LDFLAGS:=$(LDFLAGS) -lGL -lGLU -lglfw -ldl
#RELEASE_LDFLAGS:=$(LDFLAGS) $(MAIN_DLL_DIRS) $(MAIN_DLL_LINKS)\
	-lGL -lGLU -lglfw -ldl

#DEBUG_LDFLAGS=$(LDFLAGS) -lboost_system
TEST_LDFLAGS:=$(LDFLAGS) -lboost_unit_test_framework -lGL -lGLU -lglfw -ldl
###############################################################################
LINK_CC=$(CC) $(CFLAGS) $(WFLAGS)
LINK_CXX=$(CXX) $(CPPFLAGS) $(WFLAGS)
COMPILE_CC=$(LINK_CC) -c
COMPILE_CXX=$(LINK_CXX) -c
DEPEND_CC=$(LINK_CC) -M
DEPEND_CXX=$(LINK_CXX) -M
COMPILE_HPP=$(LINK_CXX) -x c++-header
###############################################################################

default:.sentinel release
all:default test #debug
vpath %.hpp $(DIR_ROOT_INCLUDE)
vpath %.cpp $(DIR_ROOT_SRC)

.sentinel:
	$(MKDIR) $(SENTINEL_DIRS)
	@touch .sentinel

$(DIR_GL3W)%$(OBJ_EXT): $(DIR_GL3W)$(DIR_SRC)*.c
	$(COMPILE_CC) -fPIC $<\
		-o $@
$(DIR_GL3W)lib%$(OBJ_EXT)$(DIR_GL3W)%$(DLL_EXT): $(DIR_GL3W)$(DIR_SRC)*.c
	$(LINK_CC) $(CFLAGS) -shared $<\
		-o $@


#$(DIR_ROOT_LIB)%$(OBJ_EXT) $(DIR_ROOT_LIB)%$(DEP_EXT):\
		$(DIR_ROOT_SRC)%.cpp $(DIR_ROOT_INCLUDE)%.hpp
$(DIR_ROOT_LIB)%$(OBJ_EXT):\
		$(DIR_ROOT_SRC)%.cpp $(DIR_ROOT_INCLUDE)%.hpp
	$(COMPILE_CXX) $<\
		-o $@

$(DIR_ROOT_LIB)%$(DEP_EXT): $(DIR_ROOT_SRC)%.cpp
	$(DEPEND_CXX) $<\
		-o $@
$(DIR_ROOT_LIB)*/%$(DEP_EXT): $(DIR_ROOT_SRC)*/%.cpp
	$(DEPEND_CXX) $<\
		-o $@

lib%$(DLL_EXT): $(MAIN_SRCS)
	$(LINK_CXX) -shared $<\
		-o $@ $(LDFLAGS)

%.hpp$(PCH_EXT): %.hpp
	$(COMPILE_HPP) $<\
		-o $@


clean-deps:; $(RM) $(EXE_OBJS:.o=.d) $(MAIN_DEPS) 
clean-pchs:; $(RM) $(MAIN_PCHS)
clean-objs:; $(RM) $(EXE_OBJS) $(MAIN_OBJS)
clean-dlls:; $(RM) $(MAIN_DLLS)
clean-exes:; $(RM) $(EXES)
clean-sentinels:; $(RM) .sentinel
clean-gl3w:; $(RM) $(GL3W_OBJS)
clean: clean-exes clean-dlls clean-deps clean-objs clean-pchs clean-sentinels;

env:
	@echo "$(foreach var,CC CXX CFLAGS CPPFLAGS WFLAGS\
		RELEASE_LDFLAGS TEST_LDFLAGS MAIN_OBJS,\r$(var) = ${$(var)}\n)"


-include $(MAIN_DEPS)

release: $(RELEASE_EXE) $(RELEASE_OBJ) $(MAIN_OBJS) $(GL3W_OBJS);
#$(RELEASE_EXE): $(RELEASE_OBJ) $(MAIN_OBJS) $(MAIN_DLLS) $(GL3W_OBJS)
$(RELEASE_EXE): $(RELEASE_OBJ) $(MAIN_OBJS) $(GL3W_OBJS)
	$(LINK_CXX) -fPIE\
		$(RELEASE_OBJ) $(MAIN_OBJS) $(GL3W_OBJS)\
		$(RELEASE_LDFLAGS)\
		-o $@

test: $(TEST_EXES) ; # use --log_level=error
$(DIR_ROOT_BIN)math$(TEST_EXT)$(EXE_EXT):\
		$(DIR_TEST)$(DIR_SRC)math.cpp $(MAIN_OBJS)
	$(LINK_CXX) -fPIE\
		$< $(MAIN_OBJS) $(GL3W_OBJS)\
		$(TEST_LDFLAGS)\
		-o $@

$(DIR_ROOT_BIN)model$(TEST_EXT)$(EXE_EXT):\
		$(DIR_TEST)$(DIR_SRC)model.cpp $(MAIN_OBJS)
	$(LINK_CXX) -fPIE\
		$< $(MAIN_OBJS) $(GL3W_OBJS)\
		$(TEST_LDFLAGS)\
		-o $@

.PHONY: all depends env release test debug\
	clean clean-exes clean-dlls clean-deps clean-objs clean-sentinels
