// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/sampler/integral.h"

namespace coro::chr {

// Return a generator of lowercase alpha characters, i.e. a-z.
Gen<char> lower();

// Return a generator of uppercase alpha characters, i.e. A-Z.
Gen<char> upper();

// Return a generator of alpha characters, i.e a-z or A-Z.
Gen<char> alpha();

// Return a generator of alpha-numeric characters, i.e. a-z or A-Z or 0-9.
Gen<char> alphanum();

// Return a generator of hex digits, i.e. 0-9 or a-f, or if upper is true 0-9 or A-F.
Gen<char> hex(bool upper = false);

}; // costr
