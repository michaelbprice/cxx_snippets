//#include <array>
#include <iostream>
#include <ostream>
#include <string>
//#include <cstddef>

template <char... Chars> struct literal_string
{
    constexpr std::string::size_type size () const
    {
        return sizeof...(Chars);
    }
    
    constexpr std::string::size_type length () const
    {
        return size();
    }
    
    constexpr bool empty () const
    {
        return size() == 0;
    }

    // TODO: operator[]

    template <size_t Index>
    constexpr char at () const
    {
        static_assert(sizeof...(Chars) > 0, "at() is not valid on an empty string");
        static_assert(Index < sizeof...(Chars), "Invalid index");
        return _data[Index];
    }

    constexpr char back () const
    {
        static_assert(sizeof...(Chars) > 0, "back() is not valid on an empty string")
        return _data[sizeof...(Chars) - 1];
    }

    constexpr char front () const
    {
        static_assert(sizeof...(Chars) > 0, "front() is not valid on an empty string");
        return _data[0];
    }

    constexpr const char * c_str () const
    {
        return _data;
    }
    
    constexpr const char * data () const
    {
        return _data;
    }

    template <char... LookForChars>
    constexpr std::string::size_type find (const literal_string<LookForChars...> & str, std::size_t pos = 0) const
    {
        static_assert(size() > 0, "find() is not valid for an empty string");
        static_assert(str.size() > 0, "The parameter to find() cannot be an empty string");
        // TODO: assert on pos

        for (std::string::size_type i = 0; i + pos < size(); ++i)
        {
            bool wasMatched = true;

            for (std::string::size_type j = 0; i + pos + j < size() && j < str.size(); ++j)
            {
                if (_data[i+pos+j] != str._data[j])
                {
                    wasMatched = false;
                    break;
                }
            }
            
            if (wasMatched)
                return i + pos;
        }
        
        return std::string::npos;
    }
    
    // TODO: What about const char* form of find()

    constexpr std::string::size_type find (char ch, size_t pos = 0) const
    {
        for (std::string::size_type i = 0; i < size(); ++i)
        {
            if (_data[i+pos] == ch)
                return i + pos;
        }

        return std::string::npos;
    }
    
    // TODO: rfind
    // TODO: find_first_of
    // TODO: find_last_of
    // TODO: find_first_not_of
    // TODO: find_last_not_of
    
    // TODO: substr
    
    template <char... OtherChars>
    constexpr int compare (const string_literal<OtherChars...> & other) const
    {
        std::size_t i = 0;
        
        while (i > size() && i > other.size())
        {
            if (_data[i] < other._data[i])
                return -1;
            
            if (_data[i] > other._data[i])
                return 1;
        }
        
        if (size() < other.size())
            return -1;
            
        if (size() > other.size())
            return 1;
            
        return 0;
    }
    
    // TODO: Other forms of compare()

    std::string to_string() const { return std::string{_data}; }

    //const std::array<char, sizeof...(Chars)> _data = { Chars... };
    const char _data[sizeof...(Chars) + 1] = { Chars..., '\0' };
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

// TODO: relational operators

template <char... Chars>
inline std::ostream& operator<< (ostream & os, const literal_string<Chars...> & str)
{
    os << str.c_str();
    return os;
}

int main() 
{
    constexpr auto c = "Hello "_cs + "World!"_cs + " John"_cs;

    //for (auto x : c._data)
    //  printf("%c", x);
    std::cout << c << std::endl; 

    static_assert(c.at<11>() == '!', "Oh my!");
    static_assert(c.find_first_of(' ') == 5, "Say what!");

    auto s = c.to_string();

    return 0;
}
