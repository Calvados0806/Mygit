#ifndef TYPELIST_HPP
#define TYPELIST_HPP

#include <cstddef>

template <typename... Types> struct TypeList;

template <typename T, typename... Types>
struct TypeList<T, Types...> {
	using Head = T;
	using Tail = TypeList<Types...>;
	static const size_t length = 1 + sizeof...(Types);
};

template <typename T, typename Tl> struct Cons;

template <typename T, typename... Types>
struct Cons<T, TypeList<Types...>> {
	using type = TypeList<T, Types...>;
};


template <typename Tl>
struct CutLast {
	using type = typename Cons<typename Tl::Head, typename CutLast<typename Tl::Tail>::type>::type;
};

template <typename T, typename U>
struct CutLast<TypeList<T, U>> {
	using type = TypeList<T>;
};

#endif // TYPELIST_HPP
