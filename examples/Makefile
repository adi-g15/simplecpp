opengl: opengl.cpp
	make --directory=..
	g++ -g $< -I../include -L../lib -lsprite -ldl -lX11 -lglfw -pthread

old_opengl: opengl.cpp
	g++ $< ../src/glad.c -lglfw -ldl -I../include -o $@
