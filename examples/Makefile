all: MAKE_LIBRARY kaladarshan.prog line.prog petals.prog raagmalaa.prog rotatingSquares.old.prog

# giving extension .prog to just make it easy to delete all 'easily' with make clean

kaladarshan.prog: kaladarshan.cpp MAKE_LIBRARY
	g++ -g $< -I../include -L../lib -lsprite $(shell pkg-config --cflags --libs glut glu) -lpthread -o $@

line.prog: line.cpp MAKE_LIBRARY
	g++ -g $< -I../include -L../lib -lsprite $(shell pkg-config --cflags --libs glut glu) -lpthread -o $@

petals.prog: petals.cpp MAKE_LIBRARY
	g++ -g $< -I../include -L../lib -lsprite $(shell pkg-config --cflags --libs glut glu) -lpthread -o $@

raagmalaa.prog: raagmalaa.cpp MAKE_LIBRARY
	g++ -g $< -I../include -L../lib -lsprite $(shell pkg-config --cflags --libs glut glu) -lpthread -o $@

rotatingSquares.old.prog: rotatingSquares.old.cpp MAKE_LIBRARY
	g++ -g $< -I../include -L../lib -lsprite $(shell pkg-config --cflags --libs glut glu) -lpthread -o $@

clean:
	rm -rf *.prog

MAKE_LIBRARY:
	make -C ..
