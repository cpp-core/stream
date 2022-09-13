// Copyright (C) 2017, 2019, 2021, 2022 by Mark Melton
//

#pragma once
#include <stdexcept>

namespace core {
inline namespace fp {

auto fold_l(auto begin, auto end, auto value, auto op) {
    while (begin != end)
	value = op(value, *begin++);
    return value;
}

auto fold_l(const auto& c, auto value, auto op) {
    return fold_l(c.begin(), c.end(), value, op);
}

auto fold_l(const auto& c, auto op) {
    if (c.size() == 0)
	throw std::invalid_argument("fold_l: container cannot be empty");
    return fold_l(++c.begin(), c.end(), *c.begin(), op);
}

auto any(const auto& c, auto pred) {
    return fold_l(c, false, [&](bool value, const auto& elem) { return value or pred(elem); });
}
    
auto all(const auto& c, auto pred) {
    return fold_l(c, true, [&](bool value, const auto& elem) { return value and pred(elem); });
}

}; // fp
}; // end ns core
