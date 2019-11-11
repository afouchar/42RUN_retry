NAME			=	42Run
CC				=	g++ -std=c++11
CFLAGS			=	-MMD#-Wall -Wextra -Werror -MMD

#SCHOOL
BREW_DPD_DIR	=	/Users/afouchar/.brew/opt
#HOME
#BREW_DPD_DIR	=	/usr/local/opt

GLFW_DIR		=	$(BREW_DPD_DIR)/glfw
GLEW_DIR		=	$(BREW_DPD_DIR)/glew
GLM_DIR			=	$(BREW_DPD_DIR)/glm
DEVIL_DIR		=	$(BREW_DPD_DIR)/devil
ASSIMP_DIR		=	$(BREW_DPD_DIR)/assimp

INCLUDE			=	-I $(GLEW_DIR)/include/GL \
					-I $(GLFW_DIR)/include/GLFW \
					-I $(GLM_DIR)/include/glm/ \
					-I $(DEVIL_DIR)/include/ \
					-I $(ASSIMP_DIR)/include/ \
					-I Includes \
					-I Scripts

GLEW			=	$(GLEW_DIR)/lib/libGLEW.dylib
GLFW			=	$(GLFW_DIR)/lib/libglfw.dylib
ASSIMP			=	$(BREW_DPD_DIR)/assimp/lib/libassimp.4.1.0.dylib
DEVIL			=	$(BREW_DPD_DIR)/devil/lib/libI*.dylib

FRAMEWORKS		=	-framework OpenGL #-framework Cocoa
CC_NEEDS		=	$(FRAMEWORKS) $(GLFW) $(GLEW) $(ASSIMP) $(DEVIL)

SRC_FILE		=	Camera.cpp \
					Collider.cpp \
					Bound.cpp \
					GameBehaviour.cpp \
					Input.cpp \
					Light.cpp \
					Loader.cpp \
					Mesh.cpp \
					Object.cpp \
					RenderPipeline.cpp \
					Shader.cpp \
					Text.cpp \
					Transform.cpp \
					Window.cpp \
					main.cpp

SCRIPT_FILE		=	PathGenerator.cpp \
					Chunk.cpp \
					Ship.cpp

SRC_DIR			=	Sources
SCRIPT_DIR		=	Scripts
OBJ_DIR			=	_BUILD_OBJ_
HDR_DIR			=	Includes
SRC				=	$(addprefix $(SRC_DIR)/, $(SRC_FILE))
SCRIPT			=	$(addprefix $(SCRIPT_DIR)/, $(SCRIPT_FILE))
OBJ				=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_FILE))
SCRIPT_OBJ		=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SCRIPT_FILE))
DPD				=	$(patsubst %.cpp, $(OBJ_DIR)/%.d, $(SRC_FILE))
SCRIPT_DPD		=	$(patsubst %.cpp, $(OBJ_DIR)/%.d, $(SCRIPT_FILE))


DEPENDS			=	$(OBJ:.o=.d)
SCRIPT_DEPENDS	=	$(SCRIPT_OBJ:.o=.d)

.PHONY: all clean fclean pclean re tests run

release:
	@make -j all

all: $(NAME)

$(NAME): $(OBJ) $(SCRIPT_OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(SCRIPT_OBJ) -o $(NAME) $(CC_NEEDS)
	@# $(CC) $(CFLAGS) $(OBJ) $(SCRIPT_OBJ) -o $(NAME) $(LIBS) $(FRAMEWORKS) $(GLFW) $(GLEW)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(SCRIPT_DIR)/%.cpp Makefile | $(OBJ_DIR)
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

-include $(DPD) $(SCRIPT_DPD)