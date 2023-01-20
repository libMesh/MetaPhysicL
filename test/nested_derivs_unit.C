#include "metaphysicl/dualnumberarray.h"

using namespace MetaPhysicL;
typedef double Real;

typedef DualNumber<Real, NumberArray<4, Real>> ADReal;
typedef DualNumber<ADReal, NumberArray<2, ADReal>> CompoundADType;

int
main(int argc, const char * argv[])
{
  ADReal u0 = 2;
  u0.derivatives()[0] = 1;
  ADReal u1 = 3;
  u1.derivatives()[1] = 1;
  ADReal v0 = 4;
  v0.derivatives()[2] = 1;
  ADReal v1 = 5;
  v1.derivatives()[3] = 1;

  ADReal u = 0.5 * (u0 + u1);
  u = u + 0;
  ADReal v = 0.5 * (v0 + v1);

  const ADReal zero(0, 0);
  const ADReal one(1, 0);

  CompoundADType u_c(u, zero);
  auto & u_cd = u_c.derivatives();
  u_cd[0] = one;

  CompoundADType v_c(v, zero);
  auto & v_cd = v_c.derivatives();
  v_cd[1] = one;

  u_c = u_c + zero;
}
