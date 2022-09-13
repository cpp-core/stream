// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include <fstream>
#include "coro/stream/util.h"

namespace coro {

/// Write lines from the supplied **Stream** `source` to the given
/// `writeable`.
void write_lines(Stream auto source, Writeable auto& writeable) {
    for (const auto& str : source) {
	writeable.write(str.data(), str.size());
	writeable.write("\n", 1);
    }
}

/// Write lines to writable.
auto write_lines(Writeable auto& writeable) {
    return [&]<Stream S>(S&& source) {
	return write_lines(std::forward<S>(source), writeable);
    };
}

/// Write lines from the supplied **Stream** `source` to the given `file` uncompressed.
void write_lines_plain(Stream auto source, std::string_view file) {
    std::ofstream ofs{file};
    for (const auto& line : source) {
	ofs.write(line.data(), line.size());
	ofs.write("\n", 1);
    }
}

/// Write lines to file.
inline auto write_lines(std::string_view file) {
    return [=]<Stream S>(S&& source) {
	return write_lines(std::forward<S>(source), file);
    };
}

}; // coro
