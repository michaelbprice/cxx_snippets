
//char * gets(char *); // works around silly C++11/14 library issue on ToT clang

#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>

namespace std {

#pragma region Some forward declarations

template <typename charT, charT... Chars>
class literal_string;

template <typename charT, charT... Chars>
constexpr literal_string<charT, Chars...> operator"" _cs();

#pragma endregion

#pragma region Some helper types

template <size_t N, typename charT, charT HeadChar, charT... TailChars>
struct literal_string_indexer
{
    static_assert(N < sizeof...(TailChars) + 1, "Index is too big");
    using type = typename literal_string_indexer<N - 1, charT, TailChars...>::type;
};

template <typename charT, charT HeadChar, charT... TailChars>
struct literal_string_indexer<0, charT, HeadChar, TailChars...>
{
    using type = literal_string<charT, HeadChar, TailChars...>;
};

/* TODO: literal_string_reverser - Might need name-able parameter packs... */

#pragma endregion


template <typename charT, char HeadChar, char... TailChars>
class literal_string<charT, HeadChar, TailChars...>
{
public:
    static constexpr const charT _data[] = { HeadChar, TailChars... };

#pragma region Friends and constructors (broken)

/*
friend constexpr literal_string<charT, HeadChar, TailChars...> operator"" <charT, HeadChar, TailChars...> _cs();

private:
    literal_string() = default;
*/
#pragma endregion

public:
    #pragma region Type aliases & static values

    using value_type = charT;
    using size_type = std::size_t;
    using difference_type = std::size_t;
    // iterator types???

    using tail_type = literal_string<charT, TailChars...>;
    //using reverse_type = literal_string_reverser<charT, HeadChar, TailChars...>::type

    //const static charT head_value = HeadChar;
    const static size_type size_value = sizeof...(TailChars) + 1;

    #pragma endregion

    #pragma region Size-related functions

    constexpr size_type size() const
    {
        return size_value;
    }

    constexpr size_type length() const
    {
        return size_value;
    }

    constexpr bool empty() const
    {
        return false;
    }

    #pragma endregion

    #pragma region Accessors

    constexpr charT operator[] (size_t pos) const
    {
        return _data[pos];
    }

    constexpr charT at(size_t pos) const
    {
        return _data[pos];
    }

    constexpr charT back() const
    {
        return _data[size_value - 1];
    }

    constexpr charT front() const
    {
        return _data[0];
    }

    #pragma endregion

    #pragma region Searching

    template <charT... LookForChars>
    constexpr size_type find(const literal_string<charT, LookForChars...>& str, size_type pos = 0) const
    {
        //static_assert(sizeof...(Chars) > 0, "find() is not valid for an empty string");
        //static_assert(sizeof...(LookForChars) > 0, "The parameter to find() cannot be an empty string");

        for (size_type i = 0; i + pos < size(); ++i)
        {
            bool wasMatched = true;

            for (size_type j = 0; i + pos + j < size() && j < str.size(); ++j)
            {
                if (_data[i + pos + j] != str._data[j])
                {
                    wasMatched = false;
                    break;
                }
            }

            if (wasMatched)
                return i + pos;
        }

        return std::string::npos; // TODO: Should I have my own npos?
    }

    constexpr size_type find(const charT* s, size_type pos, size_type count) const;

    constexpr size_type find(const charT* s, size_type pos = 0) const;

    constexpr size_type find(charT ch, size_t pos = 0) const
    {
        for (size_type i = 0; i < size(); ++i)
        {
            if (_data[i + pos] == ch)
                return i + pos;
        }

        return std::string::npos;
    }

    template <charT... LookForChars>
    constexpr size_type rfind(const literal_string<charT, LookForChars...>& str, size_type pos = size_value) const
    {
        //static_assert(sizeof...(Chars) > 0, "find() is not valid for an empty string");
        //static_assert(sizeof...(LookForChars) > 0, "The parameter to find() cannot be an empty string");

        for (/* empty init */; pos > 0; --pos)
        {
            for (size_type j = 0; pos + j < size() && j < str.size(); ++j)
            {
                // TODO: Do I need to subtract one from 'pos' to get the right index here...

                if (_data[pos + j] != str._data[j])
                    break;

                if (j == str.size() - 1)
                    return pos;
            }
        }

        return std::string::npos;
    }

    constexpr size_type rfind(const charT* s, size_type pos, size_type count) const;

    constexpr size_type rfind(const charT* s, size_type pos = size_value) const;

    constexpr size_type rfind(charT ch, size_t pos = size_value - 1) const
    {
        for (/* empty init */; pos > 0; --pos)
        {
            if (_data[pos - 1] == ch)
                return pos - 1;
        }

        return std::string::npos;
    }

    template <charT... LookForChars>
    constexpr size_type find_first_of(const literal_string<charT, LookForChars...>& str, size_type pos = 0) const;
    constexpr size_type find_first_of(const charT* s, size_type pos, size_type count) const;
    constexpr size_type find_first_of(const charT* s, size_type pos = 0) const;
    constexpr size_type find_first_of(charT ch, size_type pos = 0) const;

    template <charT... LookForChars>
    constexpr size_type find_first_not_of(const literal_string<charT, LookForChars...>& str, size_type pos = 0) const;
    constexpr size_type find_first_not_of(const charT* s, size_type pos, size_type count) const;
    constexpr size_type find_first_not_of(const charT* s, size_type pos = 0) const;
    constexpr size_type find_first_not_of(charT ch, size_type pos = 0) const;

    template <charT... LookForChars>
    constexpr size_type find_last_of(const literal_string<charT, LookForChars...>& str, size_type pos = size_value) const;
    constexpr size_type find_last_of(const charT* s, size_type pos, size_type count) const;
    constexpr size_type find_last_of(const charT* s, size_type pos = size_value) const;
    constexpr size_type find_last_of(charT ch, size_t pos = size_value) const;

    template <charT... LookForChars>
    constexpr size_type find_last_not_of(const literal_string<charT, LookForChars...>& str, size_type pos = size_value) const;
    constexpr size_type find_last_not_of(const charT* s, size_type pos, size_type count) const;
    constexpr size_type find_last_not_of(const charT* s, size_type pos = size_value) const;
    constexpr size_type find_last_not_of(charT ch, size_t pos = size_value) const;

#pragma endregion

    #pragma region Substrings

    template <size_type Pos>
    constexpr auto substr() const
        -> typename literal_string_indexer<Pos, charT, HeadChar, TailChars...>::type
    {
        return typename literal_string_indexer<Pos, charT, HeadChar, TailChars...>::type();
    }

    constexpr tail_type cdr () const
    {
        return tail_type();
    }

    #pragma endregion

    #pragma region Comparison

    template <charT... OtherChars>
    constexpr int compare(const literal_string<charT, OtherChars...>& other) const
    {
        for (size_type i = 0; i < size() && i < other.size(); ++i)
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

    constexpr int compare(const basic_string<charT>& str) const;
    constexpr int compare(std::size_t pos1, std::size_t count1, const basic_string<charT>& str) const;
    constexpr int compare(std::size_t pos1, std::size_t count1, const basic_string<charT>& str,
        std::size_t pos2, std::size_t count2) const;
    constexpr int compare(const charT* s) const;
    constexpr int compare(std::size_t pos1, std::size_t count1, const charT* s) const;
    constexpr int compare(std::size_t pos1, std::size_t count1,
        const charT* s, std::size_t count2) const;

    #pragma endregion

    #pragma region Conversions

    constexpr const char * c_str () const
    {
        return _data;
    }

    constexpr const char * data () const
    {
        return _data;
    }

    std::string to_string () const
    {
        return _data;
    }

    constexpr size_type to_number () const
    {
        static_assert((_data[0] >= '0' && _data[0] <= '9'), "Not a numeric string");

        if (size_value == 1)
            return _data[0] - '0';

        auto power = size_type{1};
        
        for (size_type i = 1; i < size_value; ++i)
            power *= 10;

        return ((_data[0] - '0') * power) + cdr().to_number();
    }

    constexpr operator const char* () const
    {
        return _data;
    }

    operator std::string () const
    {
        return _data;
    }

    constexpr operator size_type () const
    {
        return to_number();
    }

    #pragma endregion

};

template<typename charT>
class literal_string<charT>
{
public:
    static constexpr const charT _data[] = { };

#pragma region Friends and constructors (broken)
/*
template <typename charU>
friend constexpr literal_string<charU> operator "" _cs ();
private:
    literal_string() = default;
*/
#pragma endregion

public:
    #pragma region Type aliases

    using value_type = charT;
    using size_type = std::size_t;
    using difference_type = std::size_t;
    // iterator types???

    using tail_type = literal_string<charT>;
    //using reverse_type = literal_string<charT>;

    #pragma endregion

    #pragma region Size-related functions

    constexpr size_type size() const
    {
        return 0;
    }

    constexpr size_type length() const
    {
        return 0;
    }

    constexpr bool empty() const
    {
        return true;
    }

    #pragma endregion

    // There are no accessors for empty strings

    // There are no search functions for empty strings

    // There are no substring functions for empty strings

    #pragma region Comparison

    #pragma endregion

    #pragma region Conversions

    constexpr const char * c_str() const
    {
        return _data;
    }

    constexpr const char * data() const
    {
        return _data;
    }

    std::string to_string() const
    {
        return _data;
    }

    // TODO: Make this only accessible to other literal_strings
    constexpr size_type to_number() const
    {
        return 0;
    }

    constexpr operator const char* () const
    {
        return _data;
    }

    operator std::string() const
    {
        return _data;
    }

    #pragma endregion

};

#pragma region Friend functions

#pragma region User-defined literal

template <typename charT, charT... Chars>
constexpr literal_string<charT, Chars...> operator"" _cs()
{
    return literal_string<charT, Chars...>();
}

#pragma endregion

#pragma region Concatenation

template <typename charT, charT... LeftChars, charT... RightChars>
constexpr auto operator+ (const literal_string<charT, LeftChars...> &l,
                          const literal_string<charT, RightChars...> &r)
    -> literal_string<charT, LeftChars..., RightChars...>
{
    return literal_string<charT, LeftChars..., RightChars...>();
}

#pragma endregion

#pragma region Boolean operators

template <typename charT, charT... LeftChars, charT... RightChars>
constexpr bool operator== (const literal_string<charT, LeftChars...>& lhs,
                           const literal_string<charT, RightChars...>& rhs)
{
    return (lhs.compare(rhs) == 0);
}

template <typename charT, charT... LeftChars, charT... RightChars>
constexpr bool operator!= (const literal_string<charT, LeftChars...>& lhs,
                           const literal_string<charT, RightChars...>& rhs)
{
    return (lhs.compare(rhs) != 0);
}

template <typename charT, charT... LeftChars, charT... RightChars>
constexpr bool operator< (const literal_string<charT, LeftChars...>& lhs,
                          const literal_string<charT, RightChars...>& rhs)
{
    return (lhs.compare(rhs) < 0);
}

template <typename charT, charT... LeftChars, charT... RightChars>
constexpr bool operator<= (const literal_string<charT, LeftChars...>& lhs,
                           const literal_string<charT, RightChars...>& rhs)
{
    return (lhs.compare(rhs) <= 0);
}

template <typename charT, charT... LeftChars, charT... RightChars>
constexpr bool operator> (const literal_string<charT, LeftChars...>& lhs,
                          const literal_string<charT, RightChars...>& rhs)
{
    return (lhs.compare(rhs) > 0);
}

template <typename charT, charT... LeftChars, charT... RightChars>
constexpr bool operator>= (const literal_string<charT, LeftChars...>& lhs,
                           const literal_string<charT, RightChars...>& rhs)
{
    return (lhs.compare(rhs) >= 0);
}


template <typename charT, charT... RightChars>
constexpr bool operator== (const charT* lhs, const literal_string<charT, RightChars...>& rhs);

template <typename charT, charT... LeftChars>
constexpr bool operator== (const literal_string<charT, LeftChars...>& lhs, const charT* rhs);

template <typename charT, charT... RightChars>
constexpr bool operator!= (const charT* lhs, const literal_string<charT, RightChars...>& rhs);

template <typename charT, charT... LeftChars>
constexpr bool operator!= (const literal_string<charT, LeftChars...>& lhs, const charT* rhs);

template <typename charT, charT... RightChars>
constexpr bool operator< (const charT* lhs, const literal_string<charT, RightChars...>& rhs);

template <typename charT, charT... LeftChars>
constexpr bool operator< (const literal_string<charT, LeftChars...>& lhs, const charT* rhs);

template <typename charT, charT... RightChars>
constexpr bool operator<= (const charT* lhs, const literal_string<charT, RightChars...>& rhs);

template <typename charT, charT... LeftChars>
constexpr bool operator<= (const literal_string<charT, LeftChars...>& lhs, const charT* rhs);

template <typename charT, charT... RightChars>
constexpr bool operator> (const charT* lhs, const literal_string<charT, RightChars...>& rhs);

template <typename charT, charT... LeftChars>
constexpr bool operator> (const literal_string<charT, LeftChars...>& lhs, const charT* rhs);

template <typename charT, charT... RightChars>
constexpr bool operator>= (const charT* lhs, const literal_string<charT, RightChars...>& rhs);

template <typename charT, charT... LeftChars>
constexpr bool operator>= (const literal_string<charT, LeftChars...>& lhs, const charT* rhs);

// TODO: Perhaps basic_string overloads as well?

#pragma endregion

#pragma region Stream operators

template <typename charT, charT... Chars>
inline std::ostream& operator<< (std::ostream& os, const literal_string<charT, Chars...>& str)
{
    return (os << str.c_str());
}

#pragma endregion

#pragma endregion

} // namespace std


#pragma region Test helpers

using std::operator"" _cs;

void testSizes()
{
    static_assert("He"_cs.size() == 2, "size() is wrong");
    static_assert(L"Wo"_cs.size() == 2, "size() is wrong");
    static_assert(u"Hello!"_cs.size() == 6, "size() is wrong");
    static_assert(U"World!"_cs.size() == 6, "size() is wrong");
}

void testEmptyStrings()
{
    constexpr auto mb = ""_cs;
    constexpr auto wide = L""_cs;
    constexpr auto utf16 = u""_cs;
    constexpr auto utf32 = U""_cs;

    static_assert(mb.size() == 0, "Multi-byte size() is wrong");
    static_assert(wide.size() == 0, "Wide size() is wrong");
    static_assert(utf16.size() == 0, "UTF-16 size() is wrong");
    static_assert(utf32.size() == 0, "UTF-32 size() is wrong");

    static_assert(mb.length() == 0, "Multi-byte length() is wrong");
    static_assert(wide.length() == 0, "Wide length() is wrong");
    static_assert(utf16.length() == 0, "UTF-16 length() is wrong");
    static_assert(utf32.length() == 0, "UTF-32 length() is wrong");

    static_assert(mb.empty() == true, "Multi-byte empty() is wrong");
    static_assert(wide.empty() == true, "Wide empty() is wrong");
    static_assert(utf16.empty() == true, "UTF-16 empty() is wrong");
    static_assert(utf32.empty() == true, "UTF-32 empty() is wrong");
}

void testSearching()
{
    constexpr auto hi = "Hello "_cs + "World!"_cs;

    static_assert(hi.find('?') == std::string::npos, "find() is wrong");
    static_assert(hi.find(' ') == 5, "find() is wrong");
    static_assert(hi.find('o') == 4, "find() is wrong");
    static_assert(hi.find("orl"_cs) == 7, "find() is wrong");
    static_assert(hi.rfind('o') == 7, "rfind() is wrong");
    static_assert(hi.rfind("el"_cs) == 1, "rfind() is wrong");
}

void testSubstringFunctions()
{
    constexpr auto big = "Hello, World!"_cs;
    constexpr auto smaller = big.substr<7>();
    constexpr auto lessbig = big.cdr();

    static_assert(smaller.size() == 6, "substr<>() is wrong");
    static_assert(smaller[0UL] == 'W', "substr<>() is wrong");
    static_assert(big.substr<0>().at(0) == 'H', "substr<>() is wrong");
    static_assert(big.substr<12>().at(0) == '!', "substr<>() is wrong");
    static_assert(big.size() == lessbig.size() + 1, "cdr() is wrong");

}

void testComparisons()
{
    constexpr auto first = "One"_cs;
    constexpr auto second = "Two"_cs;
    constexpr auto last = "One"_cs;

    static_assert(first.compare(last) == 0, "compare() is wrong");
    static_assert(first.compare(second) == -1, "compare() is wrong");
    static_assert(second.compare(last) == 1, "compare() is wrong");

    static_assert("A"_cs == "A"_cs, "operator== is wrong");
    static_assert("A"_cs != "a"_cs, "operator!= is wrong");
    static_assert("a"_cs > "A"_cs, "operator> is wrong");
    static_assert("1"_cs < "2"_cs, "operator< is wrong");
}

void testConversions()
{
    static_assert("0"_cs.to_number() == 0, "to_number() is wrong");
    static_assert("1"_cs.to_number() == 1, "to_number() is wrong");
    static_assert("11"_cs.to_number() == 11, "to_number() is wrong");
    static_assert("65535"_cs.to_number() == 65535, "to_number() is wrong");
    //"99kj343"_cs.to_number();

    constexpr size_t number = "12345"_cs;
    static_assert(number == 12345, "operator size_type is wrong");
}

void testConcatenation()
{
    constexpr auto hello = "Hello"_cs;
    constexpr auto world = "World"_cs;

    constexpr auto together = hello + ", "_cs + world + "!"_cs;
    static_assert(together.size() == 13, "operator+ is wrong");
    static_assert(together[5UL] == ',', "operator+ is wrong");
}

#pragma region Class templates

template <typename Str>
struct star_wars_speaker
{
    star_wars_speaker() { std::cout << "Someone in Star Wars" << std::endl; }
};

template <>
struct star_wars_speaker<decltype("I'd just as soon kiss a Wookiee"_cs)>
{
    star_wars_speaker() { std::cout << "Carrie Fisher" << std::endl; }
};

template <>
struct star_wars_speaker<decltype("Luke, I am your father"_cs)>
{
    star_wars_speaker() { std::cout << "James Earl Jones" << std::endl; }
};

template <>
struct star_wars_speaker<decltype("Midi-chlorians"_cs)>
{
    star_wars_speaker() { std::cout << "Liam Neeson" << std::endl; }
};

void testClassTemplates()
{
    auto one = star_wars_speaker<decltype("I'd just as soon kiss a Wookiee"_cs)>();
    auto two = star_wars_speaker<decltype("Luke, I am your father"_cs)>();
    auto thr = star_wars_speaker<decltype("Midi-chlorian"_cs)>();
    auto fur = star_wars_speaker<decltype("How wude!"_cs)>();
}

#pragma endregion

#pragma region Function templates

template <typename Str>
void star_trek_speak(const Str &)
{
    std::cout << "Someone in Star Trek" << std::endl;
}

template <>
void star_trek_speak(const decltype("Illogical captain"_cs) &)
{
    std::cout << "Leonard Nimoy" << std::endl;
}

template <>
void star_trek_speak(const decltype("He's dead Jim"_cs) &)
{
    std::cout << "DeForest Kelley" << std::endl;
}

template <>
void star_trek_speak(const decltype("Oh my"_cs) &)
{
    std::cout << "George Takei" << std::endl;
}

void testFunctionTemplates()
{
    star_trek_speak("Illogical captain"_cs);
    star_trek_speak("He's dead Jim"_cs);
    star_trek_speak("Oh my"_cs);
    star_trek_speak("Khaaaaaaaaaaannnnnn!"_cs);
}

#pragma endregion

#pragma endregion Test helpers

int main() 
{
    testSizes();
    testEmptyStrings();
    testSubstringFunctions();
    testComparisons();
    testConversions();
    testConcatenation();
    testClassTemplates();
    testFunctionTemplates();

    return 0;
}

