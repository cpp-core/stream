// Copyright 2021 by Mark Melton
//

#pragma once
#include <fstream>
#include "coro/stream/util.h"
#include "core/codex/zstd/file_compressor.h"
#include "core/file.h"

namespace coro {

// Write lines from the supplied **Stream** `source` to the given
// `writeable`.
void write_lines(Stream auto source, Writeable auto& writeable) {
    for (const auto& str : source) {
	writeable.write(str.data(), str.size());
	writeable.write("\n", 1);
    }
}

auto write_lines(Writeable auto& writeable) {
    return [&]<Stream S>(S&& source) {
	return write_lines(std::forward<S>(source), writeable);
    };
}

// Write lines from the supplied **Stream** `source` to the given `file` uncompressed.
void write_lines_plain(Stream auto source, core::File file) {
    std::ofstream ofs{file};
    for (const auto& line : source) {
	ofs.write(line.data(), line.size());
	ofs.write("\n", 1);
    }
}

// Write lines from the supplied **Stream** `source` to the given `file` compressed with zstd.
void write_lines_zstd(Stream auto source, core::File file) {
    zstd::FileCompressor c{file};
    for (const auto& line : source) {
	c.write(line.data(), line.size());
	c.write("\n", 1);
    }
}

// Write lines from the supplied **Stream** `source` to the given
// `file`. If `file` ends with ".zstd" using zstd
// compressions; otherwise, write the file uncompressed.
void write_lines(Stream auto source, core::File file) {
    if (file.ends_with(".zst")) write_lines_zstd(std::move(source), file);
    else write_lines_plain(std::move(source), file);
}

inline auto write_lines(core::File file) {
    return [=]<Stream S>(S&& source) {
	return write_lines(std::forward<S>(source), file);
    };
}

}; // coro
