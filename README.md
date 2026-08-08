# CBeta: Beta 1.7.3 client in C

A utility client for minecraft beta 1.7.3 that is written entirely from scratch in C using opengl 1.1.

This project is not even close to finished yet.

## Building

Since it is currently version 0.001, you must build it yourself

It requires libsdl2-dev and libsdl2-image-dev and opengl 1.1 (which is on every computer since 400 B.C).

### Linux dependencies

```
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
sudo apt install zlib1g-dev
```

### Windows dependencies

```
just install SDL2 and SDL2 Image and
ZLib developement libraries using MinGW or 
something and just mess around with header files 
until it just works.
```

### Building

do

`make`

then you can do

`./bin/cbeta`

(or `bin\cbeta` on windows)

> note: windows is currently unsupported
