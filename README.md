# HOW TO

## BUILD

First go to the root directory of this project and open terminal. Then type this:

```sh
mkdir build
cd build
cmake ..
make
```

  optimal:

```sh
cmake .. -G "MinGw Makefiles"
mingw32-make
```

In build directory You would have installed client.exe and server.exe if you are on windows or client.out and server.out if You arre on linux.

## RUN

### Visual Studio

run the solution and build server and client. Than you can open one window of same solution and run server first than client or you can go to the Debug/Release dir and run throw command line.

### MinGW

Than run the server executable with:

```sh
.\server.exe
```

```sh
.\client.exe
```

### Linux

or if you are on linux

```sh
./server.out
```

```sh
./client.out
```

You will receive print message on client and both programs should be terminated.
