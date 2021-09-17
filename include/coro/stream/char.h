// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/char_decl.h"

namespace costr {

coro::Generator<char> uniform_lowercase() {
    return uniform('a', 'z');
}

coro::Generator<char> uniform_uppercase() {
    return uniform('A', 'Z');
}

}; // costr
