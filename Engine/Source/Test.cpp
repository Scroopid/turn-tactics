#include "Test.h"
#include <iostream>

namespace Shady
{
  ENGINE_API void test_print(int num)
  {
    std::cout << "This is just a test " << num << std::endl;
  }
}
