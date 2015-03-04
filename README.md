#**opensim-reflex-controllers**

An OpenSim plugin creating various reflex-like controllers for muscles.  Types of reflexes include:

1. Path Stretch Reflexes (length and velocity)
2. Fiber Stretch Reflexes (length and velocity)
3. Delayed stretch reflexes (constant time offset)

###Dependencies
1. OpenSim 3.2 or above by [https://simtk.org/home/opensim](installing a distribution) or [building from source](https://github.com/opensim-org/opensim-core)
2. a git client
3. Source code (git clone https://github.com/msdemers/opensim-reflex-controllers)
4. [CMake](http://www.cmake.org/)

###Building Steps

####Linux (assuming you have gcc and/or clang)
1. build/install OpenSim. I'll call the install location ~/OpenSim_Install
2. Creat a directories for cloning and building the opensim-reflex-controllers library
```
$ mkdir ~/reflexes
$ cd ~/reflexes
```
3. If you haven't yet, clone this repository, for example to ~/reflexes/source
```
$ git clone https://github.com/msdemers/opensim-reflex-controllers ~/reflexes/source
```
you can optionally check out a specific branch
```
$ git checkout reflexes-3.2
```
4. Create a place to build the reflexes library
```
$ mkdir build
```
5. Run CMake, choosing the IDE project system of your choice, or simply UNIX Make Files will serve fine.
  1. point at ~/reflexes/source/plugin for the source directory. This is where the high-level CMakeLists.txt lives
  2. point at ~/reflexes/build for the build directory
  3. Check the OPENSIM_INSTALL_DIR variable.  If CMake hasn't automatically detected it, point this variable to your OpenSim installation (e.g. ~/OpenSim_Install)
  4. push *Configure* untill nothing appears red
  5. push *Generate*
  
6. Navigate to the build directory
```
$ cd build
```
7. Build the project. This may mean opening your IDE or using your favorite command line build tools.  If using make files.
```
make install
```

After building the install project, plugin libraries and headers for this project will have been build and copied into the opensim plugins and sdk directories. You can either import the reflexesController.so (.dylib for OS X, .dll for Windows) into the gui, or build your own opensim projects as if the reflex controller plugin were native to OpenSim.