#include "include.hpp"

template <class T>
static void print_set(NAMESPACE::set<T> &set) {
  if (set.empty())
    return;
  size_t hash = 0;
  typename NAMESPACE::set<T>::iterator start = set.begin();
  typename NAMESPACE::set<T>::iterator end = set.end();

  while (start != end) {
    hash += (int16_t)(*start);
    hash *= 13;
    hash %= 65536;
    start++;
  }

  std::cout << "Size: " << set.size() << ", Hash: " << hash << std::endl;
}

void test_set() {
  std::cout << GREEN << "SET TESTS:" << std::endl;

  //**************************************************
  // Constructors
  //**************************************************
  std::cout << "Normal constructor:" << std::endl;
  NAMESPACE::set<int> set1;
  for (int i = 0; i < 1000; ++i) {
    int tmp = rand();
    set1.insert(tmp);
  };
  print_set(set1);

  std::cout << "Range constructor:" << std::endl;
  NAMESPACE::set<int>::iterator it = set1.begin();
  std::advance(it, 500);
  NAMESPACE::set<int> set2(set1.begin(), it);
  print_set(set2);

  std::cout << "Copy constructor:" << std::endl;
  NAMESPACE::set<int> set3(set2);
  print_set(set3);

  std::cout << "Copy assignment operator:" << std::endl;
  set3 = set1;
  print_set(set3);

  // **************************************************
  // Iterators
  // **************************************************

  std::cout << "set::begin()" << std::endl;
  it = set3.begin();
  std::cout << *it << std::endl;

  std::advance(it, 300);
  std::cout << *it << std::endl;
  std::cout << *--it << std::endl;
  std::cout << *++it << std::endl;
  std::cout << *it++ << std::endl;
  std::cout << *it-- << std::endl;
  std::cout << *it << std::endl;

  NAMESPACE::set<int>::reverse_iterator rit = set3.rbegin();
  std::advance(rit, 5);
  std::cout << *rit << std::endl;
  std::cout << *++rit << std::endl;
  std::cout << *--rit << std::endl;
  std::cout << *rit++ << std::endl;
  std::cout << *rit-- << std::endl;
  std::cout << *rit << std::endl;

  rit = set3.rend();
  rit--;
  std::cout << *rit << std::endl;

  // **************************************************
  // Modifiers
  // **************************************************

  NAMESPACE::set<int> set4;
  for (int i = 0; i < 5; ++i) {
    int tmp = rand();
    set4.insert(tmp);
  }
  print_set(set4);
  set4.erase(*(++set4.begin()));
  print_set(set4);
  set4.erase(set4.begin());
  print_set(set4);
  set4.erase(set4.begin(), set4.end());
  print_set(set4);

  // **************************************************
  // Performance
  // **************************************************
  
  NAMESPACE::set<int> set5;
  for (int i = 0; i < 1000000; ++i) {
    int tmp = rand();
    set5.insert(tmp);
  }

  print_set(set5);

  set5.erase(set5.begin(), set5.end());
}

int main(void) {
  srand(2);  // Set the seed
  test_set();
}