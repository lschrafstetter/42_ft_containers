//////////////////////////////////////////////////////////////////////////////
// Contains the utilities for all ft::containers:
// ft::iterator_traits
// ft::reverse_iterator
// ft::enable_if
// ft::is_integral
// ft::equal
// ft::lexicographical_compare
// ft::pair
// ft::make_pair
// other helpers:
// ft::is_same
//////////////////////////////////////////////////////////////////////////////

#ifndef UTILITIES_H
#define UTILITIES_H

namespace ft {

//////////////////////////////////////////////////////////////////////////////
// ft::iterator_traits
//////////////////////////////////////////////////////////////////////////////

template <typename Iterator>
struct iterator_traits {
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
  typedef typename Iterator::iterator_category iterator_category;
};

// Pointer specialization
template <typename T>
struct iterator_traits<T*> {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef std::random_access_iterator_tag iterator_category;
};

// Const pointer specialization
template <typename T>
struct iterator_traits<const T*> {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef std::random_access_iterator_tag iterator_category;
};

//////////////////////////////////////////////////////////////////////////////
// ft::reverse_iterator
//////////////////////////////////////////////////////////////////////////////

template <class Iterator>
class reverse_iterator {
  typedef Iterator iterator_type;

  typedef typename std::iterator_traits<iterator_type>::iterator_category
      iterator_category;
  typedef typename std::iterator_traits<iterator_type>::value_type value_type;
  typedef typename std::iterator_traits<iterator_type>::difference_type
      difference_type;
  typedef typename std::iterator_traits<iterator_type>::pointer pointer;
  typedef typename std::iterator_traits<iterator_type>::reference reference;

 public:
  //**************************************************
  // Constructors
  //**************************************************

  reverse_iterator() : current(iterator_type()){};
  explicit reverse_iterator(iterator_type x) : current(iterator_type(x)){};
  template <class U>
  reverse_iterator(const reverse_iterator<U>& other) : current(other.current) {}

  //**************************************************
  // Operator overloads
  //**************************************************

  // Assignment operator
  template <class U>
  reverse_iterator& operator=(const reverse_iterator<U>& other) {
    this->current = other.current;
    return *this;
  }

  // Access operators
  reference operator*() const { return current[-1]; };

  pointer operator->() const { return &(operator*()); };

  reference operator[](difference_type n) const { return current[-n - 1]; };

  // Arithmetic operators
  reverse_iterator& operator++() {
    --current;
    return *this;
  };

  reverse_iterator& operator--() {
    ++current;
    return *this;
  };

  reverse_iterator operator++(int) {
    reverse_iterator<iterator_type> tmp(current);
    --current;
    return tmp;
  };

  reverse_iterator operator--(int) {
    reverse_iterator<iterator_type> tmp(current);
    ++current;
    return tmp;
  };

  reverse_iterator operator+(difference_type n) const {
    return this->operator[](n);
  };

  reverse_iterator operator-(difference_type n) const {
    return this->operator[](-n);
  };

  reverse_iterator& operator+=(difference_type n) {
    current -= n;
    return *this;
  };

  reverse_iterator& operator-=(difference_type n) {
    current += n;
    return *this;
  };

  //**************************************************
  // Functions
  //**************************************************

  iterator_type base() { return current; };

 protected:
  iterator_type current;
};

//////////////////////////////////////////////////////////////////////////////
// ft::enable_if
//////////////////////////////////////////////////////////////////////////////

// https://eli.thegreenplace.net/2014/sfinae-and-enable_if/
template <bool, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
  typedef T type;
};

//////////////////////////////////////////////////////////////////////////////
// ft::is_integral
//////////////////////////////////////////////////////////////////////////////

// https://cplusplus.com/reference/type_traits/is_integral/?kw=is_integral
template <class T, T v>
struct integral_constant {
  static const T value = v;
  typedef T value_type;
  typedef integral_constant<T, v> type;
};

template <class T, T v>
const T integral_constant<T, v>::value;

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <class T>
struct is_integral : public integral_constant<bool, false> {};

template <>
struct is_integral<bool> : public true_type {};
template <>
struct is_integral<char> : public true_type {};
template <>
struct is_integral<unsigned char> : public true_type {};
template <>
struct is_integral<short> : public true_type {};
template <>
struct is_integral<unsigned short> : public true_type {};
template <>
struct is_integral<int> : public true_type {};
template <>
struct is_integral<unsigned int> : public true_type {};
template <>
struct is_integral<long> : public true_type {};
template <>
struct is_integral<unsigned long> : public true_type {};
// C++98 doesn't support long long types
// template <>
// struct is_integral<long long> : public true_type {};
// template <>
// struct is_integral<unsigned long long> : public true_type {};

//////////////////////////////////////////////////////////////////////////////
// ft::equal
//////////////////////////////////////////////////////////////////////////////

// https://en.cppreference.com/w/cpp/algorithm/equal
template <class InputIt1, class InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
  while (first1 != last1) {
    if (!(*(first1++) == *(first2++))) {
      return false;
    }
  }
  return true;
}

// https://en.cppreference.com/w/cpp/algorithm/equal
// BinaryPredicate: bool pred(const Type1 &a, const Type2 &b);
template <class InputIt1, class InputIt2, class BinaryPredicate>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2,
           BinaryPredicate p) {
  while (first1 != last1) {
    if (!p(*(first1++), *(first2++))) {
      return false;
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
// ft::lexicographical_compare
//////////////////////////////////////////////////////////////////////////////

// returns true if the first range is lexicographically less, otherwise false
template <class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2) {
  while ((first1 != last1) && (first2 != last2)) {
    if (*first1 < *first2) return true;
    if (*first2 < *first1) return false;
    ++first1;
    ++first2;
  }
  // Checks if one or both ranges go on.
  // Both end: return false (first is not smaller, because they are equal)
  // First ends, second goes on: return true (first is smaller)
  // First goes on, second ends: return false (second is smaller)
  return (first1 == last1) && (first2 != last2);
}

// https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
// returns true if the first range is lexicographically less, otherwise false
// Compare: bool cmp(const Type1 &a, const Type2 &b);
template <class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2, Compare comp) {
  while ((first1 != last1) && (first2 != last2)) {
    if (comp(*first1, *first2)) return true;
    if (comp(*first2, *first1)) return false;
    ++first1;
    ++first2;
  }
  // Checks if one or both ranges go on.
  // Both end: return false (first is not smaller, because they are equal)
  // First ends, second goes on: return true (first is smaller)
  // First goes on, second ends: return false (second is smaller)
  return (first1 == last1) && (first2 != last2);
}

//////////////////////////////////////////////////////////////////////////////
// ft::pair
//////////////////////////////////////////////////////////////////////////////

template <class T1, class T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

 public:
  //**************************************************
  // Constructors
  //**************************************************

  pair(){};
  pair(const first_type& x, const second_type& y) : first(x), second(y){};
  pair(const pair& p) : first(p.first), second(p.second){};

  //**************************************************
  // Operator overloads
  //**************************************************

  // Assignment operator
  pair& operator=(const pair& other) {
    this->first = other.first;
    this->second = other.second;
    return *this;
  };

  //**************************************************
  // Member objects
  //**************************************************

  first_type first;
  second_type second;
};

//**************************************************
// Non-member operator overloads pertaining ft::pair
//**************************************************

template <class T1, class T2>
bool operator==(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
  return (lhs.first == rhs.first) && (lhs.second == rhs.second);
}

template <class T1, class T2>
bool operator!=(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
  return !(lhs == rhs);
}

template <class T1, class T2>
bool operator<(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
  // Only if the member "first" of the first pair is not smaller, the second
  // gets compared
  return lhs.first < rhs.first ||
         (!(rhs.first < lhs.first) && lhs.second < rhs.second);
}

template <class T1, class T2>
bool operator<=(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
  // lhs and rhs are "switched" so the already defined "<" can be used
  return !(rhs < lhs);
}

template <class T1, class T2>
bool operator>(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
  // lhs and rhs are "switched" so the already defined "<" can be used
  return rhs < lhs;
}

template <class T1, class T2>
bool operator>=(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
  return !(lhs < rhs);
}

//////////////////////////////////////////////////////////////////////////////
// ft::make_pair
//////////////////////////////////////////////////////////////////////////////

template <class T1, class T2>
ft::pair<T1, T2> make_pair(T1 t, T2 u) {
  return ft::pair<T1, T2>(t, u);
}

//////////////////////////////////////////////////////////////////////////////
// ft::is_same
//////////////////////////////////////////////////////////////////////////////

// https://en.cppreference.com/w/cpp/types/is_same
template <typename T, typename U>
struct is_same : public false_type {};

template <typename T>
struct is_same<T, T> : public true_type {};

//////////////////////////////////////////////////////////////////////////////
// ft::is_const
//////////////////////////////////////////////////////////////////////////////

template <class T>
struct is_const : false_type {};

template <class T>
struct is_const<const T> : true_type {};

}  // namespace ft

#endif  // UTILITIES_H