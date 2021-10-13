// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/mp/same.h"

namespace costr {

template<class T, template<class...> class C>
constexpr bool is_kind = core::mp::is_same_template_v<T,C>;

template<class T>
using Gen = coro::Generator<T>;

template<class T>
coro::Generator<T> nil() {
    co_return;
}

}; // costr
