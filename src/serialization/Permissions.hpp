#ifndef PEMISSIONS_HPP
#define PEMISSIONS_HPP

#include "TypeList.hpp"
#include <utility>

template <typename T, typename U> class can_serialize;
template <typename T, typename U> class can_deserialize;

template <typename T, typename... Ts>
class can_serialize<T, TypeList<Ts...>> {
	using Yes = char;
	using No = struct { char m[2]; };

	template <typename F>
	static Yes testf(decltype(F::serialize(std::declval<Ts>()...)) *);

	template <typename F>
	static No testf(...);

public:
	static const bool value = sizeof(testf<T>(0)) == sizeof(Yes);
};

template <typename T, typename... Ts>
class can_deserialize<T, TypeList<Ts...>> {
	using Yes = char;
	using No = struct { char m[2]; };

	template <typename F>
	static Yes testf(typename std::enable_if<std::is_same<
		decltype(F::deserialize(std::declval<Ts>()...)), bool>::value>::type *);

	template <typename F>
	static No testf(...);

public:
	static const bool value = sizeof(testf<T>(0)) == sizeof(Yes);
};

#endif // PEMISSIONS_HPP
