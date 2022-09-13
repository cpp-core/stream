// Copyright (C) 2019, 2020, 2021, 2022 by Mark Melton
//

#pragma once
#include <variant>

namespace core {
inline namespace utility {

template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template<class... Ts> 
overloaded(Ts...) -> overloaded<Ts...>;
       
/// Pattern match a `std::variant` derived type against its contained type.
///
/// \tparam T The variant type
/// \tparam Us... The lambda types
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// match(try_value,
///     [](const TrySuccess<int>& value) {
///         cout << "The Try contained the value " << value.get() << endl;
///     },
///     [](const TryFailure& fail) {
///         cout << "Try Try contained a failure"
///     });
/// ```
///
/// \endverbatim
/// 
template<class T, class... Us>
auto match(T&& variant, Us&&... handlers) {
    return std::visit(overloaded{std::forward<Us>(handlers)...}, std::forward<T>(variant));
}

}; // utility
}; // core
