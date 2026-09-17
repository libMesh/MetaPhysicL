//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MetaPhysicL - A metaprogramming library for physics calculations
//
//-----------------------------------------------------------------------el-

#ifndef METAPHYSICL_SPARSE_PATTERN_COMPARE_H
#define METAPHYSICL_SPARSE_PATTERN_COMPARE_H

#include <cstddef>

namespace MetaPhysicL
{

// Element-wise index-pattern equality. Works for any index storage exposing size()
// and operator[] (std::vector for DynamicSparseNumberArray, the array wrapper for
// SemiDynamicSparseNumberArray), which do not uniformly provide operator!=.
// Kept in this tiny standalone header so consumers (e.g. libMesh TypeTensor) can use
// the shared-sparsity tensor routines without pulling in the container headers.
template <typename IdxA, typename IdxB>
inline bool
same_index_pattern(const IdxA & a, const IdxB & b)
{
  if (a.size() != b.size())
    return false;
  for (std::size_t k = 0; k < a.size(); ++k)
    if (a[k] != b[k])
      return false;
  return true;
}

} // namespace MetaPhysicL

#endif // METAPHYSICL_SPARSE_PATTERN_COMPARE_H
