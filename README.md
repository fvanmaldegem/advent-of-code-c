# Adevent of Code in C
Author: Floris van Maldegem

Advent of Code performed in C.

### Requirements
- `make` (`build-essential`)
- `clang`
- `[libcurl](https://ec.haxx.se/install/linux.html)`

### How to build
```bash
make <year>/<day>
```

### How to run
After you've build the solution. You can directly run the binary: 
```bash
build/<year>_<day>
```

This will run it with the example input data in `/test/<year>/<day>/input.txt`.
If you want to run it with YOUR input data, you can set the `SESSION_COOKIE` environment variable like this:
```bash
SESSION_COOKIE="abcde"  build/<year>_<day>
```

### How to set up clangd
We can easily generate a `compile_commands.json` file with [`bear`](https://github.com/rizsotto/Bear):
```bash
bear -- make all
```