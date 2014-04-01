#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -Wall -fexceptions -fPIC
RESINC = 
LIBDIR = 
LIB = -lGL -lGLU -lX11
LDFLAGS = 

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = ./libOpenGLAcquisition.so

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2 -fPIC
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = ./libOpenGLAcquisition.so

OBJ_DEBUG = $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/model_loader_obj.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/glx.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/main.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/model_loader.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/save_to_file.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/scene.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/shader_loader.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/tools.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.o $(OBJDIR_DEBUG)/OpenGLAcquisition.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader/bmp.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader/ppm.o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/model_loader_obj.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/glx.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/main.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/model_loader.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/save_to_file.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/scene.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/shader_loader.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/tools.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.o $(OBJDIR_RELEASE)/OpenGLAcquisition.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader/bmp.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader/ppm.o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d . || mkdir -p .
	test -d $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src || mkdir -p $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src
	test -d $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TrajectoryParser || mkdir -p $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TrajectoryParser
	test -d $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix || mkdir -p $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	test -d $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader || mkdir -p $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) -shared $(LIBDIR_DEBUG) $(OBJ_DEBUG)  -o $(OUT_DEBUG) $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/model_loader_obj.o: opengl_depth_and_color_renderer/src/model_loader_obj.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/model_loader_obj.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/model_loader_obj.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.o: opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.cpp -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.o: opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.cpp -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/glx.o: opengl_depth_and_color_renderer/src/glx.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/glx.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/glx.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/main.o: opengl_depth_and_color_renderer/src/main.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/main.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/main.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/model_loader.o: opengl_depth_and_color_renderer/src/model_loader.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/model_loader.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/model_loader.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/save_to_file.o: opengl_depth_and_color_renderer/src/save_to_file.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/save_to_file.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/save_to_file.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/scene.o: opengl_depth_and_color_renderer/src/scene.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/scene.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/scene.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/shader_loader.o: opengl_depth_and_color_renderer/src/shader_loader.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/shader_loader.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/shader_loader.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/tools.o: opengl_depth_and_color_renderer/src/tools.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/tools.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/tools.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.o: opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.o: opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.o: opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.o: opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.o: opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.o

$(OBJDIR_DEBUG)/OpenGLAcquisition.o: OpenGLAcquisition.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c OpenGLAcquisition.cpp -o $(OBJDIR_DEBUG)/OpenGLAcquisition.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.o: opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader/bmp.o: opengl_depth_and_color_renderer/src/TextureLoader/bmp.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/TextureLoader/bmp.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader/bmp.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader/ppm.o: opengl_depth_and_color_renderer/src/TextureLoader/ppm.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/TextureLoader/ppm.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader/ppm.o

$(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.o: opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.c -o $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf .
	rm -rf $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src
	rm -rf $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TrajectoryParser
	rm -rf $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/AmMatrix
	rm -rf $(OBJDIR_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/opengl_depth_and_color_renderer/src/TextureLoader

before_release: 
	test -d . || mkdir -p .
	test -d $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src || mkdir -p $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src
	test -d $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TrajectoryParser || mkdir -p $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TrajectoryParser
	test -d $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix || mkdir -p $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	test -d $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader || mkdir -p $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) -shared $(LIBDIR_RELEASE) $(OBJ_RELEASE)  -o $(OUT_RELEASE) $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/model_loader_obj.o: opengl_depth_and_color_renderer/src/model_loader_obj.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/model_loader_obj.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/model_loader_obj.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.o: opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.cpp -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TrajectoryParser/InputParser_C.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.o: opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.cpp -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TrajectoryParser/TrajectoryParser.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/glx.o: opengl_depth_and_color_renderer/src/glx.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/glx.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/glx.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/main.o: opengl_depth_and_color_renderer/src/main.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/main.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/main.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/model_loader.o: opengl_depth_and_color_renderer/src/model_loader.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/model_loader.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/model_loader.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/save_to_file.o: opengl_depth_and_color_renderer/src/save_to_file.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/save_to_file.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/save_to_file.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/scene.o: opengl_depth_and_color_renderer/src/scene.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/scene.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/scene.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/shader_loader.o: opengl_depth_and_color_renderer/src/shader_loader.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/shader_loader.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/shader_loader.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/tools.o: opengl_depth_and_color_renderer/src/tools.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/tools.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/tools.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.o: opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/solveHomography.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.o: opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/matrix3x3Tools.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.o: opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/matrix4x4Tools.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.o: opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/matrixCalculations.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.o: opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/ocvStaging.o

$(OBJDIR_RELEASE)/OpenGLAcquisition.o: OpenGLAcquisition.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c OpenGLAcquisition.cpp -o $(OBJDIR_RELEASE)/OpenGLAcquisition.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.o: opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix/solveLinearSystemGJ.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader/bmp.o: opengl_depth_and_color_renderer/src/TextureLoader/bmp.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/TextureLoader/bmp.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader/bmp.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader/ppm.o: opengl_depth_and_color_renderer/src/TextureLoader/ppm.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/TextureLoader/ppm.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader/ppm.o

$(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.o: opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.c -o $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader/texture_loader.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf .
	rm -rf $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src
	rm -rf $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TrajectoryParser
	rm -rf $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/AmMatrix
	rm -rf $(OBJDIR_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/opengl_depth_and_color_renderer/src/TextureLoader

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

