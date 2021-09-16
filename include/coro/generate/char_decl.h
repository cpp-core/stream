// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/integral.h"

namespace cogen {

coro::Generator<char> uniform_lowercase();
coro::Generator<char> uniform_uppercase();

}; // cogen
