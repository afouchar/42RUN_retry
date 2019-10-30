NAME            =    42Run
CC                =    g++ -std=c++11
CFLAGS            =    -MMD#-Wall -Wextra -Werror -MMD

GLFW_DIR = /Users/afouchar/.brew/opt/glfw
GLEW_DIR = /Users/afouchar/.brew/opt/glew
GLM_DIR = /Users/afouchar/.brew/opt/glm
DEVIL_DIR = /Users/afouchar/.brew/opt/devil
ASSIMP_DIR = /Users/afouchar/.brew/opt/assimp

INCLUDE            =    -I $(GLEW_DIR)/include/GL \
                    -I $(GLFW_DIR)/include/GLFW \
					-I $(GLM_DIR)/include/glm/ \
					-I $(DEVIL_DIR)/include/ \
					-I $(ASSIMP_DIR)/include/ \
                    -I includes

GLEW            =    $(GLEW_DIR)/lib/libGLEW.dylib
GLFW            =    $(GLFW_DIR)/lib/libglfw.dylib
ASSIMP =  /Users/afouchar/.brew/Cellar/assimp/4.1.0/lib/libassimp.4.1.0.dylib
DEVIL = /Users/afouchar/.brew/Cellar/devil/1.8.0_1/lib/libI*.dylib

FRAMEWORKS        =    -framework OpenGL #-framework Cocoa
CC_NEEDS        =    $(FRAMEWORKS) $(GLFW) $(GLEW) $(ASSIMP) $(DEVIL)

SRC_FILE        =	Camera.cpp \
					Collider.cpp \
					GameBehaviour.cpp \
					Input.cpp \
					Light.cpp \
					Loader.cpp \
					Mesh.cpp \
					Object.cpp \
					PathGenerator.cpp \
					RenderPipeline.cpp \
					Shader.cpp \
					Text.cpp \
					Transform.cpp \
					Window.cpp \
					main.cpp

# HDR_FILE        =    name.h

SRC_DIR            =    sources
OBJ_DIR            =    obj
HDR_DIR            =    includes
SRC                =    $(addprefix $(SRC_DIR)/, $(SRC_FILE))
OBJ                =    $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_FILE))
DPD                =    $(patsubst %.cpp, $(OBJ_DIR)/%.d, $(SRC_FILE))
# HDR                =    $(addprefix $(HDR_DIR)/, $(HDR_FILE))

DEPENDS            =    $(OBJ:.o=.d)

.PHONY: all clean fclean pclean re tests run

release:
	@make -j all

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(CC_NEEDS)
	@# $(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS) $(FRAMEWORKS) $(GLFW) $(GLEW)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re:
	@make fclean
	@make release

run: release
	@echo ""
	./$(NAME)
	@echo ""
	@echo "exitcode: $$?"

-include $(DPD)