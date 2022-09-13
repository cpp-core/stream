// Copyright (C) 2017, 2019, 2022 by Mark Melton
//

#include "coro/stream/detail/random.h"

namespace coro::detail {

static std::mt19937 gs_rng;

std::mt19937& rng() { return gs_rng; }

};
