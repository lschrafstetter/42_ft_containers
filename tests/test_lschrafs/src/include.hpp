#pragma once

#if TESTSTD  // if set, use standard template library

#define NAMESPACE std
#include <map>
#include <set>
#include <stack>
#include <vector>

#else

#define NAMESPACE ft
#include "../../../map.hpp"
#include "../../../set.hpp"
#include "../../../stack.hpp"
#include "../../../vector.hpp"

#endif

#include <iostream>
#include <stdlib.h>

#define DEFAULT "\033[39m"
#define BLACK "\033[30m"
#define GREY "\033[90m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[94m"
#define MAGENTA "\033[35m"

void test_vector();
void test_map();
void test_stack();
void test_set();