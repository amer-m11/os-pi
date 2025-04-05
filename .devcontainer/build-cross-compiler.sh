#!/bin/bash
# This script is based on the instructions from https://wiki.osdev.org/GCC_Cross-Compiler
set -e  # Exit on any error


BINUTILS_VERSION="2.38"
GCC_VERSION="12.1.0"
GDB_VERSION="12.1"
export TARGET="aarch64-elf"  # Target for Raspberry Pi with ELF format
export CC_HOME="/opt/cross"
mkdir -p $CC_HOME
export PATH="$CC_HOME/bin:$PATH"


# Create a directory to store the source code needed to build the cross-compiler
mkdir -p $HOME/cc-build-src
cd $HOME/cc-build-src


# Download and extract gcc, gdb and binutils
if [ ! -f "gcc-${GCC_VERSION}.tar.xz" ]; then
    echo "Downloading GCC ${GCC_VERSION}..."
    wget https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz
fi

if [ ! -f "binutils-${BINUTILS_VERSION}.tar.xz" ]; then
    echo "Downloading Binutils ${BINUTILS_VERSION}..."
    wget https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz
fi

if [ ! -d "gcc-${GCC_VERSION}" ]; then
    echo "Extracting GCC..."
    tar xf gcc-${GCC_VERSION}.tar.xz
fi

if [ ! -d "binutils-${BINUTILS_VERSION}" ]; then
    echo "Extracting Binutils..."
    tar xf binutils-${BINUTILS_VERSION}.tar.xz
fi

cd $HOME/cc-build-src
if [ ! -f "gdb-${GDB_VERSION}.tar.xz" ]; then
    echo "Downloading GDB ${GDB_VERSION}..."
    wget https://ftp.gnu.org/gnu/gdb/gdb-${GDB_VERSION}.tar.xz
    tar xf gdb-${GDB_VERSION}.tar.xz
fi


# Build Binutils
cd $HOME/cc-build-src
mkdir build-binutils
cd build-binutils
../binutils-${BINUTILS_VERSION}/configure --target=$TARGET --prefix="$CC_HOME" --with-sysroot --disable-nls --disable-werror
make
make install


# Verify binutils installation
cd $HOME/cc-build-src
which -- $TARGET-as || echo "$TARGET-as is not in the PATH"


# build GDB
cd $HOME/cc-build-src
mkdir -p build-gdb
cd build-gdb
../gdb-${GDB_VERSION}/configure --target=$TARGET --prefix="$CC_HOME" --disable-werror
make all-gdb
make install-gdb


# Build GCC
cd $HOME/cc-build-src
mkdir build-gcc
cd build-gcc
../gcc-${GCC_VERSION}/configure --target=$TARGET --prefix="$CC_HOME" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
make all-gcc
make all-target-libgcc
make all-target-libstdc++-v3
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3
