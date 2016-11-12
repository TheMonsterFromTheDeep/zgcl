![zLib](logo.png)

zLib is a C utility library built to provide cleaner and simpler structure for C code.
# Features
Right now, zLib features dynamic memory for generic types, a new string type, and exception handling (which will be disable-able
in the future).
# Installation
Right now zLib is a static library. The reason for this is because it is mostly for personal use, and it is undesirable
to have to install a shared library to simply run an executable built using zLib.

There are no pre-built downloads yet, so it has to be built from source.
```
git clone https://github.com/TheMonsterFromTheDeep/zlib.git
cd zlib
make
sudo make install
```
This will install zLib to `/usr/lib` and put its headers in `/usr/include`.
# Usage
The very first thing to do when working with zLib is making sure it links properly. First, it needs to be installed, and then
it is linked with `-lzlib` (at least on GCC).

The next thing to do is note zLib implements the main function so it can implement a default exception handler. It replaces 
main with a call to the `entry` function. This means that the main file for a zLib program must look like this:
```C
#include <zlib/zentry.h>

int entry(zargs args) {
    return 0; /* A return statement is required, as this is not the main() function */
}
```
In the future, it will be possible to disable this behavior.

The entry function must take a single argument of type `zargs`. This object contains both the number of command line arguments
and their data. It has two members: `count` and `get`. `count` is the number of command line arguments, and `get` is an array
containing all the arguments. This means that a program to read out each command line argument would look like this:
```C
#include <stdio.h>
#include <zlib/zentry.h>

int entry(zargs args) {
    int i;
    for(i = 0; i < args.count; ++i) {
        puts(args.get[i]);
    }
    
    return 0;
}
```

Of course, the entry function is simply a side-effect of exception handling. The other features of zLib are much
more useful.

Right now, the most useful feature in zLib is dynamic lists of any type. Dynamic lists are included in `zlist.h`.
The declaration statement for a list is `zlist_of(type)`. Lists are initialized with `zlist_init(name,size)` and 
have access operators `zlist_get(name,index)` and `zlist_set(name,index)`. Their size can be accessed with `zlist_size(name)`.
zlists should be freed with `zlist_free(name)` - trying to use the `free()` function by itself will result in an error due
to how zlists are structured.

Note that many of the list functions are macros - and due to the restrictions on how they can be implemented, they will
behave strangely on any expression that does not return the exact same value every time it is evaluated. It is also a bad idea
to call list 'functions' with any function that is intensive. Generally, it is best to pass nothing but a variable or constant
to a list function.

The following example shows their basic usage:
```C
#include <stdio.h>
#include <zlib/zentry.h>
#include <zlib/zlist.h>

int entry(zargs args) {
    size_t i; /* List index variable */

    zlist_of(int) mylist; /* Declare a list of ints called 'mylist' */
    zlist_init(mylist, 20); /* Initialize 'mylist' with 20 (uninitialized) elements */
    
    /* Iterate over the length of mylist */
    for(i = 0; i < zlist_size(mylist); ++i) {
        /* Set the element of mylist at 'i' to be equal to the (i + 1)th multiple of 5 */
        zlist_set(mylist, i, (i + 1) * 5);
    }
    
    /* Iterate again */
    for(i = 0; i < zlist_size(mylist); ++i) {
        /* Print the element of mylist at i */
        printf("%d\n", zlist_get(mylist, i));
    }
    
    /* Free the memory allocated for mylist */
    zlist_free(mylist);
    
    return 0; /* Return statement */
}
```

zlists also support an 'add' function, which will append the specified element to the end of the list. (In the future, they
will support remove functions and insertion functions as well).

The add statements could be used to change the previous code as such:
```C
#include <stdio.h>
#include <zlib/zentry.h>
#include <zlib/zlist.h>

int entry(zargs args) {
    size_t i; /* List index variable */

    zlist_of(int) mylist; /* Declare a list of ints called 'mylist' */
    zlist_init(mylist, 0); /* Initialize 'mylist' with zero elements */
    
    /* Vary i from 0 to 20 */
    for(i = 0; i < 20; ++i) {
        /* Add the (i + 1)th multiple of 5 to mylist */
        zlist_add(mylist, (i + 1) * 5);
    }
    
    /* Iterate over mylist (which now has size 20) */
    for(i = 0; i < zlist_size(mylist); ++i) {
        /* Print the element of mylist at i */
        printf("%d\n", zlist_get(mylist, i));
    }
    
    /* Free the memory allocated for mylist */
    zlist_free(mylist);
    
    return 0; /* Return statement */
}
```

One of the most important things about lists is that they throw exceptions. For example, the following code:
```C
#include <zlib/zentry.h>
#include <zlib/zlist.h>

int entry(zargs args) {
    zlist_of(int) mylist;
    zlist_init(mylist, 0);
    
    int i = zlist_get(mylist, 1);
    
    zlist_free(mylist);
    
    return 0;
}
```
will exit with the error message `Exiting: Index out of list bounds!` The obvious issue here is that the program never reaches
the `zlist_free()` statement, and as such, will leak memory.

This will be addressed soon...

Exceptions are a very important feature of zlib. The reason for their existence is to make it easier to write programs that can
recover from errors such as segfaults. They are implemented using `setjmp()` and `longjmp()`. Catching an exception is very
simple:
```C
#include <zlib/zentry.h>
#include <zlib/zlist.h>
#include <zlib/zexception.h> /* Include exception header for TRY / CATCH */

int entry(zargs args) {
    zlist_of(int) mylist;
    zlist_init(mylist, 0);
    
    TRY {
        int i = zlist_get(mylist, 1);
    }
    CATCH {
        puts("Caught exception");
    }
    
    zlist_free(mylist);
    
    return 0;
}
```
In fact, the `CATCH` block is not even necessary. The exception message is stored in `except_msg` and can be printed from there.

zLib also supports its own strings. These are under `zstr.h` and introduce a struct type, `zstr`. All zstr functions take a 
zstr pointer.

The zstr struct has a `size` member, an `alloc` member, and a `data` member. `size` is the size of the zstr, `alloc` is the
space allocated for the str, and `data` is a null-terminated character array (the null terminator is not included in `size` or
`alloc`).

The two zstr creation functions are `zstr_empty()` and `zstr_from()`:
```C
zstr *empty = zstr_empty();
zstr *hello = zstr_from("Hello, world!");
```
zstrs also support comparison functions, access functions, and concatenation functions (so far).
```C
zstr *hello = zstr_from("Hello");
zstr *world = zstr_from("world");
zstr_at(hello, 0); /* => 'H' */
zstr_eq(hello, world); /* => FALSE */
zstr_eqc(hello, "Hello"); /* => TRUE */
zstr *blank = zstr_empty();
zstr_cat(blank, hello); /* blank => "Hello" */
zstr_catc(blank, " world"); /* blank => "Hello world" */
```

Finally, zstrs also need to be freed using `zstr_free()`.
