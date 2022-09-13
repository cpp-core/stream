// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once

namespace coro::detail {
       
// The **Fixed** template class transparently adapts any class `T` to
// be non-copyable and non-moveable. A common use-case is to ensure
// that nothing is attempting to copy or move a resource.
template<class T>
class Fixed : public T {
    using T::T;
    Fixed(const Fixed&) = delete;
    Fixed(Fixed&&) = delete;

    Fixed& operator=(const Fixed&) = delete;
    Fixed& operator=(Fixed&&) = delete;
};

}; // coro::detail

