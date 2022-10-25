// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/sampler/integral.h"

namespace coro::chr {

/// Return a generator of lowercase alpha characters, i.e. a-z.
Generator<char> lower();

/// Return a generator of uppercase alpha characters, i.e. A-Z.
Generator<char> upper();

/// Return a generator of alpha characters, i.e a-z or A-Z.
Generator<char> alpha();

/// Return a generator of alpha-numeric characters, i.e. a-z or A-Z or 0-9.
Generator<char> alphanum();

/// Return a generator of binary digits, i.e. 0-1.
Generator<char> binary();

/// Return a generator of octal digits, i.e. 0-1.
Generator<char> octal();

/// Return a generator of decimal digits, i.e. 0-9.
Generator<char> decimal();

/// Return a generator of hex digits, i.e. 0-9 or a-f, or if upper is true 0-9 or A-F.
Generator<char> hex(bool upper = false);

}; // costr
