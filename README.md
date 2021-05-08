## simplecpp

Just `make install` to build and install the library, headers, and the `s++` script, which calls the C++ compiler with appropriate options.

### s++

s++ is the compiler for use with simplecpp, it simply is a script that takes care of the includes and linking libraries for you.
It will be automatically installed with `make install`, or you can only generate this s++ script, with `make s++`.

To use simplecpp, your source files should contain
```cpp
// main.cpp

#include <simplecpp>

int main() { turtleSim(); getClick(); }
```

And compile with
```sh
s++ main.cpp
```

> NOTE: If you like you can pass additional arguments to s++, like `s++ main.cpp -o main -lGL`

### Directory Structure

Simplecpp directory contains following subdirectories:

       include : contains C++ include files

       lib     : contains libsprite.a  

       src     : source files.


### Building

```sh
make s++
```

### Installing s++

> Requires root permissions, use `sudo make install` if running as non-root
```
make install
```

### Future

-[] To reduce dynamic allocations

