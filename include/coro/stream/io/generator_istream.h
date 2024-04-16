// Copyright 2021, 2022, 2024 by Mark Melton
//

#pragma once
#include <streambuf>

#include "coro/stream/util.h"

namespace coro {

template <class T, class CharT = char, class TraitsT = std::char_traits<CharT>>
class generator_istreambuf : public std::streambuf {
public:
    generator_istreambuf(coro::Generator<T> &&g) : _g(std::move(g)) {}

    virtual ~generator_istreambuf() {}

    virtual int underflow() override {
        if (_g.next()) {
            const auto& str = _g();
            setg((CharT*)str.data(), (CharT*)str.data(), (CharT*)str.data() + str.size());
            return std::char_traits<CharT>::to_int_type(*this->gptr());
        } else {
            return std::char_traits<CharT>::eof();
        }
    }

private:
    coro::Generator<T> _g;
};

template <class T, class CharT = char, class TraitT = std::char_traits<CharT>>
class generator_istream : public std::basic_istream<CharT, TraitT> {
public:
    generator_istream(coro::Generator<T> &&g)
        : std::basic_istream<CharT, TraitT>::basic_istream(
              new generator_istreambuf(std::move(g))) {}

private:
};

}; // coro
