#ifndef STDFIELDS_HPP
#define STDFIELDS_HPP

#include <vector>
#include <cinttypes>
#include <type_traits>

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& space_delimiter(std::basic_ostream<CharT, Traits>& os)
{
	return os << CharT(' ');
}

template <typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& space_delimiter(std::basic_istream<CharT, Traits>& is)
{
	return is;
}

struct IntField {
	template <typename OutputStream, typename IntType>
	static void serialize(OutputStream &out, IntType v)
	{
		static_assert(std::is_integral<IntType>::value, "Integral type expected");
		out << v;
	}

	template <typename InputStream, typename IntType>
	static bool deserialize(InputStream &in, IntType &v)
	{
		static_assert(std::is_integral<IntType>::value, "Integral type expected");
		in >> v;
		return !in.rdstate();
	}
};

struct FloatPointField {
	template <typename OutputStream>
	static void serialize(OutputStream &out, float v)
	{
		out << v;
	}

	template <typename OutputStream>
	static void serialize(OutputStream &out, double v)
	{
		out << v;
	}

	template <typename InputStream>
	static bool deserialize(InputStream &in, float &v)
	{
		in >> v;
		return !in.rdstate();
	}

	template <typename InputStream>
	static bool deserialize(InputStream &in, double &v)
	{
		in >> v;
		return !in.rdstate();
	}
};

template <typename TypeField>
struct DynamicArrayField {
	template <typename OutputStream, typename Type, typename SizeType = size_t>
	static void serialize(OutputStream &out, const Type *data, SizeType n)
	{
		static_assert(std::is_integral<SizeType>::value, "Integral type expected");

		IntField::serialize(out, n);
		out << space_delimiter;
		for (SizeType i = 0; i < n; i++) {
			TypeField::serialize(out, data[i]);
			out << space_delimiter;
		}
	}

	template <typename OutputStream, typename Type>
	static void serialize(OutputStream &out, const std::vector<Type> &v)
	{
		serialize(out, v.data(), v.size());
	}

	template <typename InputStream, typename Type, typename SizeType = size_t>
	static bool deserialize(InputStream &in, Type *&data, SizeType &n)
	{
		static_assert(std::is_integral<SizeType>::value, "Integral type expected");

		bool status = IntField::deserialize(in, n);
		if (status) {
			data = new Type[n];
			for (SizeType i = 0; i < n && status; i++)
				status = TypeField::deserialize(in, data[i]);
		}

		return status;
	}

	template <typename InputStream, typename Type>
	static bool deserialize(InputStream &in, std::vector<Type> &v)
	{
		size_t n;
		Type temp;
		bool status = IntField::deserialize(in, n);
		if (status) {
			for (size_t i = 0; i < n && status; i++) {
				status = TypeField::deserialize(in, temp);
				v.push_back(temp);
			}
		}

		return status;
	}
};

#endif // STDFIELDS_HPP
