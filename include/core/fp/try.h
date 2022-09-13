// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <string>
#include <variant>
#include "core/mp/same.h"
#include "core/fp/detail/decorator.h"
#include "core/fp/detail/any_type.h"
#include "core/fp/try_decl.h"
#include "core/fp/either_decl.h"
#include "core/fp/option.h"

namespace core {
inline namespace fp
{

template<class T>
struct TrySuccess : Decorator<T> {
    using Base = Decorator<T>;
    using Base::Base;
};

struct TryFailure {
    // Returns true if this Failure contains an exception.
    operator bool() const { return !!eptr_; }

    // Rethrows the contained excepition (or a runtime error if no
    // exception).
    void rethrow() const {
	if (not eptr_)
	    throw std::runtime_error("Failure: attempt to rethrow with no exception");
	std::rethrow_exception(eptr_);
    }

    // Returns a string decribing the contained exception.
    std::string what() const {
	if (not eptr_)
	    return "no exception";
	try {
	    std::rethrow_exception(eptr_);
	} catch (const std::exception& e) {
	    return e.what();
	} catch (const std::string& s) {
	    return s;
	} catch (const char *p) {
	    return std::string{p};
	} catch (...) {
	    return "exception with no information";
	}
    }
    
    std::exception_ptr eptr_;
};

/// The Try template class is an algebraic sum type that models a
/// Success wsih tyep T or a Failure with an exception.
///
/// To construct a Try instance, use either the Success template class
/// or the Failure template class.
///
/// \tparam T The Success type
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// match(try_value,
///     [](const TrySuccess<int>& value) {
/// 	    cout << "The Try contained the int " << value.get() << endl;
///     },
///     [](const TryFailure& value) {
/// 	    cout << "The Try contains an error" << endl;
///     });
/// ```
///
/// \endverbatim
///
template<class T>
struct Try : public std::variant<TrySuccess<T>, TryFailure> {
    using Base = std::variant<TrySuccess<T>, TryFailure>;
    using Base::Base;

    Try(const Try<AnyType>& other)
	: Base(other.get_failure()) {
    }

    /// Returns true if this **Try** contains a success value.
    bool is_success() const { return not is_failure(); }

    /// Returns true if this **Try** contains a failure.
    bool is_failure() const { return std::holds_alternative<TryFailure>(*this); }

    /// Evaluates to true in a bool context if this **Try** contains a
    /// success value.
    explicit operator bool() const { return is_success(); }

    /// Returns the succcess value or throws.
    const T& get() const & { return std::get<TrySuccess<T>>(*this).get(); }
    
    /// Returns the succcess value or throws.
    T& get() & { return std::get<TrySuccess<T>>(*this).get(); }
    
    /// Returns the succcess value or throws.
    T&& get() && { return std::move(std::get<TrySuccess<T>>(*this)).get(); }

    /// Returns the failure value or throws.
    const TryFailure& get_failure() const & { return std::get<TryFailure>(*this); }

    /// Returns the failure value or throws.
    TryFailure& get_failure() & { return std::get<TryFailure>(*this); }

    /// Returns the failure value or throws.
    TryFailure&& get_failure() && { return std::move(std::get<TryFailure>(*this)); }

    /// Returns the current **Success** if this **Try** is a success
    /// and the given predicate returns true when applied to the
    /// success value; otherwise, returns a **Failure**.
    template<class P>
    Try<T> filter(P&& predicate) const {
	if (is_success() and predicate(get())) return *this;
	else return TryFailure{};
    }

    /// Returns the result of the supplied `func` applied the the
    /// succcess value if this **Try** is a **Success**; otherwise,
    /// returns this **Failure**.
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F, T>>>
    requires (mp::is_same_template_v<G, Try> ||
	      mp::is_same_template_v<G, Success> ||
	      mp::is_same_template_v<G, Failure>)
    G flat_map(F&& func) const {
	if (is_success()) return func(get());
	else return TryFailure{};
    }

    /// Returns `ff` applied to the failure value or `fs` applied to
    /// the success value.
    template<class FF, class FS,
	     class GF = std::remove_cvref_t<std::invoke_result_t<FF, TryFailure>>,
	     class GS = std::remove_cvref_t<std::invoke_result_t<FS, T>>,
	     class G = std::common_type_t<GF,GS>>
    G fold(FF&& ff, FS&& fs) {
	if (is_success()) return fs(get());
	else return ff(get_failure());
    }

    /// Applies the supplied `func` to the success value or does
    /// nothing if this is a failure.
    template<class F>
    void foreach(F&& func) {
	if (is_success())
	    func(get());
    }

    /// Returns the success value or the supplied value if this is a
    /// failure.
    const T& get_or_else(const T& value) const {
	if (is_success()) return get();
	else return value;
    }

    /// If this is a **Succcess**, returns the result of `func` applied
    /// to the success value as a **Try**; otherwise, if this is a
    /// **Failure**, returns the failure value as a **Try**.
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F, T>>>
    Try<G> map(F&& func) const {
	if (is_success()) return func(get());
	else return get_failure();
    }

    /// If this is a **Success**, returns this **Try**; otherwise,
    /// returns the supplied `value`.
    Try<T> or_else(const Try<T>& value) const {
	if (is_success()) return *this;
	else return value;
    }

    /// If this is a **Success**, returns **Right** of the success
    /// value; otherwise, returns **Left** of the failure value.
    Either<std::string,T> to_either() const requires (not std::is_same_v<T, AnyType>) {
	if (is_success()) return Right{get()};
	else return Left{get_failure().what()};
    }

    // If this is a **Success**, returns **Right** of the success
    // value; otherwise, returns **Left** of the failure value.
    Either<std::string,T> to_either() const requires (std::is_same_v<T, AnyType>) {
	return Left{get_failure().what()};
    }

    /// If this is a **Success**, returns **Some** of the success
    /// value; otherwise, returns **None**.
    Option<T> to_option() const requires (not std::is_same_v<T, AnyType>) {
	if (is_success()) return Some(get());
	else return None{};
    }

    // If this is a **Success**, returns **Some** of the success
    // value; otherwise, returns **None**.
    Option<T> to_option() const requires (std::is_same_v<T, AnyType>) {
	return None{};
    }

    /// if this is a **Success**, returns the result of `succeed`
    /// applied to the success value; otherwise, returns the result of
    /// `fail` applied to the failure value.
    template<class Fail, class Succeed,
	     class FailR = std::remove_cvref_t<std::invoke_result_t<Fail, TryFailure>>,
	     class SucceedR = std::remove_cvref_t<std::invoke_result_t<Succeed, T>>>
    requires (mp::is_same_template_v<SucceedR, Try> ||
	      mp::is_same_template_v<SucceedR, Success> ||
	      mp::is_same_template_v<SucceedR, Failure>)
    SucceedR transform(Fail&& fail, Succeed&& succeed) {
	if (is_success()) return succeed(get());
	else return fail(get_failure());
    }
};

/// Use the Success tmeplate class to construct a Try with a Success
/// value.
///
/// \tparam T The success type.
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// auto value = Success{42};
/// ASSERT_TRUE(value);
/// ```
///
/// \endverbatim
template<class T>
struct Success : Try<T> {
    using Base = Try<T>;
    using Base::Base;
};
template<class T> Success(const T&) -> Success<T>;

/// Use the Failure template class to construct a Try with an Failure
/// exception.
///
/// \tparam T The success type.
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// auto value = Failure{std::runtime_error("bad")};
/// ASSERT_FALSE(value);
/// ```
///
/// \endverbatim
template<class T = AnyType>
struct Failure : Try<T> {
    using Base = Try<T>;
    using Base::Base;
    
    /// Constructs a **Failure** from an exception pointer.
    Failure(std::exception_ptr&& eptr) : Base(TryFailure{std::forward<std::exception_ptr>(eptr)}){}
    /// Constructs a **Failure** from an exception.
    Failure(const std::exception& e) : Base(TryFailure{std::make_exception_ptr(e)}) { }
    /// Constructs an empty **Failure**.
    Failure() : Base(TryFailure{}) { }
};

}; // utility
}; // core

