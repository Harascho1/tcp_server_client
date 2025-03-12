# HOW TO

## BUILD

First go to the root directory of this project and open terminal. Then type this:

```sh
mkdir build
cd build
cmake ..
make
```

In build directory You would have installed client.exe and server.exe if you are on windows or client.out and server.out if You arre on linux.

## RUN

Than run the server executable with:

```sh
./server.exe
```

or if you are on linux

```sh
./server.out
```

Leave that running and open a new terminal in same directory (build) and execute client with:

```sh
./client.exe
```

or if you are on linux

```sh
./client.out
```

You will receive print message on client and both programs should be terminated.