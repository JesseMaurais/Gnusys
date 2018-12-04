#ifndef membuf_hpp
#define membuf_hpp

#include <string>
#include <string_view>
#include <streambuf>

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_membuf : public std::basic_streambuf<Char, Traits<Char>>
	{
		using base = std::basic_streambuf<Char, Traits<Char>>;
		using string = std::basic_string<Char, Traits<Char>, Alloc<Char>>;
		using string_view = std::basic_string_view<Char, Traits<Char>>;

	public:

		using char_type = typename base::char_type;
		using size_type = std::streamsize;

		basic_membuf() = default;

		basic_membuf(size_type n)
		{
			setbufsz(n);
		}

		base *setbuf(char_type *s, size_type n) override
		{
			size_type const m = n / 2;
			return setbuf(s, n - m, m);
		}

		base *setbuf(char_type *s, size_type n, size_type m)
		{
			auto t = s + n;
			auto u = t + m;
			base::setg(s, t, t);
			base::setp(t, u);
			return this;
		}

		base *setbufsz(size_type n)
		{
			buf.resize(n);
			return setbuf(buf.data(), n);
		}

		base *setbufsz(size_type n, size_type m)
		{
			buf.resize(n + m);
			return setbuf(buf.data(), n, m);
		}

		string_view pview() const
		{
			auto const sz = base::pptr() - base::pbase();
			return string_view(base::pbase(), sz);
		}

		string_view gview() const
		{
			auto const sz = base::egptr() - base::gptr();
			return string_view(base::gptr(), sz);
		}

	private:

		string buf;
	};

	using membuf = basic_membuf<char>;
	using wmembuf = basic_membuf<wchar_t>;
}

#endif // file
