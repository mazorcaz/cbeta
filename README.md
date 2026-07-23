# CBeta: Beta 1.7.3 client in C

A client for minecraft beta 1.7.3

## Building

Since its not done, you must build it yourself

It requires libsdl2-dev and libsdl2-image-dev and opengl 1.1.

On linux do `make`  

On windows you can build either a dynamic or a static version with `build <dynamic|static>`  
dynamic - smaller executable but requires a few dlls  
static - a bigger self contained executable that doesn't require any additional dlls

Then you can do `./bin/cbeta` on linux or `bin\cbeta` on windows
