default:.sentinel release
all:default test #debug
vpath %.hpp $(DIR_ROOT_INCLUDE)
vpath %.cpp $(DIR_ROOT_SRC)

OBJ_PATH:=\"share/sphere.obj\"
MTL_PATH:=\"share/cube.mtl\"
VERT_PATH:=\"share/fallback.vert\"
FRAG_PATH:=\"share/shade.frag\"

-include defs.mk
-include targets.mk
