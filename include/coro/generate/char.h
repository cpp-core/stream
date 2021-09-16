// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/char_decl.h"

namespace cogen {

coro::Generator<char> uniform_lowercase() {
    return uniform('a', 'z');
}

coro::Generator<char> uniform_uppercase() {
    return uniform('A', 'Z');
}

}; // cogen
