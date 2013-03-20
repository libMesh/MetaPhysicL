
#include <iostream>
#include <utility>

std::pair<int,float> testfunc (std::pair<int,float> in)
{
  in.first += 4;
  return in;
}

#include "metaphysicl/dualnumber.h"
#include "metaphysicl/shadownumber.h"
#include "metaphysicl/numberarray.h"
#include "metaphysicl/numbervector.h"
#include "metaphysicl/sparsenumbervector.h"
#include "metaphysicl/sparsenumberstruct.h"

using namespace MetaPhysicL;

int main(void)
{
  std::cout << "4+4=" << testfunc(std::make_pair(4,2.0)).first << std::endl;

  DualNumber<float, float> DFF = 0;
  std::cos(DFF);

  ShadowNumber<float, float> SN = 0;
  std::cos(SN);

  NumberArray<3, float> NA = 0;
  std::cos(NA);

  NumberVector<3, float> NV = 0;
  std::cos(NV);

  SparseNumberVectorUnitVector<3, 2, float>::type SNV;
  std::sin(SNV);

  SparseNumberStructUnitVector<3, 2, float>::type SNS;
  std::sin(SNS);
}