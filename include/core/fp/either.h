// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <variant>
#include "core/mp/same.h"
#include "core/fp/detail/decorator.h"
#include "core/fp/detail/any_type.h"
#include "core/fp/either_decl.h"
#include "core/fp/option_decl.h"

namespace core {
inline namespace fp
{
       
/// The EitherLeft template class is intended to be used internally by
/// the Either template class to model a left value of type L. It is
/// intended to be used publicly only in pattern matching cases.
///
/// \note To construct an Either with a left value, use the Left
/// template class.
///
/// \tparam L - The left value type.
template<class L>
struct EitherLeft : Decorator<L> {
    using Base = Decorator<L>;
    using Base::Base;
    template<class R> Either<L,R> as_either() const & { return *this; }
    template<class R> Either<L,R> as_either() && { return std::move(*this); }
};

/// The EitherRight template class is intended to be used internally
/// by the Either template class to model a right value of type R. It
/// is intended to be used publicly only in pattern matching cases.
///
/// \note To construct an Either with a right value, use the Right
/// template class.
///
/// \tparam R - The right value type.
template<class R>
struct EitherRight : Decorator<R> {
    using Base = Decorator<R>;
    using Base::Base;
    template<class L> Either<L,R> as_either() const & { return *this; }
    template<class L> Either<L,R> as_either() && { return std::move(*this); }
};

/// The Either template class is an algebraic type representing either
/// a left value of type `L` or a right value of type `R`.
///
/// An Either contains either an EitherLeft or an EitherRight which
/// can be constructed using the corresponding Left and Right template
/// classes. Either is right-biased meaning that methods such as map,
/// flat_map operate on the right value.
///
/// \tparam L Type of left value
/// \tparam R Type of right value
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// match(either_value,
///     [](const EitherLeft<int>& value) {
/// 	    cout << "The Either contained the int " << value.get() << endl;
///     },
///     [](const EitherRight<string>& value) {
/// 	    cout << "The Either contained the string " << value.get() << endl;
///     });
/// ```
///
/// \endverbatim
///
template<class L, class R>
struct Either : public std::variant<EitherLeft<L>, EitherRight<R>> {
    using Base = std::variant<EitherLeft<L>, EitherRight<R>>;
    using Base::Base;
    
    using MyEither = Either<L, R>;
    using MySwapped = Either<R, L>;

    // Construct an **Either** from a **Right** with no left side
    // type information.
    //
    // \tparam LL Left side type
    // \tparam RR Right side type
    template<class LL, class RR>
    requires (std::is_same_v<LL, AnyType> and not std::is_same_v<RR, AnyType>)
    Either(const Either<LL,RR>& other)
	: Base(other.get_right()) {
    }

    // Construct an **Either** from a **Left** with no right side
    // type information.
    //
    // \tparam LL Left side type
    // \tparam RR Right side type
    template<class LL, class RR>
    requires (not std::is_same_v<LL, AnyType> and std::is_same_v<RR, AnyType>)
    Either(const Either<LL,RR>& other)
	: Base(other.get_left()) {
    }

    /// Returns true if this is a Left; otherwise, false.
    bool is_left() const { return std::holds_alternative<EitherLeft<L>>(*this); }

    /// Returns true if this is a right; otherwise, false.
    bool is_right() const { return std::holds_alternative<EitherRight<R>>(*this); }
    
    /// Returns true if this is a right; otherwise, false.
    operator bool() const { return is_right(); }

    bool operator==(const MyEither& other) const {
	if (is_left() and other.is_left())
	    return get_left() == other.get_left();
	else if (is_right() and other.is_right())
	    return get_right() == other.get_right();
	return false;
    }

    /// Returns the left value or throw.
    const L& get_left() const & { return std::get<EitherLeft<L>>(*this).get(); }

    /// Returns the left value or throw.
    L& get_left() & { return std::get<EitherLeft<L>>(*this).get(); }

    /// Returns the left value or throw.
    L&& get_left() && { return std::move(std::get<EitherLeft<L>>(*this).get()); }

    /// Returns the right value or throw.
    const R& get_right() const & { return std::get<EitherRight<R>>(*this).get(); }

    //// Returns the right value or throw.
    R& get_right() & { return std::get<EitherRight<R>>(*this).get(); }

    /// Returns the right value of throw.
    R&& get_right() && { return std::move(std::get<EitherRight<R>>(*this).get()); }

    /// Returns true if this is a right value equal to `elem`; otherwise, false.
    bool contains(const R& elem) const { return is_right() and get_right() == elem; }

    /// Returns true if this is a right value and the supplied predicate is true.
    template<class P>
    bool exists(P&& predicate) const { return is_right() and predicate(get_right()); }

    /// Returns this **Either** if this is a right and the supplied
    /// predciate is true. Otherwise, returns Left{other}.
    template<class P>
    MyEither filter_or_else(P&& predicate, L&& other) const {
	if (is_right() and predicate(get_right())) return get_right();
	else return Left{std::forward<L>(other)};
    }

    /// Returns the supplied `func` applied to the right value if this
    /// is a right; otherwise, returns this **Either**. `func` must
    /// return a **Right<R>**.
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F, R>>>
    requires std::is_same_v<G, Right<R,AnyType>>
    MyEither flat_map(F&& func) const {
	if (is_right()) return func(get_right());
	else return *this;
    }

    /// Returns `fl` applied to the left value if this is a left or `fr`
    /// applied to the right value if this is a right.
    template<class FL, class FR,
	     class GL = std::remove_cvref_t<std::invoke_result_t<FL, L>>,
	     class GR = std::remove_cvref_t<std::invoke_result_t<FR, R>>>
    Either<GL,GR> fold(FL&& fl, FR&& fr) const {
	if (is_left()) return fl(get_left());
	else return fr(get_right());
    }

    /// Returns true if this is a left; otherwise, returns result of
    /// `predicate` applied to the right value.
    template<class P>
    bool forall(P&& predicate) {
	if (is_right()) return predicate(get_right());
	else return true;
    }

    /// If this is a right, applies the side-effecting function `func`
    /// to the right value. Otherwise; does nothing.
    template<class F>
    void foreach(F&& func) {
	if (is_right()) func(get_right());
    }

    /// If this is a right, return the result of `func` applied to the
    /// right value wrapped in an **Either**. Otherwise, return the
    /// left value wrapped in an **Either**.
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F, R&>>>
    constexpr Either<L,G> map(F&& func) & {
	if (is_right()) return std::invoke(std::forward<F>(func), get_right());
	else return *this;
    }

    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F, const R&>>>
    constexpr Either<L,G> map(F&& func) const & {
	if (is_right()) return std::invoke(std::forward<F>(func), get_right());
	else return *this;
    }
    
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F, R&&>>>
    constexpr Either<L,G> map(F&& func) && {
	if (is_right()) return std::invoke(std::forward<F>(func), std::move(*this).get_right());
	else return std::move(*this);
    }
    
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F>>>
    requires mp::is_same_template_v<G, Either>
    constexpr G or_else(F&& func) const & {
	if (is_right()) return get_right();
	else return std::forward<F>(func)();
    }
    
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F>>>
    requires mp::is_same_template_v<G, Left>
    constexpr Either<typename G::value_type, R> or_else(F&& func) const & {
	if (is_right()) return get_right();
	else return std::forward<F>(func)();
    }
    
    template<class F, class G = std::remove_cvref_t<std::invoke_result_t<F>>>
    requires mp::is_same_template_v<G, Either>
    constexpr G or_else(F&& func) && {
	if (is_right()) return get_right();
	else return std::forward<F>(func)();
    }

    /// Returns the right value or the given value `other`.
    R right_or_else(R&& other) const {
	if (is_right()) return get_right();
	else return other;
    }

    /// Returns a new **Either** with left and right swapped.
    MySwapped swap() const {
	if (is_left()) return MySwapped{get_left()};
	else return MySwapped{get_right()};
    }

    /// Returns a Maybe containing either the right value or nothing.
    Option<R> to_option() const {
	if (is_right()) return get_right();
	else return {};
    }

    /// Returns a vector containing right value or empty.
    std::vector<R> to_vector() const {
	if (is_right()) return {get_right()};
	else return {};
    }
};

/// Use the Left template class to create an Either with a left value.
///
/// \tparam L The Left type
/// \tparam R The Right type
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// auto l = Left{"error"s};
/// ASSERT_TRUE(r.is_left());
/// ```
///
/// \endverbatim
template<class L, class R = AnyType>
struct Left : Either<L,R> {
};
template<class L> Left(const L&) -> Left<L,AnyType>;

/// Use the Right template class to create an Either with a right value.
///
/// \tparam R The Right type
/// \tparam L The Left type
///
/// \verbatim embed:rst:leading-slashes
///
/// ```{code-block} cpp
/// auto r = Right{42};
/// ASSERT_TRUE(r.is_right());
/// ```
///
/// \endverbatim
template<class R, class L = AnyType>
struct Right : Either<L,R> {
};
template<class R> Right(const R&) -> Right<R,AnyType>;

}; // utility
}; // core

