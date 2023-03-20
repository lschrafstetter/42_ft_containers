#include "include.hpp"

template <class T, class U>
static void print_map(NAMESPACE::map<T, U> &map) {
  if (map.empty())
    return;
  size_t hash = 0;
  typename NAMESPACE::map<T, U>::iterator start = map.begin();
  typename NAMESPACE::map<T, U>::iterator end = map.end();

  while (start != end) {
    hash += (int16_t)((*start).second);
    hash *= 13;
    hash %= 65536;
    start++;
  }

  std::cout << "Size: " << map.size() << ", Hash: " << hash << std::endl;
}

void test_map() {
  std::cout << YELLOW << "MAP TESTS:" << std::endl;

  //**************************************************
  // Constructors
  //**************************************************
  std::cout << "Normal constructor:" << std::endl;
  NAMESPACE::map<int, int> map1;
  for (int i = 0; i < 1000; ++i) {
    int tmp = rand();
    map1[tmp] = tmp;
  };
  print_map(map1);

  std::cout << "Range constructor:" << std::endl;
  NAMESPACE::map<int, int>::iterator it = map1.begin();
  std::advance(it, 500);
  NAMESPACE::map<int, int> map2(map1.begin(), it);
  print_map(map2);

  std::cout << "Copy constructor:" << std::endl;
  NAMESPACE::map<int, int> map3(map2);
  print_map(map3);

  std::cout << "Copy assignment operator:" << std::endl;
  map3 = map1;
  print_map(map3);
  
  //**************************************************
  // Element access
  //**************************************************

  int random_number = rand();
  std::cout << "map::operator[]" << std::endl;
  map3[random_number] = random_number;
  std::cout << map3[random_number] << std::endl;
  map3[random_number] = random_number + 1;
  std::cout << map3[random_number] << std::endl;

  std::cout << "map::at()" << std::endl;
  std::cout << map3.at(random_number) << std::endl;

  //**************************************************
  // Iterators
  //**************************************************

  std::cout << "map::begin()" << std::endl;
  it = map3.begin();
  std::cout << (*it).second << std::endl;

  std::advance(it, 300);
  std::cout << (*it).second << std::endl;
  std::cout << (*--it).second << std::endl;
  std::cout << (*++it).second << std::endl;
  std::cout << (*it++).second << std::endl;
  std::cout << (*it--).second << std::endl;
  std::cout << (*it).second << std::endl;

  NAMESPACE::map<int, int>::reverse_iterator rit = map3.rbegin();
  std::advance(rit, 5);
  std::cout << rit->second << std::endl;
  std::cout << ++rit->second << std::endl;
  std::cout << --rit->second << std::endl;
  std::cout << rit++->second << std::endl;
  std::cout << rit--->second << std::endl;
  std::cout << rit->second << std::endl;

  rit = map3.rend();
  rit--;
  std::cout << rit->second << std::endl;

  //**************************************************
  // Modifiers
  //**************************************************

  NAMESPACE::map<int, int> map4;
  for (int i = 0; i < 5; ++i) {
    int tmp = rand();
    map4.insert(NAMESPACE::make_pair(tmp, tmp));
  }
  print_map(map4);
  map4.erase(++(map4.begin())->second);
  print_map(map4);
  map4.erase(map4.begin());
  print_map(map4);
  map4.erase(map4.begin(), map4.end());
  print_map(map4);

  //**************************************************
  // Performance
  //**************************************************
  
  NAMESPACE::map<int, int> map5;
  for (int i = 0; i < 1000000; ++i) {
    int tmp = rand();
    map5.insert(NAMESPACE::make_pair(tmp, tmp));
  }

  print_map(map5);
  
  map5.erase(map5.begin(), map5.end());
}

int main(void) {
  srand(2);  // Set the seed
  test_map();
}