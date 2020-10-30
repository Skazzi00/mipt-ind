# MIPT Industrial Programming Course 

Ilya Dedinsky's Industrial Programming Course

## Contents

 - [Quadratic equation](https://github.com/Skazzi00/mipt-ind/blob/master/src/square.c)
 - [Onegin](https://github.com/Skazzi00/mipt-ind/blob/master/src/sort.c)
 - [Vector](https://github.com/Skazzi00/mipt-ind/blob/master/include/vector.hpp)
 - [CheckedStack](https://github.com/Skazzi00/mipt-ind/blob/master/include/stack.hpp)
 
## Prerequisites 

 - `cmake`
 - `doxygen` if you need compiled documentation
 
## Build
 
```shell script
$ git submodule update --init
$ mkdir -p build
$ cd build
$ cmake ..
$ make -j
```

## Usage

 - `format_file` - formats the file (if not given then stdin),
  namely, deletes empty lines and lines without Russian letters
 
```shell script
$ ./format_file [file]
```

 - `square` or `squareC` - solve Quadratic Equation
 
```shell script
$ ./square
$ ./squareC
```

 - `sort` or `sortC` - sorts the lines in the file (or stdin in the cpp version) in lexicographic order
  and in reverse order. Outputs the result to stdout.

```shell script
$ ./sort [file]
$ ./sortC [-ho] <input>
```
