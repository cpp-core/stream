[![Build and Unit Test](https://github.com/melton1968/cxx-core-fp/actions/workflows/build.yaml/badge.svg)](https://github.com/melton1968/cxx-core-fp/actions/workflows/build.yaml)

# Functional Programming

Primitives to support a functional programming style.

## At A Glance

* [Either]() -- 
* [Option]() -- 
* [Try]() -- 
* [match]() -- 

## Installation

### Using cxx-depends (Recommended)

    git clone git@github.com:melton1968/cxx-depends
	mkdir cxx-depends/build && cd cxx-depends/build
    CC=clang-mp-11 CXX=clang++-mp-11 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt -DCORE_FP_TEST=ON ..
	make cxx_core_fp_check # Run tests
	make cxx_core_fp       # Build and install
	
### Standalone

	git clone git@github.com:melton1968/cxx-core-fp
	mkdir cxx-core-fp/build && cd cxx-core-fp/build
    CC=clang-mp-11 CXX=clang++-mp-11 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt -DCORE_FP_TEST=ON ..
	make check    # Run tests
	make install  # Install
	
Compiling and running the tests requires googletest to be
installed. This happens automagically when using cxx-depends.
	
	
