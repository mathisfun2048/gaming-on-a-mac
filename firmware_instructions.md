# Made some changes? Here's how you can recompile

## Pre-Rec: Clone this repo!
If you want to make changes, the easiest way to do so is by cloning this repo and forking!

to clone, in your terminal type the following:

`git clone https://github.com/mathisfun2048/gaming-on-a-mac`

## Step 1: Download Libraries!
This project uses 2 main "libraries": the RPi Pico SDK, and CMake!

The Pico SDK is the chip specific libraries, and CMake helps us cross compile our code to work on the arm chip. 

### RPI Pico SDK
For the RPI Pico, type this into terminal to clone this repo on your computer:

`git clone https://github.com/raspberrypi/pico-sdk`

### ARM
For the arm compilers, go to the directory you want to download it in and type this into your terminal:

`curl -L -O https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-x86_64-arm-none-eabi.tar.xz`

`tar -xf arm-gnu-toolchain-13.2.rel1-darwin-x86_64-arm-none-eabi.tar.xz`


## Step 2: Set the variables!

### i) Get things ready!
First thing we're going to do is clear the build directory. Right now, build has everything that I compiled, and does not reflect your code. 

To do this, open up terminal, and navigate to the build directory. Then type the following command:

`rm -rf *`

Accept, and then everything should be cleared. 

### ii) set variables
In our code, we utalize environment variables so that the cross compiler knows where to find the code from the libraries we installed. 

#### Pico

For pico, set it by typing this into the terminal:

`export PICO_SDK_PATH=<PATH TO YOUR PICO>`

For example, this is what I typed in because I stored it in my desktop (I use it a lot haha):

`export PICO_SDK_PATH=~/Desktop/pico-sdk`

#### ARM

For the cross compiler, type this into the terminal:

`export PATH="/<WHERE YOU HAVE IT STORED>/ARM/bin:$PATH"`

For example, this is what I typed in because I stored it in my app folder:

`export PATH="/Applications/ARM/bin:$PATH"`

## Compiling time!

The hard part is over now! Now its just time to compile!

Open terminal and navigate to the build directory and type the following:

`cmake..`

If this is successful, cmake made the proper files which can now be cross compiled! If this fails, this means that there's likely an error in CMakeLists.txt or your implementation of files that depend on CMakeLists, so check there for debugging. 

Next, type the following to compile:

`make`

If this is successful, you should see a .u2f file! This is what you drag and drop onto your rpi pico to load the software. Happy making!



