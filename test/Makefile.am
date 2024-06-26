check_PROGRAMS  =
check_PROGRAMS += avoid_spurious_fpe_unit # first, to avoid confusion later
check_PROGRAMS += compare_types_unit
check_PROGRAMS += derivs_unit
check_PROGRAMS += nd_derivs_unit
check_PROGRAMS += complex_derivs_unit
check_PROGRAMS += divgrad_unit
check_PROGRAMS += dynamic_sparse_vector_navier_unit
check_PROGRAMS += dynamic_sparse_vector_pde_unit
check_PROGRAMS += dynamic_std_array_wrapper_unit
check_PROGRAMS += identities_unit
check_PROGRAMS += instantiations_unit
check_PROGRAMS += main_unit
check_PROGRAMS += shadow_dynamic_sparse_vector_navier_unit
check_PROGRAMS += shadow_dynamic_sparse_vector_pde_unit
check_PROGRAMS += shadow_sparse_struct_navier_unit
check_PROGRAMS += shadow_sparse_struct_pde_unit
check_PROGRAMS += shadow_sparse_vector_navier_unit
check_PROGRAMS += shadow_sparse_vector_pde_unit
check_PROGRAMS += shadow_vector_navier_unit
check_PROGRAMS += shadow_vector_pde_unit
check_PROGRAMS += sparse_derivs_unit
check_PROGRAMS += sparse_identities_unit
check_PROGRAMS += sparse_struct_navier_unit
check_PROGRAMS += sparse_struct_pde_unit
check_PROGRAMS += sparse_vector_navier_unit
check_PROGRAMS += sparse_vector_pde_unit
check_PROGRAMS += testheaders_unit
check_PROGRAMS += testopt_unit
check_PROGRAMS += vector_navier_unit
check_PROGRAMS += vector_pde_unit
check_PROGRAMS += parallel_unit
check_PROGRAMS += pde_unit
check_PROGRAMS += dynamic_sparse_number_base_unit
check_PROGRAMS += ct_types_unit

AM_TESTS_ENVIRONMENT =
if CXX14_ENABLED
  check_PROGRAMS += dualnamedarray_unit
  check_PROGRAMS += namedindexarray_unit
  AM_TESTS_ENVIRONMENT += TEST_CXX14=yes; export TEST_CXX14;
endif

if CXX11_ENABLED
  check_PROGRAMS += physics_unit
  AM_TESTS_ENVIRONMENT += TEST_CXX11=yes; export TEST_CXX11;
endif

AM_CPPFLAGS  =
AM_CPPFLAGS += -I$(top_srcdir)/src/core/include
AM_CPPFLAGS += -I$(top_srcdir)/src/graphs/include
AM_CPPFLAGS += -I$(top_srcdir)/src/numerics/include
AM_CPPFLAGS += -I$(top_srcdir)/src/utilities/include
AM_CPPFLAGS += -I$(top_builddir)/src/utilities/include #metaphysicl_version.h, metaphysicl_config.h

AM_CXXFLAGS = $(ACSM_CXXFLAGS_OPT) $(ACSM_ANY_WERROR_FLAG) $(ACSM_ANY_PARANOID_FLAGS)

AM_LDFLAGS = $(top_builddir)/src/libmetaphysicl.la

# Sources for these tests
avoid_spurious_fpe_unit_SOURCES = avoid_spurious_fpe_unit.C
compare_types_unit_SOURCES = compare_types_unit.C
derivs_unit_SOURCES = derivs_unit.C
nd_derivs_unit_SOURCES = nd_derivs_unit.C
nd_derivs_unit_SOURCES += math_structs.h
complex_derivs_unit_SOURCES = complex_derivs_unit.C
divgrad_unit_SOURCES = divgrad_unit.C
dualnamedarray_unit_SOURCES = dualnamedarray_unit.C
dynamic_sparse_vector_navier_unit_SOURCES =  dynamic_sparse_vector_navier_unit.C
dynamic_sparse_vector_navier_unit_SOURCES += navier_unit.h
dynamic_sparse_vector_navier_unit_SOURCES += testing.h
dynamic_sparse_vector_pde_unit_SOURCES =  dynamic_sparse_vector_pde_unit.C
dynamic_sparse_vector_pde_unit_SOURCES += pde_unit.h
dynamic_sparse_vector_pde_unit_SOURCES += testing.h
dynamic_std_array_wrapper_unit_SOURCES = dynamic_std_array_wrapper_unit.C
dynamic_sparse_number_base_unit_SOURCES =  dynamic_sparse_number_base_unit.C
identities_unit_SOURCES = identities_unit.C
instantiations_unit_SOURCES = instantiations_unit.C
main_unit_SOURCES = main_unit.C
namedindexarray_unit_SOURCES =  namedindexarray_unit.C
shadow_dynamic_sparse_vector_navier_unit_SOURCES =  shadow_dynamic_sparse_vector_navier_unit.C
shadow_dynamic_sparse_vector_navier_unit_SOURCES += navier_unit.h
shadow_dynamic_sparse_vector_navier_unit_SOURCES += testing.h
shadow_dynamic_sparse_vector_pde_unit_SOURCES =  shadow_dynamic_sparse_vector_pde_unit.C
shadow_dynamic_sparse_vector_pde_unit_SOURCES += pde_unit.h
shadow_dynamic_sparse_vector_pde_unit_SOURCES += testing.h
shadow_sparse_struct_navier_unit_SOURCES =  shadow_sparse_struct_navier_unit.C
shadow_sparse_struct_navier_unit_SOURCES += navier_unit.h
shadow_sparse_struct_navier_unit_SOURCES += testing.h
shadow_sparse_struct_pde_unit_SOURCES =  shadow_sparse_struct_pde_unit.C
shadow_sparse_struct_pde_unit_SOURCES += pde_unit.h
shadow_sparse_struct_pde_unit_SOURCES += testing.h
shadow_sparse_vector_navier_unit_SOURCES =  shadow_sparse_vector_navier_unit.C
shadow_sparse_vector_navier_unit_SOURCES += navier_unit.h
shadow_sparse_vector_navier_unit_SOURCES += testing.h
shadow_sparse_vector_pde_unit_SOURCES =  shadow_sparse_vector_pde_unit.C
shadow_sparse_vector_pde_unit_SOURCES += pde_unit.h
shadow_sparse_vector_pde_unit_SOURCES += testing.h
shadow_vector_navier_unit_SOURCES =  shadow_vector_navier_unit.C
shadow_vector_navier_unit_SOURCES += navier_unit.h
shadow_vector_navier_unit_SOURCES += testing.h
shadow_vector_pde_unit_SOURCES =  shadow_vector_pde_unit.C
shadow_vector_pde_unit_SOURCES += pde_unit.h
shadow_vector_pde_unit_SOURCES += testing.h
sparse_derivs_unit_SOURCES = sparse_derivs_unit.C
sparse_identities_unit_SOURCES = sparse_identities_unit.C
sparse_struct_navier_unit_SOURCES =  sparse_struct_navier_unit.C
sparse_struct_navier_unit_SOURCES += navier_unit.h
sparse_struct_navier_unit_SOURCES += testing.h
sparse_struct_pde_unit_SOURCES =  sparse_struct_pde_unit.C
sparse_struct_pde_unit_SOURCES += pde_unit.h
sparse_struct_pde_unit_SOURCES += testing.h
sparse_vector_navier_unit_SOURCES =  sparse_vector_navier_unit.C
sparse_vector_navier_unit_SOURCES += navier_unit.h
sparse_vector_navier_unit_SOURCES += testing.h
sparse_vector_pde_unit_SOURCES =  sparse_vector_pde_unit.C
sparse_vector_pde_unit_SOURCES += pde_unit.h
sparse_vector_pde_unit_SOURCES += testing.h
physics_unit_SOURCES = physics_unit.C
testheaders_unit_SOURCES = testheaders_unit.C
testopt_unit_SOURCES = testopt_unit.C
vector_navier_unit_SOURCES =  vector_navier_unit.C
vector_navier_unit_SOURCES += navier_unit.h
vector_navier_unit_SOURCES += testing.h
vector_pde_unit_SOURCES =  vector_pde_unit.C
vector_pde_unit_SOURCES += pde_unit.h
vector_pde_unit_SOURCES += testing.h
parallel_unit_SOURCES = parallel_unit.C
pde_unit_SOURCES = pde_unit.C
pde_unit_SOURCES += testing.h
ct_types_unit_SOURCES = ct_types_unit.C

#Define tests to actually be run. TESTS is a special automake variable
TESTS  = run_unit_tests.sh

#-------------
# MASA support
#-------------
if MASA_ENABLED
  AM_CPPFLAGS += $(MASA_CXXFLAGS)
  AM_LDFLAGS  += $(MASA_LIBS)
endif

if VEXCL_ENABLED
  AM_CPPFLAGS += $(VEXCL_CPPFLAGS)
  AM_LDFLAGS  += $(VEXCL_LDFLAGS)
  LIBS        += $(VEXCL_LIBS)
endif

if TIMPI_ENABLED
  AM_CPPFLAGS += $(TIMPI_CPPFLAGS)
if TIMPI_LIB_ENABLED
  LIBS += $(TIMPI_LIBS)
endif
endif

CLEANFILES =

if CODE_COVERAGE_ENABLED
  CLEANFILES += *.gcda *.gcno
endif

if METAPHYSICL_LICENSESTAMPEXISTS
BUILT_SOURCES   = .license.stamp

.license.stamp: $(top_srcdir)/LICENSE
	$(top_srcdir)/src/common/lic_utils/update_license.pl $(top_srcdir)/LICENSE \
	$(top_srcdir)/test/*.C
	echo 'updated source license headers' >$@

CLEANFILES += .license.stamp
endif

# Required for AX_AM_MACROS
###@INC_AMINCLUDE@
