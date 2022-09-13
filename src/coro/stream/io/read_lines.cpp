// Copyright 2021, 2022 by Mark Melton
//

#include <fstream>
#include "coro/stream/io/read_lines.h"

namespace coro {

Generator<std::string&&> read_lines_plain(std::string_view file) {
    std::ifstream ifs{file};
    std::string line;
    while (getline(ifs, line))
	co_yield line;
    co_return;
}

}; // coro
