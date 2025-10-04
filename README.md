# clitools


## Building with CMake

Clone the repository and build using CMake:

```sh
cmake -S . -B build
cmake --build build
```

This will build all CLI tools in the `tools/` directory (e.g., `example`, `todo`).

## Running the Example

From the `build/tools/examples/` directory (or wherever your executable is built):

```sh
./example greet Alice
./example math add 2 3
```

You can also run `./example` to see help and available commands.
