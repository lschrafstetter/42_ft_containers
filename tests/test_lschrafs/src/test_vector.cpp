#include "include.hpp"

template <class T>
static void print_vector(NAMESPACE::vector<T> &vec) {
  size_t hash = 0;
  size_t size = vec.size();
  for (size_t i = 0; i < size; ++i) {
    hash += (int16_t)vec[i];
    hash *= 13;
    hash %= 65536;
  }
  std::cout << "Size: " << vec.size() << ", Hash: " << hash << std::endl;
}

void test_vector() {
  std::cout << BLUE << "VECTOR TESTS:" << std::endl;

  //**************************************************
  // Constructors
  //**************************************************

  std::cout << "Normal constructor:" << std::endl;
  NAMESPACE::vector<int> vec1;
  for (int i = 0; i < 100; i++) vec1.push_back(rand());
  print_vector(vec1);

  std::cout << "Copy constructor:" << std::endl;
  NAMESPACE::vector<int> vec2(vec1);
  print_vector(vec2);

  std::cout << "Copy assignment:" << std::endl;
  vec2.push_back(42);
  vec1 = vec2;
  print_vector(vec1);

  std::cout << "vector::assign(): once assigning 50 (with Iterator), then 42 elements (with count)" << std::endl;
  NAMESPACE::vector<int> vec3(500, 666);
  vec3.assign(vec2.begin(), vec2.begin() + 50);
  print_vector(vec3);
  vec3.assign(42, 69);
  print_vector(vec3);

  //**************************************************
  // Element access
  //**************************************************

  std::cout << "vector::at()" << std::endl;
  std::cout << vec1.at(21) << std::endl;

  std::cout << "vector::operator[]" << std::endl;
  std::cout << vec1[21] << std::endl;

  std::cout << "vector::front()" << std::endl;
  std::cout << vec1.front() << std::endl;

  std::cout << "vector::back()" << std::endl;
  std::cout << vec1.back() << std::endl;

  //**************************************************
  // Iterators
  //**************************************************

  std::cout << "vector::begin()" << std::endl;
  NAMESPACE::vector<int> vec4(100);
  for (int i = 0; i < 100; ++i) vec4[i] = i;
  NAMESPACE::vector<int>::iterator it = vec4.begin();
  std::cout << *it << std::endl;
  it += 2;
  std::cout << *(it++) << std::endl;
  std::cout << *(++it) << std::endl;
  it -= 2;
  std::cout << *(it--) << std::endl;
  std::cout << *(--it) << std::endl;

  std::cout << "vector::end()" << std::endl;
  it = vec4.end();
  --it;
  std::cout << *(it) << std::endl;

  NAMESPACE::vector<int>::reverse_iterator rit = vec4.rbegin();
  std::advance(rit, 5);
  std::cout << *rit << std::endl;
  std::cout << *++rit << std::endl;
  std::cout << *--rit << std::endl;
  std::cout << *rit++ << std::endl;
  std::cout << *rit-- << std::endl;
  std::cout << *rit << std::endl;

  rit = vec4.rend();
  std::cout << *rit << std::endl;

  //**************************************************
  // Capacity + Modifiers
  //**************************************************
  std::cout << "vector::clear() + vector::empty()" << std::endl;
  vec4.clear();
  print_vector(vec4);
  std::cout << vec4.empty() << std::endl;

  std::cout << "vector::insert()" << std::endl;
  vec1.insert(vec1.begin() + 30, 42);
  vec1.insert(vec1.begin(), vec2.begin(), vec2.end());
  print_vector(vec1);

  std::cout << "vector::erase()" << std::endl;
  vec1.erase(vec1.begin() + 42);
  print_vector(vec1);
  vec1.erase(vec1.begin(), vec1.begin() + 20);
  print_vector(vec1);

  std::cout << "vector::resize()" << std::endl;
  vec1.resize(42);
  print_vector(vec1);

  std::cout << "vector::swap()" << std::endl;
  vec1.swap(vec2);
  print_vector(vec1);

  //**************************************************
  // Performance
  //**************************************************

  NAMESPACE::vector<int> vec5;
  for (int i = 0; i < 100000; ++i) {
    vec5.push_back(i);
    vec5.insert(vec5.begin(), i);
  }

  print_vector(vec5);

  while (!vec5.empty()) {
    vec5.pop_back();
  }
}

int main(void) {
  srand(2);  // Set the seed
  test_vector();
}