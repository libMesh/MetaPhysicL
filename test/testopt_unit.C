
#include <iostream>
#include <utility>

std::pair<int,float> testfunc (std::pair<int,float> in)
{
  in.first += 4;
  return in;
}

#include "metaphysicl/dualnumber.h"
#include "metaphysicl/metaphysicl_exceptions.h"
#include "metaphysicl/numberarray.h"
#include "metaphysicl/numbervector.h"
#include "metaphysicl/shadownumber.h"
#include "metaphysicl/sparsenumbervector.h"
#include "metaphysicl/sparsenumberstruct.h"

using namespace MetaPhysicL;

int main(void)
{
  MetaPhysicL::enableFPE(true);

  std::cout << "4+4=" << testfunc(std::make_pair(4,2.0)).first << std::endl;

  DualNumber<float, float> DFF = 0;
  cos(DFF);

  ShadowNumber<float, float> SN = 0;
  cos(SN);

  NumberArray<3, float> NA = 0;
  cos(NA);

  NumberVector<3, float> NV = 0;
  // Avoid unused variable compiler warnings.
  (void)NV;
  // cos(NV);

  SparseNumberVectorUnitVector<3, 2, float>::type SNV;
  SparseNumberVectorUnitVector<3, 1, float>::type SNV2;
  sin(SNV);
  max(SNV, SNV);
  max(SNV, SNV2);
  max(1.0, SNV);
  max(SNV, 1);

  SparseNumberStructUnitVector<3, 2, float>::type SNS;
  sin(SNS);
  max(SNS, SNS);
  max(1.0, SNS);
  max(SNS, 1);

  const SparseNumberVector<unsigned int,
    Container<UnsignedIntType<0u, NullType>,
      Container<UnsignedIntType<1u, NullType>,
        Container<UnsignedIntType<5u, NullType>,
        NullContainer<UnsignedIntType<0> >, ValueLessThan>,
      ValueLessThan>,
    ValueLessThan> > max_test_1 = 0;

  const SparseNumberVector<unsigned int,
    Container<UnsignedIntType<1u, NullType>,
      Container<UnsignedIntType<2u, NullType>,
        Container<UnsignedIntType<6u, NullType>,
        NullContainer<UnsignedIntType<0> >, ValueLessThan>,
      ValueLessThan>,
    ValueLessThan> > max_test_2 = 0;

  max(max_test_1, max_test_2);

  // We need non-zero derivative here, or exp() gives us a derivative
  // of infinity*0 and throws an FP exception
  DualNumber<float, float> big = {800.0, 1};
  if (!isinf(exp(big)))
    return 1;

  return 0;
}
