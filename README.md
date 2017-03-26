![zgcl](logo.png)

# Features
zgcl stands for "Z Generic C Library." It is a collection of macros and functions that provide more generic capabilities to the C language - at this point in time, all it provides is a generic list implementation (similar to a C++ vector) as well as a string implementation and some (mostly unused) exception handling.
# Installation
Right now zgcl is a static library. The reason for this is because it is mostly for personal use, and it is undesirable
to have to install a shared library to simply run an executable built using zgcl.

There are no pre-built downloads yet, so it has to be built from source.
```
git clone https://github.com/TheMonsterFromTheDeep/zgcl.git
cd zgcl
make
sudo make install
```
This will install zgcl to `/usr/lib` and put its headers in `/usr/include`.
# Usage
The very first thing to do when working with zgcl is making sure it links properly. First, it needs to be installed, and then
it is linked with `-lzgcl` (at least on GCC).

## entry
One of the most basic features of zgcl is the `entry` function. It is a function that replaces main as the entry point for a program and provides a single `zargs` object as the command line arguments. This means that the main file for a zgcl program will look like this:
```C
#include <zgcl/zentry.h>

void entry(zargs args) {
    //...
}
```
The `zargs` object has two members: `count` and `get`. `count` is simply `argc`, and `get` is `argv`. This means that a program to simply print command line arguments could look like this:
```C
#include <stdio.h>
#include <zgcl/zentry.h>

void entry(zargs args) {
    int i;
    for(i = 0; i < args.count; ++i) {
        puts(args.get[i]);
    }
}
```
As of now, it is not really possible to avoid zgcl from overriding the main method - eventually that will be doable but for now it is recommended to not use zgcl if you either work with another library doing the same thing or simply do not like that.

## containers
As of now, zgcl supports two different containers - `zlist`s (which are generic) and `zstr`s (which are simply strings). These containers share a number of common methods, which each take a pointer to a container:

`zsize`: gets the number of elements in the container object.
`zalloc`: number of allocated elements in the container object (usually more than zsize).
`zsizeptr`, `zallocptr`: gets pointers to either `zsize` or `zalloc` - these methods usually not very useful and mostly used internally.
`zfree`: must be used to free a container - using regular `free` will result in a segfault.

## lists
`zlist`s are one of the most powerful features of zgcl.
