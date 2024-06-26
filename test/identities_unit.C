#include <cstdlib> // rand()
#include <iostream>
#include <limits>

#include "metaphysicl_config.h"

#include "metaphysicl/metaphysicl_exceptions.h"
#include "metaphysicl/numberarray.h"
#include "metaphysicl/numbervector.h"
#include "metaphysicl/sparsenumberarray.h"

static const unsigned int N = 10; // test pts.

using namespace MetaPhysicL;

#define one_test(test_func) \
  error_vec = test_func; \
  { int new_returnval = test_error_vec(random_vec, error_vec); \
  if (new_returnval) \
    std::cerr << "Failed test: " << #test_func << std::endl; \
  returnval = returnval || new_returnval; }

template <typename Vector>
int test_error_vec (const Vector& random_vec,
                    const Vector& error_vec)
{
  using std::max;
  using std::fabs;

  typedef typename Vector::value_type Scalar;

  static const Scalar tol = std::numeric_limits<Scalar>::epsilon() * 10;

  Scalar max_abs_error = 0;

  for (unsigned int i=0; i != error_vec.size(); ++i)
    {
      max_abs_error = max(max_abs_error, fabs(error_vec[i]));

      // Handle NaNs properly.  Testing max_abs_error for NaN is
      // impossible because IEEE sucks:
      // https://en.wikipedia.org/wiki/IEEE_754_revision#min_and_max
      if (max_abs_error > tol || error_vec[i] != error_vec[i])
        {
	  std::cerr << "Value " << random_vec[i] <<
		       "\nError " << error_vec[i] <<
		       "\nTol   " << tol << std::endl;
	  return 1;
        }
    }

  return 0;
}

template <typename Vector>
int vectester (void)
{
  using std::abs;
  using std::acos;
  using std::ceil;
  using std::cos;
  using std::cosh;
  using std::exp;
  using std::fabs;
  using std::floor;
  using std::log;
  using std::log10;
  using std::pow;
  using std::sin;
  using std::sinh;
  using std::tan;
  using std::tanh;

  typedef typename Vector::value_type Scalar;

  Vector random_vec;

  Vector error_vec = 0;

  std::srand(12345); // Fixed seed for reproduceability of failures

  // Avoid divide by zero errors or acos(x>1) NaNs later
  for (unsigned int i=0; i != random_vec.size(); ++i)
    random_vec[i] = .25 + (static_cast<Scalar>(std::rand())/static_cast<Scalar>(RAND_MAX)/2);

  Scalar pi = acos(Scalar(-1));

  int returnval = 0;

  one_test(log(exp(random_vec)) - random_vec);
  one_test(exp(log(random_vec)) - random_vec);
  one_test(exp(random_vec) - pow(exp(Scalar(1)), random_vec));

  one_test(tan(random_vec) - sin(random_vec)/cos(random_vec));
  one_test(random_vec - cos(acos(random_vec)));
  one_test(1 - pow(sin(random_vec), 2) - pow(cos(random_vec), 2));
  one_test(cos(random_vec) - sin(random_vec + pi/2));

  one_test(tanh(random_vec) - sinh(random_vec)/cosh(random_vec));
  one_test(1 + pow(sinh(random_vec), 2) - pow(cosh(random_vec), 2));

  one_test(log10(random_vec) - log(random_vec)/log(Scalar(10)));

  one_test(ceil(random_vec / 2 - 1));
  one_test(fabs(random_vec-.75) - abs(random_vec-.75));

  return returnval;
}

int main(int argc, char * argv[])
{
  MetaPhysicL::enableFPE(true);

  int returnval = 0;
  returnval = returnval || vectester<NumberArray<N, float> >();
  returnval = returnval || vectester<NumberArray<N, double> >();

  bool use_long_double = true;
  std::string disarg = "--disable-long-double";
  for (int i = 1; i < argc; ++i)
    if (disarg == argv[i])
      use_long_double = false;

  if (use_long_double)
    returnval = returnval || vectester<NumberArray<N, long double> >();

  // We no longer treat vectors like arrays for built-in functions, so
  // most of the identities above make no sense.
  /*
  returnval = returnval || vectester<NumberVector<N, float> >();
  returnval = returnval || vectester<NumberVector<N, double> >();
  returnval = returnval || vectester<NumberVector<N, long double> >();
  */

  return returnval;
}
