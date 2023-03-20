#include "include.hpp"

template <class T, class U>
static void print_stack(NAMESPACE::stack<T, U> stack) {
  if (stack.empty())
    return;
  size_t hash = 0;
  size_t size = 0;

  while (!stack.empty()) {
    hash += (int16_t)stack.top();
    hash *= 13;
    hash %= 65536;
    stack.pop();
    
    ++size;
  }

  std::cout << "Size: " << stack.size() << ", Hash: " << hash << std::endl;
}

void test_stack() {
  std::cout << CYAN << "SET TESTS:" << std::endl;

  //**************************************************
  // Constructors
  //**************************************************

  NAMESPACE::stack<int, NAMESPACE::vector<int> > stack1;
  for (int i = 0; i < 1000; ++i) {
    int tmp = rand();
    stack1.push(tmp);
  }

  NAMESPACE::stack<int, NAMESPACE::vector<int> > stack2(stack1);

  NAMESPACE::stack<int, NAMESPACE::vector<int> > stack3;
  for (int i = 0; i < 50; ++i) {
    int tmp = rand();
    stack3.push(tmp);
  }
  stack3 = stack1;
  print_stack(stack1);
  print_stack(stack2);
  print_stack(stack3);

  //**************************************************
  // Performance
  //**************************************************

  NAMESPACE::stack<int, NAMESPACE::vector<int> > stack4;
  for (int i = 0; i < 100000000; ++i) {
    int tmp = rand();
    stack1.push(tmp);
  }

  print_stack(stack4);
  
  while (!stack4.empty())
    stack4.pop();
}

int main(void) {
  srand(2);  // Set the seed
  test_stack();
}