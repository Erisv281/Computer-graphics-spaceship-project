# set this variable to the director in which you saved the common files
commondir = ../common/

all : project 

project : project.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o project -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES project.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -lstdc++


clean :
	rm project 
