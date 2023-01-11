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
  reverse_iterator(const reverse_iterator<U>& other) : current(other.current){};

  //**************************************************
  // Operator overloads
  //**************************************************

  // Assignment operator
  template <class U>
  reverse_iterator& operator=(const reverse_iterator<U>& other) {
    this->current = other.current;
    return *this;
  };

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
    iterator_type tmp(current);
    --current;
    return tmp;
  };

  reverse_iterator operator--(int) {
    iterator_type tmp(current);
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
template <>
struct is_integral<long long> : public true_type {};
template <>
struct is_integral<unsigned long long> : public true_type {};

//////////////////////////////////////////////////////////////////////////////
// ft::equal
//////////////////////////////////////////////////////////////////////////////

// https://en.cppreference.com/w/cpp/algorithm/equal
template <class InputIt1, class InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
  for (; first1 != last1; ++first1, ++first2) {
    if (!(*first1 == *first2)) {
      return false;
    }
  }
  return true;
};

// https://en.cppreference.com/w/cpp/algorithm/equal
// BinaryPredicate: bool pred(const Type1 &a, const Type2 &b);
template <class InputIt1, class InputIt2, class BinaryPredicate>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2,
           BinaryPredicate p) {
  for (; first1 != last1; ++first1, ++first2) {
    if (!p(*first1, *first2)) {
      return false;
    }
  }
  return true;
};

//////////////////////////////////////////////////////////////////////////////
// ft::lexicographical_compare
//////////////////////////////////////////////////////////////////////////////

// https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
// returns true if the first range is lexicographically less, otherwise false
// (void) in front of ++first2 to not generate a "unused variable" warning
template <class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2) {
  for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2) {
    if (*first1 < *first2) return true;
    if (*first2 < *first1) return false;
  }

  return (first1 == last1) && (first2 != last2);
};

// https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
// returns true if the first range is lexicographically less, otherwise false
// Compare: bool cmp(const Type1 &a, const Type2 &b);
// (void) in front of ++first2 to not generate a "unused variable" warning
template <class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2, Compare comp) {
  for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2) {
    if (comp(*first1, *first2)) return true;
    if (comp(*first2, *first1)) return false;
  }

  return (first1 == last1) && (first2 != last2);
};

//////////////////////////////////////////////////////////////////////////////
// ft::pair
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// ft::make_pair
//////////////////////////////////////////////////////////////////////////////

}  // namespace ft

#endif  // UTILITIES_H