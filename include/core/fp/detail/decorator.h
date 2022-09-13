// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <utility>

namespace core {
inline namespace type {
       
// The **Decorator** class template
template<class T>
struct Decorator {
    Decorator(const T& value) : value_(value) { }
    Decorator(T&& value) : value_(std::forward<T>(value)) { }
    
    const T& get() const & { return value_; }
    T& get() & { return value_; }
    T&& get() && { return std::move(value_); }
    
private:
    T value_;
};

}; // type
}; // core
