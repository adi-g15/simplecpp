line: line.cpp FORCE
	make -C ..
	g++ -g line.cpp -I../include -L../lib -lsprite $(shell pkg-config --cflags --libs x11 glut glu) -o line

FORCE:
