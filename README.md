# DBPlayground

A platform to show/verify how to integrate other interesting things with a database.

Based on CMU 15445's course project.

## Target list

## Build
```bash
git clone git@github.com:roanhe-ts/miniKV.git

cd miniKV

git submodule update --init --recursive

mkdir build

cd build 

cmake ..
```
Make test
```bash
make Core_test
```

## Format
Use clang-format to auto format
```bash
cd build

make format
```
