INCLUDES:= -Iinclude `pkg-config --cflags x11`
LIBS:= `pkg-config --libs x11`

# include -g here, if debug build wanted
CXX_OPTS:= -Wall -std=c++11 -pedantic -Werror

OBJS =  src/canvas.o \
		src/polygon.o \
		src/line.o \
		src/circle.o \
		src/sprite.o \
		src/turtle.o \
		src/rectangle.o \
		src/text.o \
		src/turtleSim.o \
		src/sim.o

HDRS =  include/spriteInterface.h \
		include/pose.h \
		include/composite.h \
		include/common_def.h  \
		include/sprite.h \
		include/canvas.h \
		include/polygon.h \
		include/turtle.h \
		include/line.h \
		include/keycodes.h \
		include/circle.h \
		include/rectangle.h \
		include/text.h \
		include/turtleSim.h \
		include/sim.h \
		include/simplecpp

default: $(OBJS) $(HDRS)
	mkdir -p lib
	ar rcs lib/libsprite.a $(OBJS)

s++: default
	printf "#!/bin/sh\n" > s++
	printf "g++ \$$@ -Wall -I/usr/include/simplecpp/include -L/usr/lib/simplecpp -lsprite `pkg-config --cflags --libs x11` -std=c++11\n" >> s++
	chmod a+x s++

src/canvas.o: src/canvas.cpp include/canvas.h include/sprite.h include/common_def.h include/turtle.h
	g++ -c src/canvas.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

src/polygon.o: src/polygon.cpp include/polygon.h include/sprite.h include/common_def.h
	g++ -c src/polygon.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

src/line.o: src/line.cpp include/line.h include/sprite.h include/common_def.h
	g++ -c src/line.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

src/rectangle.o: src/rectangle.cpp include/rectangle.h include/sprite.h include/common_def.h
	g++ -c src/rectangle.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

src/circle.o: src/circle.cpp include/circle.h include/sprite.h include/common_def.h
	g++ -c src/circle.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

src/text.o: src/text.cpp include/text.h include/sprite.h include/canvas.h include/common_def.h
	g++ -c src/text.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

src/sprite.o: src/sprite.cpp include/sprite.h include/common_def.h include/pose.h
	g++ -c src/sprite.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

src/turtle.o: src/turtle.cpp include/turtle.h include/common_def.h
	g++ -c src/turtle.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

src/turtleSim.o: src/turtleSim.cpp include/turtleSim.h
	g++ -c src/turtleSim.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

src/sim.o: src/sim.cpp include/sim.h
	g++ -c src/sim.cpp -o $@ ${INCLUDES} ${LIBS} ${CXX_OPTS}

install: default s++
	mkdir -p /usr/include/simplecpp/include \
			   /usr/lib/simplecpp/
	install -Dm 644 ${HDRS} /usr/include/simplecpp/include/
	install -Dm 644 lib/libsprite.a /usr/lib/simplecpp/libsprite.a
	install -Dm 755 s++ /usr/bin/s++

uninstall:
	rm -rf /usr/include/simplecpp
	rm -f /usr/lib/simplecpp/libsprite.a
	rm -f /usr/bin/s++

clean:
	rm src/*.o lib/*.a
