// clang++ (16.0.6 or 15.0.7) -O2 gives a SIGFPE when running this
// clang++ -O1, -O0, or -O3 give no SIGFPE
// clang++ 14.0.6 or 13.0.1 give no SIGFPE
// clang++ -fsanitize=undefined,address -O2 gives no warnings and no SIGFPE!

#include <iostream>

// Most of this test is library-free, just aimed at testing the
// compiler, but we need to make sure we're not using feenableexcept()
// unless we detected it, so we need MetaPhysicL::enableFPE()
#include "metaphysicl/metaphysicl_exceptions.h"

template <typename T, typename D>
struct DualNumber
{
  DualNumber () : val(0), deriv(0) {}

  template <typename T2, typename D2>
  DualNumber (DualNumber<T2,D2>& a) :
    val(a.val), deriv(a.deriv) {}

  T val;
  D deriv;
};


template <std::size_t N, typename T>
struct NumberArray
{
  NumberArray (T a)
    { for (std::size_t i=0; i != N; ++i) _data[i] = a; }

  // Access a[i] via a._data[i] and no more SIGFPE!
  template <typename T2>
  NumberArray (NumberArray<N,T2>& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] = a[i]; }

  T& operator[](std::size_t i)
    {
      // Remove this assert, even just std::endl, and no more SIGFPE!
      if (!(i < N)) { std::cerr << "Assert failed." << std::endl; }
      return _data[i]; }

  NumberArray<N,T>& operator/= (const T& a)
    { for (std::size_t i=0; i != N; ++i) _data[i] /= a; return *this; }

  T _data[N];
};


static const unsigned int N = 9; // 9 or 10 SIGFPEs
// static const unsigned int N = 8; // 8 or fewer doesn't SIGFPE

int main(int, char *[])
{
  MetaPhysicL::enableFPE(true);

  typedef float Scalar;  // double doesn't SIGFPE!
  DualNumber<Scalar, NumberArray<N, Scalar>> dn;

  // No copy made, no SIGFPE!
  DualNumber<Scalar, NumberArray<N, Scalar>> dncopy{dn};

  // Changing only one member doesn't SIGFPE!
  // 2 (or 2.0) in both denominators doesn't SIGFPE!
  // Modifying dn *or* dncopy gives a SIGFPE!
  dncopy.val /= 3.0;
  dncopy.deriv /= 3.0;

  DualNumber<double, NumberArray<N, double>> broken_conversion{dn};

  return 0;
}
