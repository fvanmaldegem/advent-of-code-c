# Adevent of Code in C
Author: Floris van Maldegem

Advent of Code performed in C.

### Requirements
- clang
- [libcurl](https://ec.haxx.se/install/linux.html)

### How to build
```bash
make <year>/day<day> && build/<year>_day<day>
```

### How to set up clangd
We can easily generate a `compile_commands.json` file with [`bear`](https://github.com/rizsotto/Bear):
```bash
bear -- make all
```