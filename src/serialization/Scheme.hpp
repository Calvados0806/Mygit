#ifndef SCHEME_HPP
#define SCHEME_HPP

#include "TypeList.hpp"
#include "Permissions.hpp"

#include <ostream>

template <typename F,
	typename NextSerializer,
	typename OutStrm,
	typename TL,
	bool serializable>
	struct SchemeSerializer;

template <typename F, typename NextSerializer, typename OutStrm>
struct SchemeSerializer<F, NextSerializer, OutStrm, TypeList<>, false> {
	// Do nothing
};

template <typename F, typename NextSerializer, typename OutStrm>
struct SchemeSerializer<F, NextSerializer, OutStrm, TypeList<>, true> {

	template <typename... TailArgs>
	static void serialize(OutStrm &out, TailArgs&&... args)
	{
		F::serialize(out);

		NextSerializer::serialize(out, std::forward<TailArgs>(args)...);
	}
};

template <typename F, typename NextSerializer, typename OutStrm, typename... Types>
struct SchemeSerializer<F, NextSerializer, OutStrm, TypeList<Types...>, false> :
	public SchemeSerializer<F, NextSerializer, OutStrm,
	typename CutLast<TypeList<Types...>>::type,
	can_serialize<F,
	typename CutLast<TypeList<OutStrm, Types...>>::type>::value> {

};

template <typename F, typename NextSerializer, typename OutStrm, typename... Types>
struct SchemeSerializer<F, NextSerializer, OutStrm, TypeList<Types...>, true> {

	template <typename... TailArgs>
	static void serialize(OutStrm &out, Types... args, TailArgs&&... targs)
	{
		F::serialize(out, std::forward<Types>(args)...);

		NextSerializer::serialize(out, std::forward<TailArgs>(targs)...);
	}
};

template <typename F,
	typename NextDeserializer,
	typename InputStrm,
	typename TL,
	bool deserializable>
	struct SchemeDeserializer;

template <typename F, typename NextDeserializer, typename InputStrm>
struct SchemeDeserializer<F, NextDeserializer, InputStrm, TypeList<>, false> {
	// Do nothing
};

template <typename F, typename NextDeserializer, typename InputStrm>
struct SchemeDeserializer<F, NextDeserializer, InputStrm, TypeList<>, true> {

	template <typename... TailArgs>
	static bool deserialize(InputStrm &in, TailArgs&&... args)
	{
		F::deserialize(in);
		return NextDeserializer::deserialize(in, std::forward<TailArgs>(args)...);
	}
};

template <typename F, typename NextDeserializer, typename InputStrm, typename... Types>
struct SchemeDeserializer<F, NextDeserializer, InputStrm, TypeList<Types...>, false> :
	public SchemeDeserializer<F, NextDeserializer, InputStrm,
	typename CutLast<TypeList<Types...>>::type,
	can_deserialize<F,
	typename CutLast<TypeList<InputStrm, Types...>>::type>::value>
{
	// Do nothing
};

template <typename F, typename NextDeserializer, typename InputStrm, typename... Types>
struct SchemeDeserializer<F, NextDeserializer, InputStrm, TypeList<Types...>, true> {

	template <typename... TailArgs>
	static bool deserialize(InputStrm &in, Types... args, TailArgs&&... targs)
	{
		return F::deserialize(in, std::forward<Types>(args)...) &&
			NextDeserializer::deserialize(in, std::forward<TailArgs>(targs)...);
	}
};

template <typename... Types> struct Scheme;

template<>
struct Scheme<> {
	template<typename OutputStream>
	static void serialize(OutputStream&)
	{
		// Do nothing
	}
	template<typename InputStream>
	static bool deserialize(InputStream&)
	{
		return true;
	}
};

template <typename F, typename... Types>
struct Scheme<F, Types...> {

	template <typename OutputStream, typename... Args>
	static void serialize(OutputStream &out, Args&&... args)
	{
		SchemeSerializer<F,
			Scheme<Types...>,
			OutputStream&,
			TypeList<Args...>,
			can_serialize<F, TypeList<OutputStream&, Args...>>::value
		>::serialize(out, std::forward<Args>(args)...);
	}

	template <typename InputStream, typename... Args>
	static bool deserialize(InputStream &in, Args&&... args)
	{
		return SchemeDeserializer<F,
			Scheme<Types...>,
			InputStream&,
			TypeList<Args...>,
			can_deserialize<F, TypeList<InputStream&, Args...>>::value
		>::deserialize(in, std::forward<Args>(args)...);
	}
};

#endif // SCHEME_HPP
