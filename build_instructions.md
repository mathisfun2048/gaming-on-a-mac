# Here's how you can compile this for yourself!

## Copy the the files in the structure on github
## Download the Pi Pico SDK and set a local variable with the path to that directory
### Here's mine, for instance
export PICO_SDK_PATH=~/Desktop/pico-sdk
## Build with the following terminal commands
cd ~/<path to your project directory>
mkdir build
cd build
cmake ..
make

## in teh build file there should be a .uf2 file. Copy this file onto your pico and you're done!