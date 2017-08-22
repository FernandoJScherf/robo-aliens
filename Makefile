#Files to compile as part of the project:
OBJS = main.c rob.c shapes.c

#Compiler we are using:
CC = gcc

#Additional compilation options we are using:
#-w suppresses all warnings.
COMPILER_FLAGS = -w -std=c99

#Libraries we are linking against:
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx

#Name of our executable:
OBJ_NAME = exec_SDL

#Target that compiles our executable:
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	
