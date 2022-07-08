#include <metaphysicl/pool_dynamicsparsenumberarray.h>

using namespace MetaPhysicL;

static SharedPool<DynamicSparseNumberArray<double, unsigned int>> test_pool;

namespace MetaPhysicL
{
template <>
inline
SharedPool<DynamicSparseNumberArray<double, unsigned int>> & getPool<double, unsigned int>()
{
  return test_pool;
}
}

int main()
{
  {
    PoolDynamicSparseNumberArray<double, unsigned int> a;
    a.insert(0) = 1;
  }
  {
    PoolDynamicSparseNumberArray<double, unsigned int> a;
    a.insert(0) = 1;
  }
}
