#include <array>
#include <string>
#include <cstddef>

template <char... Chars> struct literal_string
{
    constexpr size_t size () const { return sizeof...(Chars); }
    constexpr size_t length () const { return size(); }

    template <size_t Index>
    constexpr char at () const { static_assert(Index < sizeof...(Chars), "Invalid index"); return _data[Index]; }

    constexpr char front () const { return _data[0]; }
    constexpr char back () const { return _data[sizeof...(Chars) - 1]; }

    constexpr bool empty () const { return size() == 0; }

    constexpr size_t find_first_of (char ch) const
    {
        for (size_t i = 0; i < size(); ++i)
        {
            if (_data[i] == ch) return i;
        }

        return std::string::npos;
    }

    std::string get_mutable() const { return std::string{Chars...}; }

    const std::array<char, sizeof...(Chars)> _data = { Chars... };
};

template <typename charT, charT... Chars>
inline constexpr literal_string<Chars...> operator"" _cs ()
{ return literal_string<Chars...>(); }

template <char... LeftChars, char... RightChars>
inline constexpr auto operator+ (const literal_string<LeftChars...> &l,
                                 const literal_string<RightChars...> &r)
-> literal_string<LeftChars..., RightChars...>
{
    return literal_string<LeftChars..., RightChars...>();
}


int main() 
{
    constexpr auto c = " Hello "_cs + "World!"_cs + " John"_cs;

    for (auto x : c._data)
      printf("%c", x);

    static_assert(c.at<11>() == '!', "Oh my!");
    static_assert(c.find_first_of(' ') == 5, "Say what!");

    //auto s = c.get_mutable();

    return 1;
}
