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


# make sure you isntall arm!

here's how I did it on mac!
cd ~/Downloads
curl -L -O https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-x86_64-arm-none-eabi.tar.xz

tar -xf arm-gnu-toolchain-13.2.rel1-darwin-x86_64-arm-none-eabi.tar.xz

sudo mv arm-gnu-toolchain-13.2.Rel1-darwin-x86_64-arm-none-eabi /Applications/ARM

export PATH="/Applications/ARM/bin:$PATH"

## verify
arm-none-eabi-gcc --version

## set path to that!
export PATH="/Applications/ARM/bin:$PATH"