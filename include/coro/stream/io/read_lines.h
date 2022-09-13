// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include <string>
#include "coro/stream/util.h"

namespace coro {

/// Return a generator that reads lines from the given `readable`.
Generator<std::string&&> read_lines(Readable auto& readable) {
    std::string line;
    while (getline(readable, line))
	co_yield line;
    co_return;
}

/// Return a generator that reads lines from the plain **File** `file`.
Generator<std::string&&> read_lines_plain(std::string_view file);

}; // coro
