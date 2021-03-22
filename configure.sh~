#!/bin/sh

unamestr=$(uname)
if [ "$unamestr" = 'Linux' ]; then
	MAKEOPT="-I"`pwd`"/include -I/usr/include/X11 -L/usr/X11R6/lib -lX11 -std=c++0x"
elif [ "$unamestr" = 'Darwin' ]; then
        MAKEOPT="-I"`pwd`"/include -I/opt/X11/include -L/opt/X11/lib -lX11 -std=c++0x"
fi

rm -f makes++
echo "#!/bin/sh -x" >> makes++
echo "g++ \$@ -Wall "$MAKEOPT >> makes++
chmod +x makes++

cd src
make config
cd ..

rm -f s++

SPRITE=`pwd`"/lib/libsprite.a "$MAKEOPT

echo "#!/bin/sh -x" >> s++
echo "g++ \$@ -Wall "$SPRITE >> s++
chmod +x s++


echo "To compile use: "`pwd`"/s++ filename.cpp"

