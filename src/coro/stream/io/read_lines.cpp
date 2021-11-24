// Copyright 2021 by Mark Melton
//

#include <sstream>
#include "coro/stream/io/read_lines.h"
#include "core/codex/zstd/file_decompressor.h"

namespace coro {

Generator<std::string> read_lines_plain(core::File&& file) {
    std::ifstream ifs{file};
    std::string line;
    while (getline(ifs, line))
	co_yield line;
    co_return;
}

Generator<std::string> read_lines_zstd(core::File&& file) {
    zstd::FileDecompressor d{file};
    std::string line;
    while (d.read_line(line))
	co_yield line;
    co_return;
}

Generator<string> read_lines(core::File&& file) {
    if (file.ends_with(".zst"))
	return read_lines_zstd(std::forward<core::File>(file));
    return read_lines_plain(std::forward<core::File>(file));
}

}; // coro
