// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <variant>
#include "core/mp/same.h"
#include "core/fp/detail/decorator.h"
#include "core/fp/detail/any_type.h"
#include "core/fp/option_decl.h"
#include "core/fp/either_decl.h"

namespace core {
inline namespace fp
{

/// The OptionSome template class is intended to be used internally by
/// the Option template class to model the presence of a value of type
/// T. It is intended to be used publicly only in pattern matching
/// cases.
///
/// \note To constuct an Option with a value, using the Some template
/// class.
///
/// \tparam T - The value type
template<class T>
struct OptionSome : Decorator<T> {
    using Base = Decorator<T>;
    using Base::Base;
    using value_type = T;
};
template<class T> OptionSome(const T&) -> OptionSome<T>;

/// The OptionNone template class is intended to be used internally by
/// the Option template class to model the absence of a value of type
/// T. It is intened to be used publicly only in pattern matching
/// cases.
///
/// \note To construct an Option without a value, use the None
/// template class.
///
/// \tparam T - The value type
template<class T>
struct OptionNone {
    using value_type = T;
};

/// The Option template class is an algebraic sum type that models the
/// presence or absence of a value of type T.
///
/// An Option either contains an OptionSome or an OptionNone which
/// can be constructed using the corresponding Some and None template
/// classes.
///
/// \tparam T The value type
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// match(option_value,
///     [](const OptionSome<int>& value) {
/// 	    cout << "The value contained the int " << value.get() << endl;
///     },
///     [](const OptionNone&) {
/// 	    cout << "The value contained nothing" << endl;
///     });
/// ```
///
/// \endverbatim
///
template<class T>
struct Option : public std::variant<OptionSome<T>,OptionNone<T>> {
    using Base = std::variant<OptionSome<T>,OptionNone<T>>;
    using Base::Base;
    using value_type = T;
    
    /// Construct an empty **Option**.
    Option() : Base(OptionNone<T>{}) { }

    Option(const Option<AnyType>&) : Base(OptionNone<T>{}) { }

    /// Return true if this **Option** contains a value.
    bool has_value() const { return std::holds_alternative<OptionSome<T>>(*this); }

    /// Return true if this **Option** contains a value.
    operator bool() const { return has_value(); }

    /// Return the contained value or throw if the **Option** is empty.
    const T& get() const & { return std::get<OptionSome<T>>(*this).get(); }

    /// Returns the contained value or throw if the **Option** is empty.
    T& get() & { return std::get<OptionSome<T>>(*this).get(); }

    /// Returns the contained value or throw if the **Option** is empty.
    T&& get() && { return std::move(std::get<OptionSome<T>>(*this).get()); }

    /// Return the contained value or throw if the **Option** is empty.
    const T& operator*() const & { return std::get<OptionSome<T>>(*this).get(); }

    /// Returns the contained value or throw if the **Option** is empty.
    T& operator*() & { return std::get<OptionSome<T>>(*this).get(); }

    /// Returns the contained value or throw if the **Option** is empty.
    T&& operator*() && { return std::move(std::get<OptionSome<T>>(*this).get()); }

    /// Returns true if this **Option** is not empty and contains
    /// `elem`; otherwise, false.
    bool contains(const T& elem) const { return has_value() and get() == elem; }

    /// Returns true if this **Option** is not empty and the supplied
    /// predicate is true.
    template<class P>
    bool exists(P&& predicate) const { return has_value() and predicate(get()); }

    /// Returns this **Option** if this contains a value and the supplied
    /// predciate is true. Otherwise, returns None{};
    template<class P>
    Option filter(P&& predicate) const {
	if (has_value() and predicate(get())) return *this;
	else return OptionNone<T>{};
    }

    /// If this **Option** contains a value, returns the result of
    /// applying `func` to the value. Otherwise, returns None<T>.
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F, T>>>
    requires mp::is_same_template_v<G, Option>
    G flat_map(F&& func) const {
	if (has_value()) return func(get());
	else return OptionNone<typename G::value_type>{};
    }

    /// Flatten

    /// Fold
    template<class FE,
	class F,
	class GE = std::remove_cvref_t<std::invoke_result_t<FE>>,
	class G = std::remove_cvref_t<std::invoke_result_t<F, T>>>
    requires std::is_same_v<G, GE>
    auto fold(FE&& f_empty, F&& f) {
	if (has_value()) return f(get());
	else return f_empty();
    }

    /// If this **Option** has a value, returns the result of applying
    /// the supplied `predicate` to the contained value; otherwise, if
    /// this **Option** is empty, returns true.
    template<class P>
    bool forall(P&& predicate) const {
	if (has_value()) return predicate(get());
	else return true;
    }

    /// If this **Option** has a value, apply the supplied function
    /// `func` to the value.
    template<class F>
    void foreach(F&& func) {
	if (has_value())
	    func(get());
    }

    /// If this **Option** has a value, return the value; otherwise,
    /// return the supplied `alternate` value.
    T get_or_else(T&& alternate) {
	if (has_value()) return get();
	else return std::forward<T>(alternate);
    }

    /// If this **Option** has a value, return the value; otherwise,
    /// return the supplied `alternate` value.
    T get_or_else(T&& alternate) const {
	if (has_value()) return get();
	else return std::forward<T>(alternate);
    }

    /// If this **Option** has a value, returns a **Option** containing
    /// the result of the supplied `func` applied to the
    /// value. Otherwise, returns an empty **Option**.
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F, T>>>
    Option<G> map(F&& func) {
	if (has_value()) return OptionSome(func(get()));
	else return OptionNone<G>{};
    }

    /// Returns this **Option** if it has a value; otherwise, returns
    /// the result of `func`.
    template<class F, class R = std::invoke_result_t<F>>
    requires std::is_same_v<R, Option<T>>
    Option<T> or_else(F&& func) const {
	if (has_value()) return get();
	else return std::forward<F>(func)();
    }

    /// If this **Option** has a value, return an **Either** with the
    /// contained value as the left value. Otherwise; return an Either
    /// with the supplied argument `right` as the right value.
    template<class R>
    requires (not std::is_same_v<T, AnyType>)
    Either<T,R> to_left(R&& right) {
	if (has_value()) return Left{get()};
	else return Right{std::forward<R>(right)};
    }

    // If this **Option** has a value, return an **Either** with the
    // contained value as the left value. Otherwise; return an Either
    // with the supplied argument `right` as the right value.
    template<class R>
    requires (std::is_same_v<T, AnyType>)
    Either<T,R> to_left(R&& right) {
	return Right{std::forward<R>(right)};
    }

    /// If this **Option** has a value, return an **Either** with the
    /// contained value as the right value. Otherwise; return an Either
    /// with the supplied argument `left` as the left value.
    template<class L>
    requires (not std::is_same_v<T, AnyType>)
    Either<L,T> to_right(L&& left) {
	if (has_value()) return Right{get()};
	else return Left{std::forward<L>(left)};
    }
    
    // If this **Option** has a value, return an **Either** with the
    // contained value as the right value. Otherwise; return an Either
    // with the supplied argument `left` as the left value.
    template<class L>
    requires (std::is_same_v<T, AnyType>)
    Either<L,T> to_right(L&& left) {
	return Left{std::forward<L>(left)};
    }
};

/// Use the Some template class to create an instance of Option with a
/// value.
///
/// \tparam T The value type
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// auto value = Some{42};
/// ASSERT_TRUE(value);
/// ```
///
/// \endverbatim
///
template<class T>
struct Some : Option<T> {
    using Base = Option<T>;
    using Base::Base;
};
template<class T> Some(const T&) -> Some<T>;

/// Use the None template class to create an instance of Option
/// without a value.
///
/// \tparam T The value type
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// auto value = None{42};
/// ASSERT_FALSE(value);
/// ```
///
/// \endverbatim
///
template<class T = AnyType>
struct None : Option<T> {
    using Base = Option<T>;
    using Base::Base;
    using value_type = T;
    
    None() { }
    None(const T&) = delete;
    None& operator=(const T&) = delete;
    None(T&&) = delete;
    None& operator=(T&&) = delete;
};

template<class T>
std::ostream& operator<<(std::ostream& os, const Option<T>& value) {
    if (value) os << value.get();
    else os << "?";
    return os;
}

}; // utility
}; // core
