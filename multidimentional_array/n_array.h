#include <algorithm>
#include <array>
#include <initializer_list>
#include <iterator>




template <size_t Head, size_t ... Tail>
struct Multiplied
{
    constexpr static size_t value = Head * Multiplied<Tail...>::value;
};

template <size_t Value>
struct Multiplied<Value>
{
    constexpr static size_t value = Value;
};

/*

template <typename T, size_t Size, size_t ... Dimensions>
class n_array : private std::array<n_array<T, Dimensions...>, Size>
{
 using nested_type = n_array<T, Dimensions...>;
 using base = std::array<nested_type, Size>;

 public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using base::base;
    using base::operator[];
};

template <typename T, size_t Size>
class n_array<T, Size> : public std::array<T, Size>
{
 using nested_type = T;
 using base = std::array<nested_type, Size>;

 public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using base::base;
    using base::operator[];
};

*/

template <typename T, size_t Size, size_t ... Dimensions>
class n_array 
{
 using nested_type = n_array<T, Dimensions...>;
 using base = std::array<nested_type, Size>;

 public:
    std::array<n_array<T, Dimensions...>, Size> m_base;

    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    nested_type & operator[] (size_t index) { return m_base[index]; }

    iterator begin () { return m_base.begin()->begin(); } 
    const_iterator cbegin () { return m_base.begin()->begin(); } 
    iterator end () { return m_base.begin()->begin() + size(); } 
    const_iterator cend () { return m_base.begin()->begin() + size(); } 

    reference front () { return *begin(); }
    const_reference front () const { return *begin(); }
    reference back () { return *(end() - 1); }
    const_reference back () const { return *(end() - 1); }

    constexpr size_t size () { return Multiplied<Size, Dimensions...>::value; }
    constexpr bool empty () { return size() == 0; }
};

template <typename T, size_t Size>
class n_array<T, Size>
{
 using nested_type = T;
 using base = std::array<nested_type, Size>;

 public:
    std::array<nested_type, Size> m_base;

    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    T & operator[] (size_t index) { return m_base[index]; }

    iterator begin () { return m_base.begin(); };
    const_iterator cbegin () { return m_base.begin(); } 
    iterator end () { return m_base.begin() + size(); } 
    const_iterator cend () { return m_base.begin() + size(); } 

    reference front () { return *begin(); }
    const_reference front () const { return *begin(); }
    reference back () { return *(end() - 1); }
    const_reference back () const { return *(end() - 1); }

    constexpr size_t size () { return Size; }
    constexpr bool empty () { return size() == 0; }
};

