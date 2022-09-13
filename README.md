[![Build and Unit Test](https://github.com/melton1968/cxx-coro-stream/actions/workflows/build.yaml/badge.svg)](https://github.com/melton1968/cxx-coro-stream/actions/workflows/build.yaml)

# Streams

Stream sources, sinks and composable operations.

## At a Glance

`cxx-coro-stream` provides a uniform and efficient **Stream Processing**
framework based on co-routines. Any range-based for-loop capable
objects, i.e. any object `X` which has `X::begin()` and `X::end()` or
free functions `begin(X&)` and `end(X&)` can be used as a **Stream**
source including most containers.

**Stream** operations can be composed using the builtin primitives
listed below as well as user-defined primitives which intergrate as
first class citizens.

Suppose you need 100 uniform samples of real-valued points x,y that
are strictly within the unit circle on a 2-d plane. The following
snippet shows how to construct and read from a **Stream** to get the
desired samples:

```C++
auto g = sampler<real>(-1,+1) * sampler<real>(-1,+1)
    | filter([](const auto& pair) { return sqrt(pow(pair.first,2) + pow(pair.second,2)) < 1; })
    | take(100);
for (auto [x, y] : g);
```

* [adapt]()
* [alternate]()
* [apply]()
* [chaining]()
* [choose]()
* [collect]()
* [draw]()
* [filter]()
* [flatten]()
* [group]()
* [group tuple]()
* [iota]()
* [once]()
* [pipeline]()
* [range]()
* [read lines]()
* [reduce]()
* [repeat]()
* [sampler]()
* [sequence]()
* [take]()
* [transform]()
* [unique]()
* [write lines]()
* [zip]()

## Installation

### Using cxx-depends (Recommended)

    git clone git@github.com:melton1968/cxx-depends
	mkdir cxx-depends/build && cd cxx-depends/build
    CC=clang-mp-11 CXX=clang++-mp-11 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt -DCORE_FP_TEST=ON ..
	make cxx_coro_stream_check # Run tests
	make cxx_coro_stream       # Build and install
	
### Standalone

	git clone git@github.com:melton1968/cxx-coro-stream
	mkdir cxx-coro-stream/build && cd cxx-coro-stream/build
    CC=clang-mp-11 CXX=clang++-mp-11 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt -DCORE_FP_TEST=ON ..
	make check    # Run tests
	make install  # Install
	
Compiling and running the tests requires cxx-core-tuple, fmt and googletest to be
installed. This happens automagically when using cxx-depends.
	
	
