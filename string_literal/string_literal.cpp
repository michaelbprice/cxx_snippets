
char * gets(char *); // works around silly C++11/14 library issue on ToT clang

#include <iostream>
#include <ostream>
#include <string>

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

    constexpr char operator[] (size_t pos) const
    {
        static_assert(sizeof...(Chars) > 0, "operator[] is not valid on an empty string");
        return _data[pos];
    }

    template <size_t Index>
    constexpr char at () const
    {
        static_assert(sizeof...(Chars) > 0, "at<Index>() is not valid on an empty string");
        static_assert(Index < sizeof...(Chars), "Invalid index");
        return _data[Index];
    }

    constexpr char at (size_t pos) const
    {
        static_assert(sizeof...(Chars) > 0, "at() is not valid on an empty string");
        return _data[pos];
    }

    constexpr char back () const
    {
        static_assert(sizeof...(Chars) > 0, "back() is not valid on an empty string");
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
        static_assert(sizeof...(Chars) > 0, "find() is not valid for an empty string");
        static_assert(sizeof...(LookForChars) > 0, "The parameter to find() cannot be an empty string");

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
    
    template <char... LookForChars>
    constexpr std::string::size_type rfind (const literal_string<LookForChars...> & str, std::size_t pos = sizeof...(Chars)) const
    {
        static_assert(sizeof...(Chars) > 0, "find() is not valid for an empty string");
        static_assert(sizeof...(LookForChars) > 0, "The parameter to find() cannot be an empty string");

        for (/* empty init */; pos > 0; --pos)
        {
            for (std::string::size_type j = 0; pos + j < size() && j < str.size(); ++j)
            {
                if (_data[pos+j] != str._data[j])
                    break;

                if (j == str.size() - 1)
                    return pos;
            }
        }
        
        return std::string::npos;
    }

    // TODO: What about const char* form of rfind()

    constexpr std::string::size_type rfind (char ch, size_t pos = sizeof...(Chars)) const
    {
        for (/* empty init */; pos > 0; --pos)
        {
            if (_data[pos] == ch)
                return pos;
        }

        return std::string::npos;
    }

    // TODO: find_first_of
    // TODO: find_last_of
    // TODO: find_first_not_of
    // TODO: find_last_not_of
    
    // TODO: substr
    
    template <char... OtherChars>
    constexpr int compare (const literal_string<OtherChars...> & other) const
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

    // Conversions
    std::string to_string() const { return std::string{_data}; }

    constexpr operator const char* () const { return _data; }

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
inline std::ostream& operator<< (std::ostream & os, const literal_string<Chars...> & str)
{
    os << str.c_str();
    return os;
}

template <typename Str>
struct Speaker
{
    Speaker() { std::cout << "Unknown" << std::endl; }
};

template <>
struct Speaker<decltype("Oh my!"_cs)>
{
    Speaker() { std::cout << "George Takei" << std::endl; }
};

template <>
struct Speaker<decltype("Engage!"_cs)>
{
    Speaker() { std::cout << "Sir Patrick Stewart" << std::endl; }
};

template <typename Str>
void speak(const Str &)
{
    std::cout << "Function: Unknown" << std::endl;
}

template <>
void speak(const decltype("Oh My!"_cs) &)
{
    std::cout << "Function: George Takei" << std::endl;
}

int main() 
{
    constexpr auto c = "Hello "_cs + "World!"_cs;

    std::cout << c << std::endl; 

    static_assert(c.find('?') == std::string::npos, "I'm so so sorry");
    static_assert(c.at<11>() == '!', "Oh my!");
    static_assert(c[10] == 'd', "Crapola");
    static_assert(c.find(' ') == 5, "Say what!");
    static_assert(c.find('o') == 4, "No good");
    static_assert(c.rfind('o') == 7, "Ooops");
    static_assert(c.find("orl"_cs) == 7, "Holla");
    static_assert(c.rfind("el"_cs) == 1, "Ooops");

    auto s = c.to_string();

    s = "I don't think so";
    std::cout << s << std::endl;

    const char * old = "My word!"_cs;
    std::cout << old << std::endl;

    auto g = Speaker<decltype("Oh my!"_cs)>{};
    auto p = Speaker<decltype("Engage!"_cs)>{};
    auto b = Speaker<decltype("Resistance is futile"_cs)>{};

    speak("My name is Neo"_cs);
    speak("Oh My!"_cs);

    return 0;
}

