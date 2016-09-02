default:.sentinel release
all:default $(MAIN_OBJS) test #debug
vpath %.hpp $(DIR_ROOT_INCLUDE)
vpath %.cpp $(DIR_ROOT_SRC)

.sentinel:
	$(MKDIR) $(SENTINEL_DIRS)
	@touch .sentinel


#$(DIR_GL3W)lib%$(OBJ_EXT)$(DIR_GL3W)%$(DLL_EXT): $(DIR_GL3W)$(DIR_SRC)*.c\
		$(DIR_GL3W)$(DIR_INCLUDE)GL/*.h
#	@echo GL3W_LDFLAGS={$(GL3W_LDFLAGS)}
#	$(LINK_CC) $(CFLAGS) $(GL3W_LDFLAGS) -shared $<\
		-o $@

$(DIR_ROOT_LIB)%$(OBJ_EXT):\
		$(DIR_ROOT_SRC)%.cpp $(DIR_ROOT_INCLUDE)%.hpp
	$(COMPILE_CXX) $<\
		-o $@

$(DIR_ROOT_LIB)%$(DEP_EXT):\
		$(DIR_ROOT_SRC)%.cpp $(DIR_ROOT_INCLUDE)%.hpp
	$(DEPEND_CXX) $<\
		-o $@
$(DIR_ROOT_LIB)*/%$(DEP_EXT):\
		$(DIR_ROOT_SRC)*/%.cpp $(DIR_ROOT_INCLUDE)*/%.hpp
	$(DEPEND_CXX) $<\
		-o $@
$(DIR_ROOT_LIB)main$(OBJ_EXT):\
		$(DIR_ROOT_SRC)main.cpp $(DIR_ROOT_INCLUDE)main.hpp
	$(COMPILE_CXX) $<\
		$(RELEASE_CPPFLAGS)\
		-o $@

lib%$(DLL_EXT): $(MAIN_SRCS)
	$(LINK_CXX) -shared $<\
		-o $@ $(LDFLAGS)

%.hpp$(PCH_EXT): %.hpp
	$(COMPILE_HPP) $(CPPFLAGS) $(RELEASE_CPPFLAGS)  $<\
		-o $@

clean-deps:; $(RM) $(EXE_OBJS:.o=.d) $(MAIN_DEPS) 
clean-pchs:; $(RM) $(MAIN_PCHS)
clean-objs:; $(RM) $(EXE_OBJS) $(MAIN_OBJS)
clean-dlls:; $(RM) $(MAIN_DLLS)
clean-exes:; $(RM) $(EXES)
clean-sentinels:; $(RM) .sentinel
clean-gl3w:; $(RM) $(GL3W_OBJS)
clean: clean-exes clean-dlls clean-deps clean-objs clean-pchs clean-sentinels;

env:; @echo "$(foreach var,CC CXX CFLAGS CPPFLAGS WFLAGS\
		COMPILE_CC LINK_CC COMPILE_CXX COMPILE_HPP LINK_CXX\
		RELEASE_LDFLAGS TEST_LDFLAGS MAIN_OBJS MAIN_DLLS MAIN_DLL_LINKS\
		MAIN_INCLUDES MODULES MAIN_SUBMODULES RELEASE_CPPFLAGS \
		GL3W_OBJS GL3W_DLLS,\
		\r$(var) = ${$(var)}\n\n)"

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),.sentinel)
ifneq ($(MAKECMDGOALS),env)
ifneq ($(MAKECMDGOALS),gl3w)
	-include $(MAIN_DEPS)
endif
endif
endif
endif

release: gl3w $(MAIN_OBJS) $(MAIN_DEPS)\
		$(RELEASE_OBJ) $(RELEASE_EXE);
$(RELEASE_EXE): $(RELEASE_OBJ) $(MAIN_OBJS) $(GL3W_OBJS)\
		$(MAIN_PCHS)
	$(LINK_CXX) -fPIE $(RELEASE_CPPFLAGS)\
		$(RELEASE_OBJ) $(MAIN_OBJS) $(GL3W_OBJS) $(RELEASE_LDFLAGS)\
		-o $@

$(DIR_ROOT_INCLUDE)main$(PCH_EXT): $(DIR_ROOT_INCLUDE)main.hpp $(MAIN_INCLUDES)
	$(COMPILE_HPP) $(RELEASE_CPPFLAGS) $<\
		-o $@

$(DIR_ROOT_LIB)util$(OBJ_EXT): $(UTIL_H_ONLY)

test: $(MAIN_OBJS) $(TEST_EXES); 

gl3w: $(GL3W_OBJS)
$(DIR_GL3W)%$(OBJ_EXT): $(DIR_GL3W)$(DIR_SRC)*.c\
		$(DIR_GL3W)$(DIR_INCLUDE)GL/*.h
	$(COMPILE_CC) $(CFLAGS) $<\
		-o $@
$(GL3W_DLLS): $(DIR_GL3W)$(DIR_SRC)*.c $(DIR_GL3W)$(DIR_INCLUDE)GL/*.h
	$(LINK_CC) $(CFLAGS) $(GL3W_LDFLAGS) -shared $<\
		-o $@

# TODO Thread-safe use of MAIN_OBJS
$(DIR_TEST)$(DIR_BIN)%$(TEST_EXT)$(EXE_EXT):\
		$(DIR_TEST)$(DIR_SRC)%.cpp $(DIR_ROOT_LIB)%$(OBJ_EXT) $(MAIN_OBJS)
	$(LINK_CXX) $(TEST_CPPFLAGS) -fPIE\
		$< $(MAIN_OBJS) $(GL3W_OBJS)\
		$(TEST_LDFLAGS)\
		-o $@

$(DIR_TEST)$(DIR_BIN)model$(TEST_EXT)$(EXE_EXT):\
		$(DIR_TEST)$(DIR_SRC)model.cpp $(DIR_ROOT_LIB)model$(OBJ_EXT)\
		$(MAIN_OBJS)
	$(LINK_CXX) $(TEST_CPPFLAGS) -fPIE\
		$< $(MAIN_OBJS) $(GL3W_OBJS)\
		$(TEST_LDFLAGS)\
		$(MODEL_CPPFLAGS)\
		-o $@

.PHONY: all depends env release test debug gl3w\
	clean clean-exes clean-dlls clean-deps clean-pchs \
	clean-objs clean-sentinels
