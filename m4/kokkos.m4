AC_DEFUN([CONFIGURE_KOKKOS],
[
  AC_ARG_VAR([KOKKOS_CXXFLAGS], [Extra C++ flags for compiling Kokkos sources])

  dnl -- Backend choice (defaults to CUDA example; pick what you need)
  AC_ARG_WITH([kokkos-backend],
    [AS_HELP_STRING([--with-kokkos-backend=BACKEND],
      [cuda|hip|sycl|openmp|serial (default: cuda)])],
    [KOKKOS_BACKEND="$withval"], [KOKKOS_BACKEND=cuda])

  dnl -- Kokkos install
  AC_ARG_WITH([kokkos],
    [AS_HELP_STRING([--with-kokkos=PATH],
      [Kokkos prefix (e.g. /opt/kokkos or install prefix)])],
    [KOKKOS_PREFIX="$withval"], [KOKKOS_PREFIX=""])

  AS_IF([test "x$KOKKOS_PREFIX" != "x"], [
    KOKKOS_CPPFLAGS="-DMETAPHYSICL_KOKKOS_COMPILATION -I$KOKKOS_PREFIX/include"
    KOKKOS_CXXFLAGS="--forward-unknown-to-host-compiler $KOKKOS_CXXFLAGS"
    KOKKOS_LDFLAGS="--forward-unknown-to-host-compiler -L$KOKKOS_PREFIX/lib -Wl,-rpath,$KOKKOS_PREFIX/lib"
    KOKKOS_LIBS="-lkokkoscore"

    KOKKOS_CFG="$KOKKOS_PREFIX/include/KokkosCore_config.h"
    AS_IF([! test -r "$KOKKOS_CFG"], [
      AC_MSG_ERROR([Cannot read $KOKKOS_CFG])
    ])

    AC_PROG_GREP
    AC_PROG_SED
    AS_IF(["$GREP" -F -q '#define KOKKOS_ENABLE_OPENMP' \
          "$KOKKOS_CFG"],
      [have_kokkos_openmp=yes],
      [have_kokkos_openmp=no])

    if test "x$have_kokkos_openmp" = "xyes"; then
      KOKKOS_CXXFLAGS="-fopenmp $KOKKOS_CXXFLAGS"
      KOKKOS_LDFLAGS="-fopenmp $KOKKOS_LDFLAGS"
    fi

    case "$KOKKOS_BACKEND" in
      cuda)
        AC_PATH_PROG([NVCC],[nvcc], [no], [$PATH])
        AS_IF([test "x$NVCC" = "xno"],
          [AC_MSG_ERROR([nvcc not found. Install CUDA.])])
        KOKKOS_CXX="$NVCC"
        KOKKOS_CXXFLAGS="-x cu $KOKKOS_CXXFLAGS"

        dnl
        dnl credit to ChatGPT for the ensuing parsing of arch's from kokkos config
        dnl

        dnl harvest defined arch macros from Kokkos config
        AC_MSG_CHECKING([for Kokkos defined architectures])
        ax_kokkos_arch_lines=`$GREP '^[[[:space:]]]*#define[[:space:]]\{1,\}KOKKOS_ARCH_[A-Za-z0-9_][A-Za-z0-9_]*' "$KOKKOS_CFG" \
          | $SED -n 's/.*KOKKOS_ARCH_\([[A-Za-z0-9_]][[A-Za-z0-9_]]*\).*/\1/p'`
        AC_MSG_RESULT([$ax_kokkos_arch_lines])

        dnl Keep only GPU-ish tokens we know how to map
        ax_kokkos_arch_gpu=`printf "%s\n" "$ax_kokkos_arch_lines" \
          | "$GREP" '^\(KEPLER\(30\|32\|35\|37\)\{0,1\}\|MAXWELL\(50\|52\|53\)\{0,1\}\|PASCAL\(60\|61\)\{0,1\}\|VOLTA\(70\|72\)\{0,1\}\|  TURING75\|AMPERE\(80\|86\)\{0,1\}\|ADA89\|HOPPER\(90\)\{0,1\}\|AMD_GFX[0-9A-Za-z]\{1,\}\)$' \
          || true`

        dnl Prefer numbered macros; if both generic and numbered exist, numbered will appear too.
        ax_cuda_sms=
        for t in $ax_kokkos_arch_gpu; do
          case "$t" in
            KEPLER30|KEPLER)  ax_cuda_sms="$ax_cuda_sms 30" ;;
            KEPLER32)         ax_cuda_sms="$ax_cuda_sms 32" ;;
            KEPLER35)         ax_cuda_sms="$ax_cuda_sms 35" ;;
            KEPLER37)         ax_cuda_sms="$ax_cuda_sms 37" ;;
            MAXWELL|MAXWELL50) ax_cuda_sms="$ax_cuda_sms 50" ;;
            MAXWELL52)        ax_cuda_sms="$ax_cuda_sms 52" ;;
            MAXWELL53)        ax_cuda_sms="$ax_cuda_sms 53" ;;
            PASCAL|PASCAL60)  ax_cuda_sms="$ax_cuda_sms 60" ;;
            PASCAL61)         ax_cuda_sms="$ax_cuda_sms 61" ;;
            VOLTA|VOLTA70)    ax_cuda_sms="$ax_cuda_sms 70" ;;
            VOLTA72)          ax_cuda_sms="$ax_cuda_sms 72" ;;
            TURING75)         ax_cuda_sms="$ax_cuda_sms 75" ;;
            AMPERE|AMPERE80)  ax_cuda_sms="$ax_cuda_sms 80" ;;
            AMPERE86)         ax_cuda_sms="$ax_cuda_sms 86" ;;
            ADA89)            ax_cuda_sms="$ax_cuda_sms 89" ;;
            HOPPER|HOPPER90)  ax_cuda_sms="$ax_cuda_sms 90" ;;
            AMD_GFX*)         ;; dnl handled below in HIP section
          esac
        done

        dnl Unique & sorted
        ax_cuda_sms=`printf "%s\n" $ax_cuda_sms | awk '!seen[$0]++'`

        dnl Emit nvcc -gencode flags (compute+code pairs)
        if test "x$ax_cuda_sms" != x; then
          for sm in $ax_cuda_sms; do
            KOKKOS_CXXFLAGS="-gencode=arch=compute_${sm},code=sm_${sm} $KOKKOS_CXXFLAGS"
          done
        fi
        ;;
      hip)
        AC_PATH_PROG([HIPCC],[hipcc],[no],[$PATH])
        AS_IF([test "x$HIPCC" = "xno"],
          [AC_MSG_ERROR([hipcc not found; install ROCm HIP.])])
        KOKKOS_CXX="$HIPCC"
        ;;
      sycl)
        AC_PATH_PROG([ICPX],[icpx],[no],[$PATH])
        AS_IF([test "x$ICPX" = "xno"],
          [AC_MSG_ERROR([icpx (oneAPI) not found for SYCL backend.])])
        KOKKOS_CXX="$ICPX"
        KOKKOS_CXXFLAGS="-fsycl $KOKKOS_CXXFLAGS"
        ;;
      openmp)
        KOKKOS_CXX="$CXX"
        KOKKOS_CXXFLAGS="-fopenmp $KOKKOS_CXXFLAGS"
        ;;
      serial|*)
        KOKKOS_CXX="$CXX"
        ;;
    esac

  ], [
    KOKKOS_CXX=""
    KOKKOS_CPPFLAGS=""
    KOKKOS_CXXFLAGS=""
    KOKKOS_LDFLAGS=""
    KOKKOS_LIBS=""
  ])


  AC_SUBST([KOKKOS_CXX])
  AC_SUBST([KOKKOS_CXXFLAGS])
  AC_SUBST([KOKKOS_LIBS])
  AC_SUBST([KOKKOS_CPPFLAGS])
  AC_SUBST([KOKKOS_LDFLAGS])

  AM_CONDITIONAL([KOKKOS_ENABLED], [test "x$KOKKOS_CXX" != "x"])
])
