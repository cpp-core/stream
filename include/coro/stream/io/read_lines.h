// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/file.h"

namespace coro {

// Return a generator that reads lines from the given `readable`.
Generator<std::string&&> read_lines(Readable auto& readable) {
    std::string line;
    while (getline(readable, line))
	co_yield line;
    co_return;
}

// Return a generator that reads lines from the plain **File** `file`.
Generator<std::string&&> read_lines_plain(core::File file);

// Return a generator that reads lines from the zstd compressed **File** `file`.
Generator<std::string&&> read_lines_zstd(core::File file);

// Return a generator that reads lines from **File** `file` which may be compressed with zstd.
Generator<std::string&&> read_lines(core::File file);

}; // coro
