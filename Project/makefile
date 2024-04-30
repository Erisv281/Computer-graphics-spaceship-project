# set this variable to the director in which you saved the common files
commondir = ../common/

all : project 

project : project.cpp GameData/simplefont.c GameData/Controls.cpp GameData/frustum_culling.cpp GameData/Entity.cpp GameData/Spaceship.cpp GameData/Bullet.cpp GameData/Enemy.cpp  $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o project -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES project.cpp GameData/simplefont.c GameData/Controls.cpp GameData/frustum_culling.cpp GameData/Entity.cpp GameData/Spaceship.cpp GameData/Bullet.cpp GameData/Enemy.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -lstdc++


clean :
	rm project 
