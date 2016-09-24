default:.sentinel release
all:default test #debug
vpath %.hpp $(DIR_ROOT_INCLUDE)
vpath %.cpp $(DIR_ROOT_SRC)

OBJ_PATH:=\"share/icosahedron.obj\"
VERT_PATH:=\"share/checker.vert\"
FRAG_PATH:=\"share/shade.frag\"

-include defs.mk
-include targets.mk
