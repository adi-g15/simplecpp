## simplecpp

Instructions for installing the simplecpp package:
1. Copy this directory (simplecpp) somewhere.
2. Change the directory to this, and execute
   ./configure.sh

This should create simplecpp/lib/libsprite.a which is needed in
simplecpp.  The include files will be in simplecpp/include.

It should also create simplecpp/s++

s++ is the compiler for use with simplecpp.  You can create an alias
so that you dont need to give the entire filename to use s++.

To use simplecpp, your source files should contain
```cpp
#include <simplecpp>
```

------
   
Simplecpp directory contains following subdirectories:

       include : contains C++ include files

       lib     : contains libsprite.a  

       src     : source files.  

-----

### Building

```sh
make s++
```

### Installing s++

```
make install
```

### Future

*[] To reduce dynamic allocations
