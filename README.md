# DBPlayground

`DBPlaygound` aims to offer a platform to show/verify how to integrate other intersting things with a database.

We offer a prototype database based on CMU 15445's course project.

## Build
```bash
git clone git@github.com:roanhe-ts/miniKV.git

cd miniKV

git submodule update --init --recursive

mkdir build

cd build 

cmake ..
```
DO NOT SUPPORT COMMON USE for now, but, you can make test
```bash
make Core_test
```

## Format
Use clang-format to auto format
```bash
cd build

make format
```
