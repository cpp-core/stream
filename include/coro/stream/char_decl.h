// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/integral.h"

namespace costr {

coro::Generator<char> uniform_lowercase();
coro::Generator<char> uniform_uppercase();

}; // costr
