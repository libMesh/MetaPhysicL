#include <metaphysicl/dynamicsparsenumberarray.h>

using namespace MetaPhysicL;

int main()
{
  {
    DynamicSparseNumberArray<double, unsigned int> a;
    metaphysicl_assert(a.nude_data().capacity() == 0);
    a.insert(0) = 1;
  }
  {
    DynamicSparseNumberArray<double, unsigned int> a;
    metaphysicl_assert(a.nude_data().capacity() > 0);
    a.insert(0) = 1;
    a.insert(1) = 1;
  }
  {
    DynamicSparseNumberArray<double, unsigned int> a;
    metaphysicl_assert(a.nude_data().capacity() > 1);
  }
}
