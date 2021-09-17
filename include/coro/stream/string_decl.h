// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/integral.h"

namespace costr::str {

Gen<string> sample(Gen<size_t> size, Gen<char> src);
Gen<string> uniform(Gen<size_t>&& size = costr::uniform<size_t>(0, 20));
Gen<string> lowercase(Gen<size_t>&& size = costr::uniform<size_t>(0, 20));
Gen<string> uppercase(Gen<size_t>&& size = costr::uniform<size_t>(0, 20));
Gen<string> alpha(Gen<size_t>&& size = costr::uniform<size_t>(0, 20));
Gen<string> alphanum(Gen<size_t>&& size = costr::uniform<size_t>(0, 20));
Gen<string> ident(Gen<size_t>&& size = costr::uniform<size_t>(0, 20));

}; // costr
